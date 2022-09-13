#ifndef binrH
#define binrH
//-----------------------------------------------------------------------------------
#include <stdint.h>
//-----------------------------------------------------------------------------------
class Binr {
  public:
  static const int rxBuffSize = 26;
  Binr();
  bool  decode(char inChar);
  char* getBuff01(float lat, float lon, int16_t alt, uint16_t vel, uint8_t sat, 
                  uint8_t hdop, uint8_t fix, uint8_t time, int32_t dist, uint8_t bat);
  char* getBuff02(const char* imei);
  char* getBuff03(float lat, float lon, uint16_t dist, uint16_t time, uint8_t relay);
  float getP03Lat() { return rx.p03.lat; }
  float getP03Lon() { return rx.p03.lon; }
  uint16_t getP03Dist() { return rx.p03.dist; }
  uint16_t getP03Time() { return rx.p03.time; }
  uint8_t  getP03Relay() { return rx.p03.relay; }
  float getP01Lat() { return rx.p01.lat; }
  float getP01Lon() { return rx.p01.lon; }
  int16_t getP01Alt() { return rx.p01.alt; }
  uint16_t getP01Vel() { return rx.p01.vel; }
  uint8_t  getP01Sat() { return rx.p01.sat; }
  uint8_t  getP01Hdop() { return rx.p01.hdop; }
  uint8_t  getP01Fix() { return rx.p01.fix; }
  uint8_t  getP01Time() { return rx.p01.time; }
  int32_t  getP01Dist() { return rx.p01.dist; }
  uint8_t  getP01Bat() { return rx.p01.bat; }
  char* getP02imei() { return rx.p02.imei; }
  uint8_t  getNum() { return rx.buff[3]; }
  uint8_t  getLen() { return rx.buff[2]; }  
  
  private:
  struct P01 {
    uint32_t begin;
    float    lat;
    float    lon;
    int16_t  alt;
    uint16_t vel;
    uint8_t  sat;
    uint8_t  hdop;
    uint8_t  fix;
    uint8_t  time;
    int32_t  dist;
    uint8_t  bat;
    uint8_t  crc;
  };
  struct P02 {
    uint32_t begin;
    char     imei[16];
    uint8_t  crc;
  };
  struct P03 {
    uint32_t begin;
    float    lat;
    float    lon;
    uint16_t dist;
    uint16_t time;
    uint8_t  relay;
    uint8_t  crc;
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
