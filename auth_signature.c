#include "cave.h"
/* Auth_Signature has the same header as CAVE (see Exhibit 2-2) */

unsigned long Auth_Signature(const unsigned char RAND_CHALLENGE[4],
                             const unsigned char AUTH_DATA[3],
                             const unsigned char *SSD_AUTH,
                             const int SAVE_REGISTERS)
{
   int i, offset_1, offset_2;
   unsigned long AUTH_SIGNATURE;

   for (i = 0; i < 4; i++)
   {
      LFSR[i] = RAND_CHALLENGE[i] ^ SSD_AUTH[i] ^ SSD_AUTH[i+4];
   }

   if ((LFSR_A | LFSR_B | LFSR_C | LFSR_D) == 0)
   {
      for (i = 0; i < 4; i++)
         LFSR[i] = RAND_CHALLENGE[i];
   }

   /* put SSD_AUTH into r0-r7 */
   for (i = 0; i < 8; i++)
      Register[i] = SSD_AUTH[i];

   Register[8] = AAV;

   /* put AUTH_DATA into r9-r11 */
   for (i = 9; i < 12; i++)
      Register[i] = AUTH_DATA[i-9];

   /* put ESN into r12-r15 */
   for (i = 12; i < 16; i++)
      Register[i] = ESN[i-12];

   offset_1 = offset_2 = 128;
   CAVE(8, &offset_1, &offset_2);

   AUTH_SIGNATURE =
      ( ((unsigned long)(Register[0] ^ Register[13]) << 16) +
        ((unsigned long)(Register[1] ^ Register[14]) << 8)  +
        ((unsigned long)(Register[2] ^ Register[15]) )        )
      & 0x3ffff;

   if (SAVE_REGISTERS)
   {
      /* save LFSR and offsets */

      SAVED_OFFSET_1 = offset_1;
      SAVED_OFFSET_2 = offset_2;
      for (i = 0; i < 4; i++)
      {
         SAVED_LFSR[i] = LFSR[i];
         SAVED_RAND[i] = RAND_CHALLENGE[i];
         SAVED_ESN[i] = ESN[i];
         if (i < 3)
         {
            SAVED_DATA[i] = AUTH_DATA[i];
         }
      }
   }

   return(AUTH_SIGNATURE);
}

