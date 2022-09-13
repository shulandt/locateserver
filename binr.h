#ifndef binrH
#define binrH
//-----------------------------------------------------------------------------------
class Binr {
  public:
  static const int rxBuffSize = 26;
  Binr();
  bool  decode(char inChar);
  char* getBuff01(float lat, float lon, short alt, unsigned short vel, unsigned char sat, 
                  unsigned char hdop, unsigned char fix, unsigned char time, long dist, 
                  unsigned char bat);
  char* getBuff02(const char* imei);
  char* getBuff03(float lat, float lon, unsigned short dist, unsigned short time, unsigned char relay);
  float getP03Lat() { return rx.p03.lat; }
  float getP03Lon() { return rx.p03.lon; }
  unsigned short getP03Dist() { return rx.p03.dist; }
  unsigned short getP03Time() { return rx.p03.time; }
  unsigned char  getP03Relay() { return rx.p03.relay; }
  float getP01Lat() { return rx.p01.lat; }
  float getP01Lon() { return rx.p01.lon; }
  short getP01Alt() { return rx.p01.alt; }
  unsigned short getP01Vel() { return rx.p01.vel; }
  unsigned char  getP01Sat() { return rx.p01.sat; }
  unsigned char  getP01Hdop() { return rx.p01.hdop; }
  unsigned char  getP01Fix() { return rx.p01.fix; }
  unsigned char  getP01Time() { return rx.p01.time; }
  long  getP01Dist() { return rx.p01.dist; }
  unsigned char  getP01Bat() { return rx.p01.bat; }
  char* getP02imei() { return rx.p02.imei; }
  unsigned char  getNum() { return rx.buff[3]; }
  unsigned char  getLen() { return rx.buff[2]; }  
  
  private:
  struct P01 {
    long  begin;
    float lat;
    float lon;
    short alt;
    unsigned short vel;
    unsigned char  sat;
    unsigned char  hdop;
    unsigned char  fix;
    unsigned char  time;
    long  dist;
    unsigned char  bat;
    unsigned char  crc;
  };
  struct P02 {
    long  begin;
    char  imei[16];
    unsigned char  crc;
  };
  struct P03 {
    long  begin;
    float lat;
    float lon;
    unsigned short dist;
    unsigned short time;
    unsigned char  relay;
    unsigned char  crc;
  };  
  char   pos;
  bool   foundA5;
  bool   found5A;
  union {
    char buff[26];
    P01  p01;
    P02  p02;
    P03  p03;
  }rx;  
  union {
    char buff[26];
    P01  p01;
    P02  p02;
    P03  p03;
  }tx;
};
//-----------------------------------------------------------------------------------
#endif
