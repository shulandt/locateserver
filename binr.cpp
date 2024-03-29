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
char* Binr::getBuff01(float lat, float lon, int16_t alt, uint16_t vel, uint8_t sat, 
                      uint8_t hdop, uint8_t fix, uint8_t time, int32_t dist, uint8_t bat) {
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
  tx.p01.crc = crc8((unsigned char*)tx.buff, 25);
  return tx.buff;                
}
//-----------------------------------------------------------------------------------
char* Binr::getBuff02(const char* imei) {
  tx.p02.begin = 0x02115AA5;
  strncpy(tx.p02.imei, imei, 15);
  tx.p02.imei[15] = 0;
  tx.p02.crc = crc8((unsigned char*)tx.buff, 20);
  return tx.buff;                  
}
//-----------------------------------------------------------------------------------
char* Binr::getBuff03(float lat, float lon, uint16_t dist, uint16_t time, uint8_t relay) {
  tx.p03.begin = 0x030E5AA5;
  tx.p03.lat = lat;
  tx.p03.lon = lon;
  tx.p03.dist = dist;
  tx.p03.time = time;
  tx.p03.relay = relay;
  tx.p03.crc = crc8((unsigned char*)tx.buff, 17);
  return tx.buff;                  
}
//-----------------------------------------------------------------------------------
bool Binr::decode(char inChar) {
  if(!foundA5) {
    if(inChar == (char)0xA5) {
      foundA5 = true;
      found5A = false;
      pos = 0;
    }   
  }
  else if(!found5A) {
    if(inChar == (char)0x5A)
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
        if((unsigned char)inChar == crc8((unsigned char*)rx.buff, pos))
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
