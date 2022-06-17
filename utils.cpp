#include "utils.h"
#include <stdio.h>
//-----------------------------------------------------------------------------------
unsigned char nmeaCheckSum(char* buff, int len) {
  unsigned char sum = 0;
  for(int i = 1; i < len; i++)
    sum ^= buff[i];
  return sum;    
}
//-----------------------------------------------------------------------------------
