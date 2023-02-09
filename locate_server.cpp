#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>

#include "binr.h"
#include "utils.h"

// Server port number
int listenPort = 5000;
// input buffer size
const int inBufSize = 64;
// max number of clients
const int max_clients = 30;

struct ClientData {
    char  imei[16];
    float lat;
    float lon;
    short alt;
    unsigned short vel;
    unsigned char  sat;
    unsigned char  hdop;
    unsigned char  fix;
    unsigned char  timeToExec;
    long  dist;
    unsigned char  bat;
    float fixLat;
    float fixLon;
    unsigned short distLim;
    unsigned short timeLim;
};

uint8_t recvBuf[inBufSize] = {};
struct sockaddr_in cl_addr;
socklen_t addrlen = sizeof(cl_addr);
int client_socket[max_clients];
Binr binr[max_clients];
ClientData clientData[max_clients];

char fromClientFileName[] = "/var/www/html/backend_data/from_client";
char toClientFileName[] = "/var/www/html/backend_data/to_client";
char directoryName[] = "/var/www/html/backend_data/";
//---------------------------------------------------------------------------
void* fileReadThread(void* param);
void removeFromClientFile(int i);
//---------------------------------------------------------------------------
void signal_callback_handler(int signum) {
  printf("Server stopped\n");
  for(int i = 0; i < max_clients; i++) {
    removeFromClientFile(i);   
  }
  exit(signum);
}
//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  int master_socket;
  int addrlen;
  long client_activity[max_clients];
  int activity;
  int max_sd;
  struct sockaddr_in servAddr;
  fd_set readfds;               // Set of socket descriptors
  struct timeval tv;
  struct timespec ts;
  pthread_t tid;
  pthread_attr_t attr;
  
  signal(SIGINT, signal_callback_handler);  
  
  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, fileReadThread, NULL);
  
  for(int i = 0; i < max_clients; i++) {
    client_socket[i] = 0;
    client_activity[i] = 0;
    removeFromClientFile(i);
  }
  
  srand(time(NULL));

  master_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(master_socket <= 0)
  {
    printf("Error creating socket!\n");
    exit(1);
  }
  printf("Socket successfully created\n");

  // Set master socket to allow multiple connections,
  // this is just a good habit, it will work without this
  int enable = 1;
  if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    printf("setsockopt(SO_REUSEADDR) failed");

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(listenPort);

  if((bind(master_socket, (struct sockaddr*)&servAddr, sizeof(servAddr))) != 0)
  {
    printf("Socket bind failed\n");
    exit(1);
  }

  // Now server is ready to listen
  if((listen(master_socket, 10)) != 0)
  {
    printf("Listen failed\n");
    exit(1);
  }
  printf("Waiting for a connection\n");

  while(1)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts);
    // Clear the socket set
    FD_ZERO(&readfds);
    // Add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;
    // Add child sockets to set
    for(int i = 0; i < max_clients; i++)
    {
      // Socket descriptor
      int client_sockfd = client_socket[i];
      // If valid socket descriptor then add to read list
      if(client_sockfd > 0)
        FD_SET(client_sockfd, &readfds);
      // Highest file descriptor number, need it for the select function
      if(client_sockfd > max_sd)
        max_sd = client_sockfd;
    }
    // Wait for an activity on one of the sockets, timeout is NULL,
    // so wait indefinitely
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    activity = select(max_sd + 1, &readfds, NULL, NULL, &tv);
    if(activity < 0)
    {
      printf("Select error");
    }

    // If something happened on the master socket,
    // then it's an incoming connection
    if(FD_ISSET(master_socket, &readfds))
    {
      int client_sockfd = accept(master_socket, (struct sockaddr *)&cl_addr, (socklen_t*)&addrlen);
      if(client_sockfd < 0)
      {
        printf("Error accepting connection!\n");
        exit(1);
      }
      printf("New connection accepted\n");
      // Add new socket to array of sockets
      for(int i = 0; i < max_clients; i++)
      {
        // If position is empty
        if(client_socket[i] == 0)
        {
          client_socket[i] = client_sockfd;
          client_activity[i] = ts.tv_sec;
          break;
        }
      }
    }
    
    // Else its some IO operation on some other socket
    for(int i = 0; i < max_clients; i++)
    {
      int client_sockfd = client_socket[i];
      if(client_sockfd == 0)
        continue;
      // timeout connection check
      if(ts.tv_sec - client_activity[i] > 10) {
        printf("Client %s:%d disconnected\n", inet_ntoa(cl_addr.sin_addr), ntohs(cl_addr.sin_port));
        // Close the socket and mark as 0 in list for reuse
        close(client_sockfd);
        client_socket[i] = 0;
        removeFromClientFile(i);
      }       
      if(FD_ISSET(client_sockfd, &readfds))
      {
        client_activity[i] = ts.tv_sec;
        getpeername(client_sockfd, (struct sockaddr*)&cl_addr, (socklen_t*)&addrlen);
        // Read the incoming data
        memset(recvBuf, 0, sizeof(recvBuf));
        ssize_t recvLen = read(client_sockfd, recvBuf, sizeof(recvBuf));
        // Parsing the received data
        if(recvLen > 0)
        {
          for(int j = 0; j < recvLen; j++)
          { 
            if(binr[i].decode(recvBuf[j]))
            {
              //printf("binr num = %d\n", binr[i].getNum());
              switch(binr[i].getNum()) {
                case 0x01: {
                  clientData[i].lat = binr[i].getP01Lat();
                  clientData[i].lon = binr[i].getP01Lon();
                  clientData[i].alt = binr[i].getP01Alt();
                  clientData[i].vel = binr[i].getP01Vel();
                  clientData[i].sat = binr[i].getP01Sat();
                  clientData[i].hdop = binr[i].getP01Hdop();
                  clientData[i].fix = binr[i].getP01Fix();
                  clientData[i].timeToExec = binr[i].getP01Time();
                  clientData[i].dist = binr[i].getP01Dist();
                  clientData[i].bat = binr[i].getP01Bat();

                  char fileName[sizeof(fromClientFileName) + 3];
                  sprintf(fileName, "%s%02d", fromClientFileName, i);
                  FILE* fpPipe = fopen(fileName,"w");
                  if(fpPipe != NULL)
                  {
                    fprintf(fpPipe, "%s,%f,%f,%d,%ld,%d,%d\n", clientData[i].imei, clientData[i].lat, clientData[i].lon, clientData[i].sat,
                            clientData[i].dist, clientData[i].timeToExec, clientData[i].bat);
                    fflush(fpPipe);
                    fclose(fpPipe);
                  }
                  
                  char logFileName[sizeof(directoryName) + 16];
                  sprintf(logFileName, "%s%s", directoryName, clientData[i].imei);
                  FILE* logFile = fopen(logFileName, "a");
                  if(logFile) 
                  {
                    float lat = clientData[i].lat;
                    float lon = clientData[i].lon;
                    lat = fabs((int)lat + (lat - (int)lat) * 0.6) * 100.;
                    lon = fabs((int)lon + (lon - (int)lon) * 0.6) * 100.;
                    char ns, ew;
                    if(clientData[i].lat < 0)
                      ns = 'S';
                    else
                      ns = 'N';
                    if(clientData[i].lon < 0)
                      ew = 'W';
                    else
                      ew = 'E';
                    char nmeaBuff[130];
                    sprintf(nmeaBuff, "$GPGGA,%02d%02d%02d,%011.6f,%c,%012.6f,%c,%c,%d,%0.f,%.1f,M,,M,,*",
                            0, 0, 0, lat, ns, lon, ew, clientData[i].fix, clientData[i].sat, clientData[i].hdop,
                            clientData[i].alt);                    
                    fprintf(logFile, "%s\n", nmeaBuff);
                    char rmcStatus;
                    if(clientData[i].fix)
                      rmcStatus = 'A';
                    else
                      rmcStatus = 'V';
                    sprintf(nmeaBuff, "$GPRMC,%02d%02d%02d,%c,%011.6f,%c,%012.6f,%c,%05.1f,,%02d%02d%02d,,,%c**",
                            0, 0, 0, rmcStatus, lat, ns, lon, ew, clientData[i].vel * 3.6 / 1.852, 0, 0, 0, rmcStatus);                    
                    fprintf(logFile, "%s\n", nmeaBuff);
                    fclose(logFile);
                  }
                }
                break;
                case 0x02: {
                  strncpy(clientData[i].imei, binr[i].getP02imei(), 15);
                  clientData[i].imei[15] = 0;
                }
                break;
                case 0x03: {
                  clientData[i].fixLat = binr[i].getP03Lat();
                  clientData[i].fixLon = binr[i].getP03Lon();
                  clientData[i].distLim = binr[i].getP03Dist();
                  clientData[i].timeLim = binr[i].getP03Time();

                  char fileName[sizeof(fromClientFileName) + 3];
                  sprintf(fileName, "%s%02d", fromClientFileName, i);
                  FILE* fpPipe = fopen(fileName,"w");
                  if(fpPipe != NULL)
                  {
                    fprintf(fpPipe, "%s,%f,%f,%d,%ld,%d,%d,%f,%f,%d,%d\n", clientData[i].imei, clientData[i].lat, clientData[i].lon, clientData[i].sat,
                            clientData[i].dist, clientData[i].timeToExec, clientData[i].bat,
                            clientData[i].fixLat, clientData[i].fixLon, clientData[i].distLim, clientData[i].timeLim);
                    fflush(fpPipe);
                    fclose(fpPipe);
                  }
                }
                break;
              }
            }
            //printf("%02X ", recvBuf[j]);
          }
        }
        else if(recvLen == 0)
        {
          // Somebody disconnected
          printf("Client %s:%d disconnected\n", inet_ntoa(cl_addr.sin_addr), ntohs(cl_addr.sin_port));
          // Close the socket and mark as 0 in list for reuse
          close(client_sockfd);
          client_socket[i] = 0;
          removeFromClientFile(i);
        }
        else
        {
          printf("Error receiving data!\n");
          close(client_sockfd);
          client_socket[i] = 0;
          removeFromClientFile(i);
        }
      }
    }
  }
}
//-----------------------------------------------------------------------------
void* fileReadThread(void* param) {
    while(1) {
        for(int i = 0; i < max_clients; i++) {
            char fileName[sizeof(toClientFileName) + 3];
            sprintf(fileName, "%s%02d", toClientFileName, i);
            FILE* fpPipe = fopen(fileName,"r");
            if(fpPipe != NULL) {
                double lat, lon;
                int radius;
                int time_limit;
                int boom;
                int len = fscanf(fpPipe, "%lf,%lf,%d,%d,%d", &lat, &lon, &radius, &time_limit, &boom);
                fclose(fpPipe);
                remove(fileName);
                
                int client_sockfd = client_socket[i];
                if(client_sockfd == 0)
                    continue;
                if(len < 3)
                    write(client_sockfd, "\xA5\x5A\x01\x03\xEC", 5);
                else
                    write(client_sockfd, binr[i].getBuff03(lat, lon, radius, time_limit, boom), 18);
            }
        }   
        sleep(1);
    }   
}
//-----------------------------------------------------------------------------
void removeFromClientFile(int i) {
    char fileName[sizeof(fromClientFileName) + 3];
    sprintf(fileName, "%s%02d", fromClientFileName, i);
    remove(fileName);
}   
//-----------------------------------------------------------------------------
