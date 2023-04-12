#include "cave.h"

/* global definitions */
unsigned char	AAV = { 0xc7 };
unsigned char	LFSR[4];
unsigned char	Register[16];

unsigned char	A_key[8];
unsigned char	SSD_A_NEW[8], SSD_A[8];
unsigned char	SSD_B_NEW[8], SSD_B[8];
unsigned char	cmeakey[8];
unsigned char	VPM[65];

unsigned char	SAVED_LFSR[4];
int		SAVED_OFFSET_1;
int		SAVED_OFFSET_2;
unsigned char	SAVED_RAND[4];
unsigned char	SAVED_DATA[3];
unsigned char	SAVED_ESN[4];


/* table 0 is the 4 lsbs of the array,
   table 1 is the 4 msbs of the array */

unsigned char	CaveTable[256] =

   {    0xd9, 0x23, 0x5f, 0xe6, 0xca, 0x68, 0x97, 0xb0,
        0x7b, 0xf2, 0x0c, 0x34, 0x11, 0xa5, 0x8d, 0x4e,
        0x0a, 0x46, 0x77, 0x8d, 0x10, 0x9f, 0x5e, 0x62,
        0xf1, 0x34, 0xec, 0xa5, 0xc9, 0xb3, 0xd8, 0x2b,
        0x59, 0x47, 0xe3, 0xd2, 0xff, 0xae, 0x64, 0xca,
        0x15, 0x8b, 0x7d, 0x38, 0x21, 0xbc, 0x96, 0x00,
        0x49, 0x56, 0x23, 0x15, 0x97, 0xe4, 0xcb, 0x6f,
        0xf2, 0x70, 0x3c, 0x88, 0xba, 0xd1, 0x0d, 0xae,
        0xe2, 0x38, 0xba, 0x44, 0x9f, 0x83, 0x5d, 0x1c,
        0xde, 0xab, 0xc7, 0x65, 0xf1, 0x76, 0x09, 0x20,
        0x86, 0xbd, 0x0a, 0xf1, 0x3c, 0xa7, 0x29, 0x93,
        0xcb, 0x45, 0x5f, 0xe8, 0x10, 0x74, 0x62, 0xde,
        0xb8, 0x77, 0x80, 0xd1, 0x12, 0x26, 0xac, 0x6d,
        0xe9, 0xcf, 0xf3, 0x54, 0x3a, 0x0b, 0x95, 0x4e,
        0xb1, 0x30, 0xa4, 0x96, 0xf8, 0x57, 0x49, 0x8e,
        0x05, 0x1f, 0x62, 0x7c, 0xc3, 0x2b, 0xda, 0xed,
        0xbb, 0x86, 0x0d, 0x7a, 0x97, 0x13, 0x6c, 0x4e,
        0x51, 0x30, 0xe5, 0xf2, 0x2f, 0xd8, 0xc4, 0xa9,
        0x91, 0x76, 0xf0, 0x17, 0x43, 0x38, 0x29, 0x84,
        0xa2, 0xdb, 0xef, 0x65, 0x5e, 0xca, 0x0d, 0xbc,
        0xe7, 0xfa, 0xd8, 0x81, 0x6f, 0x00, 0x14, 0x42,
        0x25, 0x7c, 0x5d, 0xc9, 0x9e, 0xb6, 0x33, 0xab,
        0x5a, 0x6f, 0x9b, 0xd9, 0xfe, 0x71, 0x44, 0xc5,
        0x37, 0xa2, 0x88, 0x2d, 0x00, 0xb6, 0x13, 0xec,
        0x4e, 0x96, 0xa8, 0x5a, 0xb5, 0xd7, 0xc3, 0x8d,
        0x3f, 0xf2, 0xec, 0x04, 0x60, 0x71, 0x1b, 0x29,
        0x04, 0x79, 0xe3, 0xc7, 0x1b, 0x66, 0x81, 0x4a,
        0x25, 0x9d, 0xdc, 0x5f, 0x3e, 0xb0, 0xf8, 0xa2,
        0x91, 0x34, 0xf6, 0x5c, 0x67, 0x89, 0x73, 0x05,
        0x22, 0xaa, 0xcb, 0xee, 0xbf, 0x18, 0xd0, 0x4d,
        0xf5, 0x36, 0xae, 0x01, 0x2f, 0x94, 0xc3, 0x49,
        0x8b, 0xbd, 0x58, 0x12, 0xe0, 0x77, 0x6c, 0xda   };


static unsigned char bit_val(const unsigned char byte, const int bit)
{
   return((byte << (7 - bit)) & 0x80);
}

static void LFSR_cycle(void)
{
   unsigned char temp;
   int i;

   temp  = bit_val(LFSR_B,6);
   temp ^= bit_val(LFSR_D,2);
   temp ^= bit_val(LFSR_D,1);
   temp ^= bit_val(LFSR_D,0);

   /* Shift right LFSR, Discard LFSR_D[0] bit */

   for (i = 3; i > 0; i--)
   {
      LFSR[i] >>= 1;
      if (LFSR[i-1] & 0x01)
         LFSR[i] |= 0x80;
   }
   LFSR[0] >>= 1;

   LFSR_A |= temp;
}

static void Rotate_right_registers(void)
{
   unsigned int temp_reg;
   int i;

   temp_reg = Register[15]; /* save lsb */

   for (i = 15; i > 0; i--)
   {
      Register[i] >>= 1;
      if (Register[i-1] & 0x01)
         Register[i] |= 0x80;
   }

   Register[0] >>= 1;
   if (temp_reg & 0x01)
      Register[0] |= 0x80;
}

void CAVE(const int number_of_rounds, int *offset_1, int *offset_2)
{
   unsigned char temp_reg0;
   unsigned char lowNibble;
   unsigned char hiNibble;
   unsigned char temp;
   int round_index;
   int R_index;
   int fail_count;
   unsigned char T[16];

   for (round_index = number_of_rounds - 1;
        round_index >= 0;
        round_index --)
   {
      /* save R0 for reuse later */
      temp_reg0 = Register[0];

      for (R_index = 0; R_index < 16; R_index++)
      {
         fail_count = 0;
         while(1)
         {
            *offset_1 += (LFSR_A ^ Register[R_index]);
            /* will overflow; mask to prevent */
            *offset_1 &= 0xff;
            lowNibble = CaveTable[*offset_1] & LOMASK;
            if (lowNibble == (Register[R_index] & LOMASK))
            {
               LFSR_cycle();
               fail_count++;
               if (fail_count == 32)
               {
                  LFSR_D++; /* no carry to LFSR_C */
                  break;
               }
            }
            else break;
         }

         fail_count = 0;
         while(1)
         {
            *offset_2 += (LFSR_B ^ Register[R_index]);
            /* will overflow; mask to prevent */
            *offset_2 &= 0xff;
            hiNibble = CaveTable[*offset_2] & HIMASK;
            if (hiNibble == (Register[R_index] & HIMASK))
            {
               LFSR_cycle();
               fail_count++;
               if (fail_count == 32)
               {
                  LFSR_D++; /* no carry to LFSR_C */
                  break;
               }
            }
            else break;
         }

         temp = lowNibble | hiNibble;
         if (R_index == 15)
            Register[R_index] = temp_reg0 ^ temp;
         else
            Register[R_index] = Register[R_index+1] ^ temp;

         LFSR_cycle();
      }

      Rotate_right_registers();

      /* shuffle the mixing registers */
      for (R_index = 0; R_index < 16; R_index++)
      {
         temp = CaveTable[16*round_index + R_index] & LOMASK;
         T[temp] = Register[R_index];
      }
      for (R_index = 0; R_index < 16; R_index++)
         Register[R_index] = T[R_index];
   }
}

