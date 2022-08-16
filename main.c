#include <stdio.h>
#include <stdlib.h>
#include "ASE128.h"

uint8_t data[16]    = "Hello from Jessy";
uint8_t key[16]     = "Hello Polytech !";

int main()
{

    uint8_t cryto[16]={0};

    printf(" Data:\t");for(uint8_t i=0;i<16;i++)printf("0x%02x ",data[i]);printf("\n");
    printf("       ");for(uint8_t i=0;i<16;i++)printf("  %c  ",data[i]);printf("\n");
    printf("------------------------------------------------------------------------------------------\n");
    printf(" Key:\t");for(uint8_t i=0;i<16;i++)printf("0x%02x ",key[i]);printf("\n");
    printf("       ");for(uint8_t i=0;i<16;i++)printf("  %c  ",key[i]);printf("\n");
    printf("------------------------------------------------------------------------------------------\n");
    //printf("         [------w0------]    [------w1------]    [------w2------]    [------w3------]\n");
    //printf("-----------------------------------------------------------\n");
    //ASE128_Enc(data,key);


    // Result:
    uint8_t cipher[]     = {0xdb, 0x40, 0xf2, 0x9C,
                            0xf9, 0x67, 0xbc, 0x4b,
                            0x33, 0x47, 0x31, 0xe1,
                            0xb9, 0x99, 0xf6, 0x29};


    ASE128_Dec(cipher,key);
    //printf("\nData:\t");  for(uint8_t i=0;i<16;i++)printf("0x%x ",data[i]);
    //printf("\nKey:\t");   for(uint8_t i=0;i<16;i++)printf("0x%x ",key[i]);
    //printf("\ncryto:\t"); for(uint8_t i=0;i<16;i++)printf("0x%x ",cryto[i]);
    while(1)
    {


    }






    return 0;
}
