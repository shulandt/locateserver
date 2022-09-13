#include "binr.h"
#include "utils.h"
#include <string.h>
//-----------------------------------------------------------------------------------
Binr::Binr() {
  pos = 0;
  foundA5 = false;
  found5A = false;
}
//-----------------------------------------------------------------------------------
char* Binr::getBuff01(float lat, float lon, short alt, unsigned short vel, unsigned char sat, 
                      unsigned char hdop, unsigned char fix, unsigned char time, 
                      long dist, unsigned char bat) {
  tx.p01.begin = 0x01165AA5;
  tx.p01.lat = lat;
  tx.p01.lon = lon;
  tx.p01.alt = alt;
  tx.p01.vel = vel;
  tx.p01.sat = sat;
  tx.p01.hdop = hdop;
  tx.p01.fix = fix;
  tx.p01.time = time;
  tx.p01.dist = dist;
  tx.p01.bat = bat;
  tx.p01.crc = crc8(tx.buff, 25);
  return tx.buff;                
}
//-----------------------------------------------------------------------------------
char* Binr::getBuff02(const char* imei) {
  tx.p02.begin = 0x02115AA5;
  strncpy(tx.p02.imei, imei, 15);
  tx.p02.imei[15] = 0;
  tx.p02.crc = crc8(tx.buff, 20);
  return tx.buff;                  
}
//-----------------------------------------------------------------------------------
char* Binr::getBuff03(float lat, float lon, unsigned short dist, unsigned short time, unsigned char relay) {
  tx.p03.begin = 0x030D5AA5;
  tx.p03.lat = lat;
  tx.p03.lon = lon;
  tx.p03.dist = dist;
  tx.p03.time = time;
  tx.p03.relay = relay;
  tx.p03.crc = crc8(tx.buff, 16);
  return tx.buff;                  
}
//-----------------------------------------------------------------------------------
bool Binr::decode(char inChar) {
  if(!foundA5) {
    if(inChar == 0xA5) {
      foundA5 = true;
      found5A = false;
      pos = 0;
    }   
  }
  else if(!found5A) {
    if(inChar == 0x5A)
      found5A = true;
    else
      foundA5 = false;    
  }

  if(foundA5) {
    if(pos > 2) {
      if(pos == 26) {
        foundA5 = false;
        return false;
      }
      if(pos == (rx.buff[2] + 3)) {
        foundA5 = false;
        if((unsigned char)inChar == crc8((unsigned char)rx.buff, pos))
          return true;
        else
          return false;          
      }
    }
    rx.buff[pos++] = inChar;
  }
  return false;
}
//-----------------------------------------------------------------------------------
