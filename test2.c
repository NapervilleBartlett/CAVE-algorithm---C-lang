#include <stdio.h>
#include "cave.h"

unsigned char ESN[4] = { 0xd7, 0x5a, 0x96, 0xec };
unsigned char RANDSSD[7] = { 0x4d, 0x18, 0xee, 0xaa, 0x05, 0x89, 0x5c };
unsigned char MIN1[3] = {0x79, 0x29, 0x71};
unsigned char MIN2[2] = {0x02, 0x8d};
unsigned char cmeakey1[8] = {0xa0, 0x7b, 0x1c, 0xd1, 0x02, 0x75, 0x69, 0x14};
unsigned char cmeakey2[8] = {0xf0, 0x06, 0xa8, 0x5a, 0x05, 0xcd, 0xb3, 0x2a};
unsigned char RAND1[4] = {0x34, 0xa2, 0xb0, 0x5f};
unsigned char RAND2[4] = {0x53, 0x75, 0xdf, 0x99};

unsigned char buf[6] = {0xb6, 0x2d, 0xa2, 0x44, 0xfe, 0x9b};

char digits[27];

unsigned long AUTHR;

void SSD_Update(void)
{
   int i;

   for (i=0; i<8; i++)
   {
      SSD_A[i] = SSD_A_NEW[i];
      SSD_B[i] = SSD_B_NEW[i];
   }
}

void main(void)
{
   int i,j;
   unsigned char auth_data[3], test_buf[6];
   unsigned char a_key_check[7], a_key[21];

   strcpy(digits,"14142135623730950488086500");
   strcpy(a_key,"14142135623730950488");

   A_Key_Checksum(a_key, a_key_check);
   printf("A-Key check digits =");
   for (i=0; i<6; i++)
      printf(" %c", a_key_check[i]);
   printf("\n");

   /* check A_key and SSD */
   if (A_Key_Verify(digits))
      printf("A key verified ok\n");
   else
   {
      printf("A key verification failed\n");
      return;
   }

   /* check SSD generation process */
   SSD_Generation(RANDSSD);
   SSD_Update();

   printf("SSD_A =");
   for (i=0; i < 4; i++)
   {
      printf(" ");
      for (j = 0; j < 2; j++)
         printf("%02x",(unsigned int)SSD_A[2*i+j]);
   }
   printf("\n");

   printf("SSD_B =");
   for (i=0; i < 4; i++)
   {
      printf(" ");
      for (j = 0; j < 2; j++)
         printf("%02x",(unsigned int)SSD_B[2*i+j]);
   }
   printf("\n");

   /* put MIN1 into auth_data (no dialed digits) */
   for (i = 0; i<3; i++)
      auth_data[i] = MIN1[i];

   /* test vector 1 */
   printf("\nVector 1\n\n");
   AUTHR = Auth_Signature(RAND1, auth_data, SSD_A, 1);
   printf("RAND_CHALLENGE   =");
   for (i=0; i<2; i++)
   {
      printf(" ");
      for (j=0; j< 2; j++)
         printf("%02x",(unsigned int)RAND1[2*i+j]);
   }
   printf("\n");
   printf("AUTH_SIGNATURE   = %01lx %04lx\n", AUTHR>>16, AUTHR&0x0000ffff);
  
   for (i=0; i< 6; i++)
      test_buf[i] = buf[i];

   Key_VPM_Generation();
   CMEA(test_buf, 6);

   printf("CMEAkey k0,. .k7 =");
   for (i=0; i<8; i++)
      printf(" %02x",(unsigned int)cmeakey[i]);
   printf("\n");

   printf("CMEA output      =");
   for (i=0; i<6; i++)
      printf(" %02x",(unsigned int)test_buf[i]);
   printf("\n");

   CMEA(test_buf, 6);
   printf("CMEA output/input=");
   for (i=0; i<6; i++)
      printf(" %02x",(unsigned int)test_buf[i]);
   printf("\n");

   printf("VPM:");
   for (i=0; i<65; i++)
   {
      printf(" %02x",(unsigned int)VPM[i]);
      if (((i+1)%8) == 0)
         printf("\n    ");
   }
   printf("\n");

   
   /* test vector 2 */
   printf("\nVector 2\n\n");
   AUTHR = Auth_Signature(RAND2, auth_data, SSD_A, 1);
   printf("RAND_CHALLENGE   =");
   for (i=0; i<2; i++)
   {
      printf(" ");
      for (j=0; j< 2; j++)
         printf("%02x",(unsigned int)RAND2[2*i+j]);
   }
   printf("\n");
   printf("AUTH_SIGNATURE   = %01lx %04lx\n", AUTHR>>16, AUTHR&0x0000ffff);
  
   for (i=0; i< 6; i++)
      test_buf[i] = buf[i];

   Key_VPM_Generation();
   CMEA(test_buf, 6);

   printf("CMEAkey k0,. .k7 =");
   for (i=0; i<8; i++)
      printf(" %02x",(unsigned int)cmeakey[i]);
   printf("\n");

   printf("CMEA output      =");
   for (i=0; i<6; i++)
      printf(" %02x",(unsigned int)test_buf[i]);
   printf("\n");

   CMEA(test_buf, 6);
   printf("CMEA output/input=");
   for (i=0; i<6; i++)
      printf(" %02x",(unsigned int)test_buf[i]);
   printf("\n");

   printf("VPM:");
   for (i=0; i<65; i++)
   {
      printf(" %02x",(unsigned int)VPM[i]);
      if (((i+1)%8) == 0)
         printf("\n    ");
   }
   printf("\n");

}
   
