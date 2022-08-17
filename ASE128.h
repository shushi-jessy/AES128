#ifndef ASE128_H_INCLUDED
#define ASE128_H_INCLUDED

#include <stdint.h>
#include <assert.h>

void ASE128_BlockEncryption(uint8_t plaintext[4][4],uint8_t key[4][4],uint8_t cipher[4][4]);
void ASE128_BlockDecryption(uint8_t cipher[4][4],uint8_t key[4][4],uint8_t plaintext[4][4]);

void Padding_Encryption(uint8_t* pleintext,uint8_t length);




#endif // ASE128_H_INCLUDED
