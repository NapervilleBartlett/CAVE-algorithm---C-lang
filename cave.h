/* header for CAVE and related procedures */

void CAVE(const int number_of_rounds,
          int *offset_1,
          int *offset_2);

void A_Key_Checksum(const char A_KEY_DIGITS[20],
                    char A_KEY_CHECKSUM[6]);

int A_Key_Verify(const char A_KEY_DIGITS[26]);

void SSD_Generation(const unsigned char RANDSSD[7]);

unsigned long Auth_Signature(const unsigned char RAND_CHALLENGE[4],
                             const unsigned char AUTH_DATA[3],
                             const unsigned char *SSD_AUTH,
                             const int SAVE_REGISTERS);

void Key_VPM_Generation(void);

#define LOMASK	0x0F
#define HIMASK	0xF0
#define TRUE	1
#define FALSE	0

/* authentication algorithm version (fixed) */

extern unsigned char	AAV;

/* external saved data */

extern unsigned char	ESN[4];

/* saved outputs */

extern unsigned char	LFSR[4];

#define LFSR_A	LFSR[0]
#define LFSR_B	LFSR[1]
#define LFSR_C	LFSR[2]
#define LFSR_D	LFSR[3]

extern unsigned char	Register[16];

extern unsigned char	A_key[8];
extern unsigned char	SSD_A_NEW[8], SSD_A[8];
extern unsigned char	SSD_B_NEW[8], SSD_B[8];
extern unsigned char	cmeakey[8];
extern unsigned char	VPM[65];

extern unsigned char	SAVED_LFSR[4];
extern int		SAVED_OFFSET_1;
extern int		SAVED_OFFSET_2;
extern unsigned char	SAVED_RAND[4];
extern unsigned char	SAVED_DATA[3];
extern unsigned char	SAVED_ESN[4];

extern unsigned char	CaveTable[256];

/* end of CAVE header */

