#include "cave.h"
/* A_Key_Checksum has the same header as CAVE (see Exhibit 2-2) */

static void mul10(unsigned char i64[8],
                  unsigned int carry)
{
   int i;
   unsigned int temp;

   for (i = 7; i >= 0; i--)
   {
      temp = ((unsigned int)(i64[i]) * 10) + carry;
      i64[i] = temp & 0xFF;
      carry = temp >> 8;
   }
}

static unsigned long Calc_Checksum(const unsigned char A_key[8])
{
   int i, offset_1, offset_2;
   unsigned long A_key_checksum;

   /* see if 32 MSB are zero */
   if ((A_key[0] | A_key[1] | A_key[2] | A_key[3]) != 0)
   {
      /* put 32 MSB into LFSR */
      for (i = 0; i < 4; i++)
         LFSR[i] = A_key[i];
   }
   else
   {
      /* put ESN into LFSR */
      for (i = 0; i < 4; i++)
         LFSR[i] = ESN[i];
   }

   /* put A_key into r0-r7 */
   for (i = 0; i < 8; i++)
      Register[i] = A_key[i];

   Register[8] = AAV;

   /* put ls 24 bits of A_key into r9-r11 */
   for (i = 9; i < 12; i++)
      Register[i] = A_key[5+i-9];

   /* put ESN into r12-r15 */
   for (i = 12; i< 16; i++)
      Register[i] = ESN[i-12];

   offset_1 = offset_2 = 128;

   CAVE(8, &offset_1, &offset_2);

   A_key_checksum = 
            ( ((unsigned long)(Register[0] ^ Register[13]) << 16) +
              ((unsigned long)(Register[1] ^ Register[14]) << 8)  +
              ((unsigned long)(Register[2] ^ Register[15]))        )
            & 0x3ffff;

   return (A_key_checksum);
}
 
/* A_KEY_DIGITS contains the ASCII digits in the order to be entered */

void A_Key_Checksum(const char A_KEY_DIGITS[20],
                    char A_KEY_CHECKSUM[6])
{
   int i, offset_1, offset_2;
   unsigned char temp_A_key[8];
   unsigned long A_key_checksum;

   /* convert digits to 64-bit representation in temp_A_key */

   for (i = 0; i < 8; i++)
      temp_A_key[i] = 0;

   for (i = 0; i < 20; i++)
   {
      mul10(temp_A_key,(unsigned int)(A_KEY_DIGITS[i] - '0'));
   }

   A_key_checksum = Calc_Checksum(temp_A_key);

   /* convert checksum to decimal digits */
   for (i = 0; i < 6; i++)
   {
      A_KEY_CHECKSUM[5-i] = '0' + (A_key_checksum % 10);
      A_key_checksum /= 10;
   }
}

/* A_Key_Verify has the same header as CAVE (see Exhibit 2-2) */

/* A_KEY_DIGITS contains the ASCII digits in the order entered */

int A_Key_Verify(const char A_KEY_DIGITS[26])
{
   int i, offset_1, offset_2;
   unsigned char temp_A_key[8];
   unsigned long entered_checksum;

   /* convert first 20 digits to 64-bit representation in temp_A_key */

   for (i = 0; i < 8; i++)
      temp_A_key[i] = 0;

   for (i = 0; i < 20; i++)
   {
      mul10(temp_A_key,(unsigned int)(A_KEY_DIGITS[i] - '0'));
   }

   /* convert last 6 digits to entered checksum */

   entered_checksum = 0;
   for (i = 20; i < 26; i++)
   {
      entered_checksum = (entered_checksum * 10)
         + (A_KEY_DIGITS[i] - '0');
   }

   if (Calc_Checksum(temp_A_key) == entered_checksum)
   {
      for (i = 0; i < 8; i++)
      {
         A_key[i] = temp_A_key[i];
         SSD_A[i] = SSD_B[i] = 0;
      }
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

