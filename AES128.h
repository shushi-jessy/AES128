/*****************************************************************************************************
**  Filename:       AES128.h                                                                        **
**  Author:         Jessy                                                                           **
**  Last update:    2022-08-18  V1.0.0                                                              **
**  Description:    Advance Encryption Standard(AES) 128-bit Electrical Code Book(ECB) C library    **
*****************************************************************************************************/
#ifndef AES128_H
#define AES128_H

#include <stdint.h>

/* @ Brief
    * [DESCRIPTION]     This function encrypt input plaintext input cipher and the plaintext is padded.
    * [INPUT]           pointer to key string of 16-byte,
                        pointer to plaintext of n-byte(n<255)
    * [OUTPUT]          pointer to string of cipher, first byte of the string indicates cipher length
*/uint8_t* AES128_Encryption(uint8_t* key,uint8_t* plaintext,uint8_t plaintextext_len);


/* @ Brief
    * [DESCRIPTION]     This function decrypt input cipher input plaintext and the plaintext is unpadded.
                        Wrong padded cipher will cause wrong decrypting result.
    * [INPUT]           pointer to key string of 16-byte,
                        pointer to cipher of n-byte(n<255)
    * [OUTPUT]          pointer to string of plaintext, first byte of the string indicates plaintext length
*/uint8_t* AES128_Decryption(uint8_t* key,uint8_t* cipher,uint8_t cipher_len);


/* @ Brief
    * [DESCRIPTION]     This function is used to test "AES128_Encryption" and "AES128_Decryption" with
                        printed results. Plaintext and key can be modified in source file.
    * [INPUT]           none
    * [OUTPUT]          none
*/void AES128_Example(void);


#endif // AES128_H_INCLUDED
