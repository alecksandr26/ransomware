#ifndef ENCIPHER_H
#define ENCIPHER_H

#include <stddef.h>

extern void encipher(const unsigned char *data, unsigned char *ciphered_data, size_t amount_bytes,
		     const char *str_key, size_t key_size);

extern void decipher(const unsigned char *ciphered_data, unsigned char *data, size_t amount_bytes,
		     const char *str_key, size_t key_size);

#endif
