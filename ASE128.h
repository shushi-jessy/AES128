#ifndef ASE128_H_INCLUDED
#define ASE128_H_INCLUDED

#include <stdint.h>
#include <assert.h>

void ASE128_Enc(uint8_t *data,uint8_t* key);
void ASE128_Dec(uint8_t *data,uint8_t* key);






#endif // ASE128_H_INCLUDED
