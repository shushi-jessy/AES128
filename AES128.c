/*****************************************************************************************************
**  Filename:       AES128.c                                                                        **
**  Author:         Jessy                                                                           **
**  Last update:    2022-08-18  V1.0.0                                                              **
**  Description:    Advance Encryption Standard(AES) 128-bit Electrical Code Book(ECB) C library    **
*****************************************************************************************************/

#include "AES128.h"

static void AddRoundKey(uint8_t state[4][4],uint8_t round);
static void SubBytes(uint8_t state[4][4]);
static void InvSubBytes(uint8_t state[4][4]);
static void ShiftRows(uint8_t state[4][4]);
static void InvShiftRows(uint8_t state[4][4]);
static void MixColumns(uint8_t state[4][4]);
static void InvMixColumns(uint8_t state[4][4]);
static void AES128_KeySchedule(uint8_t *key);
static uint8_t gmul(uint8_t a, uint8_t b);                   // Finite field multiplication

static uint8_t Round_Key[44][4] = {0};
/*
    Round Key 0:    Round_Key[0][0~3]   Round_Key[1][0~3]   Round_Key[2][0~3]   Round_Key[3][0~3]      = > Used defined key itself (1-double-word length)
    Round Key 1:    Round_Key[4][0~3]   Round_Key[5][0~3]   Round_Key[6][0~3]   Round_Key[7][0~3]
    Round Key 2:    Round_Key[8][0~3]   Round_Key[9][0~3]   Round_Key[10][0~3]  Round_Key[11][0~3]
    Round Key 3:    Round_Key[12][0~3]  Round_Key[13][0~3]  Round_Key[14][0~3]  Round_Key[15][0~3]
    Round Key 4:    Round_Key[16][0~3]  Round_Key[17][0~3]  Round_Key[18][0~3]  Round_Key[19][0~3]
    Round Key 5:    Round_Key[20][0~3]  Round_Key[21][0~3]  Round_Key[22][0~3]  Round_Key[23][0~3]
    Round Key 6:    Round_Key[24][0~3]  Round_Key[25][0~3]  Round_Key[26][0~3]  Round_Key[27][0~3]
    Round Key 7:    Round_Key[28][0~3]  Round_Key[29][0~3]  Round_Key[30][0~3]  Round_Key[31][0~3]
    Round Key 8:    Round_Key[32][0~3]  Round_Key[33][0~3]  Round_Key[34][0~3]  Round_Key[35][0~3]
    Round Key 9:    Round_Key[36][0~3]  Round_Key[37][0~3]  Round_Key[38][0~3]  Round_Key[39][0~3]
    Round Key 10:   Round_Key[40][0~3]  Round_Key[41][0~3]  Round_Key[42][0~3]  Round_Key[43][0~3]
*/

static uint8_t RCON[255] =
{
0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d,
0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab,
0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d,
0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25,
0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01,
0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d,
0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa,
0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a,
0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02,
0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f,
0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5,
0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33,
0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb
};
static uint8_t SBOX[256] =
{
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};
static uint8_t SBOXinv[256] =
{
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};
static uint8_t MIXER[4][4] =
{
    {0x02,0x01,0x01,0x03},
    {0x03,0x02,0x01,0x01},
    {0x01,0x03,0x02,0x01},
    {0x01,0x01,0x03,0x02}
};
static uint8_t MIXERinv[4][4] =
{
    {0x0E,0x09,0x0D,0x0B},
    {0x0B,0x0E,0x09,0x0D},
    {0x0D,0x0B,0x0E,0x09},
    {0x09,0x0D,0x0B,0x0E}
};
static void AddRoundKey(uint8_t state[4][4],uint8_t round)
{
    for(uint8_t i=0;i<4;i++)
        for(uint8_t j=0;j<4;j++)
            state[i][j]^=Round_Key[(round<<2)+i][j];
}
static void SubBytes(uint8_t state[4][4])
{
    for(uint8_t i=0;i<4;i++)
        for(uint8_t j=0;j<4;j++)
            state[i][j]=SBOX[state[i][j]];
}
static void InvSubBytes(uint8_t state[4][4])
{
    for(uint8_t i=0;i<4;i++)
        for(uint8_t j=0;j<4;j++)
            state[i][j]=SBOXinv[state[i][j]];
}
static void ShiftRows(uint8_t state[4][4])
{
    uint8_t temp[4][4]=
    {
        {state[0][0], state[1][1], state[2][2], state[3][3]},
        {state[1][0], state[2][1], state[3][2], state[0][3]},
        {state[2][0], state[3][1], state[0][2], state[1][3]},
        {state[3][0], state[0][1], state[1][2], state[2][3]},
    };
    for(uint8_t i=0;i<4;i++)
        for(uint8_t j=0;j<4;j++)
            state[i][j]=temp[i][j];
}
static void InvShiftRows(uint8_t state[4][4])
{
    uint8_t temp[4][4]=
    {
      {state[0][0], state[3][1], state[2][2], state[1][3]},
      {state[1][0], state[0][1], state[3][2], state[2][3]},
      {state[2][0], state[1][1], state[0][2], state[3][3]},
      {state[3][0], state[2][1], state[1][2], state[0][3]},
    };
    for(uint8_t i=0;i<4;i++)
        for(uint8_t j=0;j<4;j++)
            state[i][j]=temp[i][j];
}
static void InvMixColumns(uint8_t state[4][4])
{
    uint8_t temp[4][4]={0};
    for(uint8_t i=0;i<4;i++)
        for(uint8_t j=0;j<4;j++)
        {
           for(uint8_t k=0;k<4;k++)
                temp[i][j]^=gmul(MIXERinv[k][j],state[i][k]);
        }
     for(uint8_t i=0;i<4;i++)
        for(uint8_t j=0;j<4;j++)state[i][j]=temp[i][j];
}
static void MixColumns(uint8_t state[4][4])
{
    uint8_t temp[4][4]={0};
    for(uint8_t i=0;i<4;i++)
        for(uint8_t j=0;j<4;j++)
        {
           for(uint8_t k=0;k<4;k++)
                temp[i][j]^=gmul(MIXER[k][j],state[i][k]);
        }
     for(uint8_t i=0;i<4;i++)
        for(uint8_t j=0;j<4;j++)state[i][j]=temp[i][j];
}
static void AES128_KeySchedule(uint8_t *key)
{
        // First round key is the input key itself
        for(uint8_t i=0;i<4;i++)for(uint8_t j=0;j<4;j++)Round_Key[i][j] = key[(i<<2)+j];

        uint8_t temp[4] = {0};      // A double word temporary variable
        uint8_t iteration = 1;      // RCON iteration, must start from 1
        for(uint8_t i=4;i<48;i=i+4)
        {// Key expansion: each round key has 4 doublewords: 4*10=40 in total,but the root key(1-round key)does not change, thus starts from 4th(count from 0) word and finish at 40+4=44 words

            // Step 1: Rotate the previous doubleword
            temp[0] = Round_Key[i-1][1];
            temp[1] = Round_Key[i-1][2];
            temp[2] = Round_Key[i-1][3];
            temp[3] = Round_Key[i-1][0];

            // Step 2: Refer to S-Box table
            temp[0]=SBOX[temp[0]];
            temp[1]=SBOX[temp[1]];
            temp[2]=SBOX[temp[2]];
            temp[3]=SBOX[temp[3]];

            // Step 3: Refer to RCON table for each first byte of a doubleword
            temp[0]^= RCON[iteration++];

            // Step 4: XOR the (n-4)(n-3)(n-2)(n-1) double word
            for(uint8_t j=0;j<4;j++)
            {
                temp[0] ^= Round_Key[i-4+j][0];
                temp[1] ^= Round_Key[i-4+j][1];
                temp[2] ^= Round_Key[i-4+j][2];
                temp[3] ^= Round_Key[i-4+j][3];
                Round_Key[i+j][0]= temp[0];
                Round_Key[i+j][1]= temp[1];
                Round_Key[i+j][2]= temp[2];
                Round_Key[i+j][3]= temp[3];
            }
        }
}
static uint8_t gmul(uint8_t a, uint8_t b) // Referring to: https://en.wikipedia.org/wiki/Finite_field_arithmetic
{
	/* Multiply two numbers in the GF(2^8) finite field defined
    * by the modulo polynomial relation x^8 + x^4 + x^3 + x + 1 = 0
    * (the other way being to do carryless multiplication followed by a modular reduction)
    */
	uint8_t p = 0; /* accumulator for the product of the multiplication */
	while (a != 0 && b != 0)
    {
        if (b & 1) /* if the polynomial for b has a constant term, add the corresponding a to p */
            p ^= a; /* addition in GF(2^m) is an XOR of the polynomial coefficients */

        if (a & 0x80) /* GF modulo: if a has a nonzero term x^7, then must be reduced when it becomes x^8 */
            a = (a << 1) ^ 0x11b; /* subtract (XOR) the primitive polynomial x^8 + x^4 + x^3 + x + 1 (0b1_0001_1011) � you can change it but it must be irreducible */
        else
            a <<= 1; /* equivalent to a*x */
        b >>= 1;
	}
	return p;
}
uint8_t* AES128_Encryption(uint8_t* key,uint8_t* plaintext,uint8_t plaintextext_len)
{
    // Generate round keys:
    AES128_KeySchedule(key);

    // Plaintext padding:
    uint8_t  npaddings = 16-plaintextext_len%16;

    // Get cipher size: (This is equal to padded plaintext size)
    uint8_t  cipher_len = plaintextext_len + npaddings;

    // Get block number:
    uint8_t  nblocks = cipher_len>>4;

    // Allocate memory to padded plaintext and cipher:
    uint8_t* newplaintext = malloc(cipher_len);
    uint8_t* cipher       = malloc(cipher_len+1);   // one more byte to store cipher size, first byte thus cipher[0]

    // Padded plaintext:
    for(uint8_t i=0;i<plaintextext_len;i++)    newplaintext[i]=plaintext[i];
    for(uint8_t i=0;i<npaddings;i++)           newplaintext[i+plaintextext_len]=npaddings;

    // Loop for the encryption of each block:
    for(uint8_t nb=0;nb<nblocks;nb++)
    {
        uint8_t temp[4][4]={0};

        for(uint8_t i=0;i<4;i++)for(uint8_t j=0;j<4;j++)temp[i][j]=newplaintext[(nb<<4)+(i<<2)+j];

        AddRoundKey(temp,0);

        for(uint8_t round=1;round<=10;round++)
        {
            // Step 1:
            SubBytes(temp);

            // Step 2:
            ShiftRows(temp);

            // Step 3: (Last round(10) does not execute this function)
            if(round<10) MixColumns(temp);

            // Step 4:
            AddRoundKey(temp,round);
        }

        // Update block cipher to entire cipher string
        for(uint8_t i=0;i<4;i++)for(uint8_t j=0;j<4;j++)cipher[(nb<<4)+(i<<2)+j+1]=temp[i][j];
    }
    // Set cipher length as first data byte
    cipher[0]=cipher_len;

    // free unused allocated memories:
    free(newplaintext);
    /* CANNOT free cipher */

    return cipher;
}
uint8_t* AES128_Decryption(uint8_t* key,uint8_t* cipher,uint8_t cipher_len)
{
    // Generate round keys:
    AES128_KeySchedule(key);

    // Get block number:
    uint8_t  nblocks = cipher_len>>4;

    // Allocate memory to padded plaintext and cipher:
    uint8_t* plaintext    = malloc(cipher_len+1);   // one more byte to store plaintext size, first byte thus plaintext[0]

    for(uint8_t nb=0;nb<nblocks;nb++)
    {
        uint8_t temp[4][4]={0};

        for(uint8_t i=0;i<4;i++)for(uint8_t j=0;j<4;j++)temp[i][j]=cipher[(nb<<4)+(i<<2)+j];

         AddRoundKey(temp,10);

        for(int round=9;round>=0;round--)
        {
            // Step 1:
            InvShiftRows(temp);

            // Step 2:
            InvSubBytes(temp);

            // Step 3:
            AddRoundKey(temp,round);

            // Step 4: (Last round(0) does not execute this function)
            if(round>0)InvMixColumns(temp);
        }
        // Update block plaintext to entire plaintext string
        for(uint8_t i=0;i<4;i++)for(uint8_t j=0;j<4;j++)plaintext[(nb<<4)+(i<<2)+j+1]=temp[i][j];
    }

    // Remove padding by setting plaintext length as first data byte(last value of plaintext refers to padded value before unpadded)
    plaintext[0]=cipher_len-plaintext[cipher_len];

    // free unused allocated memories:
    /* CANNOT free plaintext */

    return plaintext;
}
void AES128_Example(void)
{
    // Create the plaintext and key:
    uint8_t* Plaintext     = "This is a string of 29 bytes!";   // The plaintext length can be from 1 to 255 bytes
    uint8_t* Key           = "Key of 16 bytes!";                // The key length MUST be fixed 16 bytes

    // Get plaintext length:
    uint8_t  Plaintext_len = 0;for(uint8_t i=0;Plaintext[i]!=0;i++)Plaintext_len++;

    // Print plaintext(unpadded):
    printf("Plaintext(char):\t");for(uint8_t i=0;i<Plaintext_len;i++)printf(" %c  ",Plaintext[i]);printf("\n");
    printf("Plaintext(hex): \t");for(uint8_t i=0;i<Plaintext_len;i++)printf("%02x  ",Plaintext[i]);printf("\n");
    printf("-----------------------------------------------------------------------------------------------\n");

    // Print key:
    printf("Key(char):      \t");for(uint8_t i=0;i<16;i++)printf(" %c  ",Key[i]);printf("\n");
    printf("Key(hex):       \t");for(uint8_t i=0;i<16;i++)printf("%02x  ",Key[i]);printf("\n");
    printf("-----------------------------------------------------------------------------------------------\n");

    // Encode:
    uint8_t* cipher = AES128_Encryption(Key,Plaintext,Plaintext_len);

    // Print cipher:
    printf("Cipher(char):   \t");for(uint8_t i=1;i<=cipher[0];i++)printf(" %c  ",cipher[i]);printf("\n");
    printf("Cipher(hex):    \t");for(uint8_t i=1;i<=cipher[0];i++)printf("%02x  ",cipher[i]);printf("\n");
    printf("-----------------------------------------------------------------------------------------------\n");

    // Decode:
    uint8_t* plaintext = AES128_Decryption(Key,++cipher,cipher[0]);

    // Print plaintext(unpadded):
    printf("plaintext(char):\t");for(uint8_t i=1;i<=plaintext[0];i++)printf(" %c  ",plaintext[i]);printf("\n");
    printf("plaintext(hex): \t");for(uint8_t i=1;i<=plaintext[0];i++)printf("%02x  ",plaintext[i]);printf("\n");
    printf("-----------------------------------------------------------------------------------------------\n");
}
