#include <stdio.h>
#include <stdlib.h>
#include "ASE128.h"

uint8_t data[16]    = {0x54,0x77,0x6F,0x20,0x4F,0x6E,0x65,0x20,0x43,0x69,0x6E,0x65,0x20,0x54,0x77,0x6F};
uint8_t key[16]     = {0x54,0x68,0x61,0x74,0x73,0x20,0x6D,0x79,0x20,0x4B,0x75,0x6E,0x67,0x20,0x46,0x75};

int main()
{

    uint8_t cryto[16]={0};
    ASE128_Enc(data,key);
    //printf("\nData:\t");  for(uint8_t i=0;i<16;i++)printf("0x%x ",data[i]);
    //printf("\nKey:\t");   for(uint8_t i=0;i<16;i++)printf("0x%x ",key[i]);
    //printf("\ncryto:\t"); for(uint8_t i=0;i<16;i++)printf("0x%x ",cryto[i]);
    while(1)
    {


    }






    return 0;
}
