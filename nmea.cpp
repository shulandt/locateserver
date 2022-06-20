#include "nmea.h"
#include <stdlib.h>
#include <string.h>
//-----------------------------------------------------------------------------------
Nmea::Nmea() {
  pos = 0;
  foundBegin = false;
}
//-----------------------------------------------------------------------------------
bool Nmea::decode(char sym) {
  if(sym == '$') {
    foundBegin = true;
    pos = 0;
  }
  if(foundBegin) {
    buff[pos] = sym;
    pos++;
    if(pos == buffSize) {
      foundBegin = false;  
      return false;
    }
    if(sym == 0x0A) {
      foundBegin = false;
      if(!checkSum())
        return false;
      buff[pos] = 0;      
      return parse();    
    }
  }
  return false;
}
//-----------------------------------------------------------------------------------
bool Nmea::checkSum() {
  unsigned char sum = 0;
  for(int i = 1; i < pos - 5; i++)
    sum ^= buff[i];
  if(sum == strtol(buff + pos - 4, NULL, 16))
    return true;
  return false;    
}
//-----------------------------------------------------------------------------------
bool Nmea::parse() {
  for(int i = 1; i < pos; i++) {
    if((buff[i] == ',') || (buff[i] == '*')) {
      strncpy(name, buff + i - 3, 3);
      name[3] = 0;
      if(!strcmp(name, "GGA"))
        return parseGGA();
      if(!strcmp(name, "RMC"))  
        return parseRMC();
      if(!strcmp(name, "EDB"))  
        return parsePLEDB();
      if(!strcmp(name, "EDA"))  
        return parsePLEDA();  	  	
      return true;
    }
  }
  return false;
}
//-----------------------------------------------------------------------------------
char* Nmea::getSentence() {
  return buff;
}
//-----------------------------------------------------------------------------------
char* Nmea::getSentenceName() {
  return name;  
}
//-----------------------------------------------------------------------------------
bool Nmea::parseGGA() {
  int numField = 0;
  for(int i = 1; i < pos; i++) {
    if(buff[i] == ',') {
      numField++;
      if(buff[i + 1] == ',')
        continue;
      switch(numField) {
        case 2:
          latitude = (buff[i + 1] - 48) * 10 + (buff[i + 2] - 48) + atof(buff + i + 3) / 60.;
          break;
        case 3:
          if(buff[i + 1] == 'S')
            latitude = -latitude;
          break;
        case 4:
          longitude = (buff[i + 1] - 48) * 100 + (buff[i + 2] - 48) * 10 + (buff[i + 3] - 48) + atof(buff + i + 4) / 60.;
          break;
        case 5:
          if(buff[i + 1] == 'W')
            longitude = -longitude;
          break;
        case 6:
          fixQuality = atoi(buff + i + 1);
          break;
        case 7:
          numSat = atoi(buff + i + 1);
          break;
        case 8:
          hdop = atof(buff + i + 1);
          break;                        
        case 9:
          altitude = atof(buff + i + 1);
          break;               
      }
    }
  }
  return true;
}
//-----------------------------------------------------------------------------------
bool Nmea::parseRMC() {
  return true;
}
//-----------------------------------------------------------------------------------
bool Nmea::parsePLEDB() {
  int numField = 0;
  for(int i = 1; i < pos; i++) {
    if(buff[i] == ',') {
      numField++;
      if(buff[i + 1] == ',')
        continue;
      switch(numField) {
        case 1:
		  distance = atoi(buff + i + 1); 
          break;
        case 2:
          timeToExecute = atoi(buff + i + 1);
          break;
        case 3:
          batteryPercent = atoi(buff + i + 1);
          break;
      }
    }
  }	
  return true;
}
//-----------------------------------------------------------------------------------
bool Nmea::parsePLEDA() {
  int numField = 0;
  for(int i = 1; i < pos; i++) {
    if(buff[i] == ',') {
      numField++;
      if(buff[i + 1] == ',')
        continue;
      switch(numField) {
        case 1:
		  fixLatitude = atof(buff + i + 1); 
          break;
        case 2:
          fixLongitude = atof(buff + i + 1);
          break;
        case 3:
          fixRadius = atoi(buff + i + 1);
          break;
        case 4:
          timeLimit = atoi(buff + i + 1);
          break;		  
      }
    }
  }	
  return true;
}
//-----------------------------------------------------------------------------------