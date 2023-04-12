#include "cave.h"
/* SSD_Generation has the same header as CAVE (see exhibit 2-2) */

void SSD_Generation(const unsigned char RANDSSD[7])
{
   int i, offset_1, offset_2;

   for (i = 0; i < 4; i++)
   {
      LFSR[i] = RANDSSD[i+3] ^ A_key[i] ^ A_key[i+4];
   }

   if ((LFSR[0] | LFSR[1] | LFSR[2] | LFSR[3]) == 0)
   {
      for (i = 0; i < 4; i++)
         LFSR[0] = RANDSSD[i+3];
   }

   for (i = 0; i < 8; i++)
      Register[i] = A_key[i];

   Register[8] = AAV;

   for (i = 9; i < 12; i++)
      Register[i] = RANDSSD[i-9];

   for (i = 12; i < 16; i++)
      Register[i] = ESN[i-12];

   offset_1 = offset_2 = 128;
   CAVE(8, &offset_1, &offset_2);

   for (i = 0; i < 8; i++)
   {
      SSD_A_NEW[i] = Register[i];
      SSD_B_NEW[i] = Register[i+8];
   }
}

