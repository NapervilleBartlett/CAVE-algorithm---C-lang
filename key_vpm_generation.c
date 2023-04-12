#include "cave.h"
/* Key_VPM_Generation has the same header as CAVE (see Exhibit 2-2) */

static void roll_LFSR(void)
{
   int i;

   LFSR_A = Register[0];
   LFSR_B = Register[1];
   LFSR_C = Register[14];
   LFSR_D = Register[15];

   if ((LFSR_A | LFSR_B | LFSR_C | LFSR_D) == 0)
   {
      for (i = 0; i < 4; i++)
         LFSR[i] = SAVED_RAND[i];
   }
}

void Key_VPM_Generation(void)
{
   int i, j, r_ptr, offset_1, offset_2, vpm_ptr;

   /* iteration 1, first pass through CAVE */

   for (i = 0; i < 4; i++)
      LFSR[i] = SAVED_LFSR[i] ^ SSD_B[i] ^ SSD_B[i+4];

   if ((LFSR_A | LFSR_B | LFSR_C | LFSR_D) == 0)
   {
      for (i = 0; i < 4; i++)
         LFSR[i] = SAVED_RAND[i];
   }

   for (i = 0; i < 8; i++)
      Register[i] = SSD_B[i];

   Register[8] = AAV;

   /* put SAVED_DATA into r9-r11 */
   for (i = 9; i < 12; i++)
      Register[i] = SAVED_DATA[i-9];

   /* put ESN into r12-r15 */
   for (i = 12; i < 16; i++)
      Register[i] = ESN[i-12];

   offset_1 = SAVED_OFFSET_1;
   offset_2 = SAVED_OFFSET_2;

   CAVE(8, &offset_1, &offset_2);

   /* iteration 2, generation of first CMEA key parameters */

   roll_LFSR();
   CAVE(4, &offset_1, &offset_2);
   for (i = 0; i < 4; i++)
      cmeakey[i] = Register[i+4] ^ Register[i+8];

   /* iteration 3, generation of second CMEA key parameters */

   roll_LFSR();
   CAVE(4, &offset_1, &offset_2);
   for (i = 4; i < 8; i++)
      cmeakey[i] = Register[i] ^ Register[i+4];

   /* iterations 4-13, generation of VPM */

   vpm_ptr = 0;
   for (i = 0; i < 10; i++)
   {
      roll_LFSR();
      CAVE(4, &offset_1, &offset_2);
      for (r_ptr = 0; r_ptr < 6; r_ptr++)
      {
         VPM[vpm_ptr] = Register[r_ptr+2] ^ Register[r_ptr+8];
         vpm_ptr++;
      }
   }

   /* iteration 14, generation of last VPM bits */

   roll_LFSR();
   CAVE(4, &offset_1, &offset_2);
   for (j = 0; j < 5; j++)
   {
      VPM[vpm_ptr] = Register[j+2] ^ Register[j+8];
      vpm_ptr++;
   }
}

