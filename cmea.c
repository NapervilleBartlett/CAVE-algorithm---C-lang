#include "cave.h"
/* tbox has the same header as CAVE (see Exhibit 2-2) */

static unsigned char tbox(const unsigned char z)
{
   int k_index, i;
   unsigned char result;

   k_index = 0;
   result = z;

   for (i = 0; i < 4; i++)
   {
      result ^= cmeakey[k_index];
      result += cmeakey[k_index+1];
      result = z + CaveTable[result];
      k_index += 2;
   }

   return(result);
}

/* CMEA has the same header as CAVE (see Exhibit 2-2) */

void CMEA(unsigned char *msg_buf, const int byte_count)
{
   int msg_index, half;
   unsigned char k, z;

   /* first manipulation (inverse of third) */

   z = 0;
   for (msg_index = 0; msg_index < byte_count; msg_index++)
   {
      k = tbox(z ^ msg_index);
      msg_buf[msg_index] += k;
      z += msg_buf[msg_index];
   }

   /* second manipulation (self-inverse) */

   half = byte_count/2;
   for (msg_index = 0; msg_index < half; msg_index++)
   {
      msg_buf[msg_index] ^= msg_buf[byte_count - 1 - msg_index] | 0x01;
   }

   /* third manipulation (inverse of first) */
   z = 0;
   for (msg_index = 0; msg_index < byte_count; msg_index++)
   {
      k = tbox(z ^ msg_index);
      z += msg_buf[msg_index];
      msg_buf[msg_index] -= k;
   }
}
 
