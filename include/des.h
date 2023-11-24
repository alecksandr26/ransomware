#ifndef DES_H
#define DES_H

extern void des_encipher(const unsigned char *plaintext, unsigned char *ciphertext, unsigned char *key);
extern void des_decipher(const unsigned char *ciphertext, unsigned char *plaintext, unsigned char *key);

#endif


