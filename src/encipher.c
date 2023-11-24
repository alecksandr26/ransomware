#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "../include/des.h"
#include "../include/hash.h"

void encipher(const unsigned char *data, unsigned char *ciphered_data, size_t amount_bytes,
	      const char *str_key, size_t key_size)
{
	assert(data != NULL && ciphered_data != NULL && amount_bytes > 0
	       && "Can't be null the data and the buffer where the ciphered data will be dump");
	assert(str_key != NULL && key_size > 0 && "The key can't be null");

	
	uint64_t hashed_key = hash_str_to_64bit(str_key, key_size);
	
	/* Iterate through chunks of 8 bytes */
	for (size_t i = 0; i < amount_bytes; i += 8)
		des_encipher((const unsigned char *) data + i, ciphered_data + i,
			     (unsigned char *) &hashed_key);

	/* The last bytes just copy the bytes */
	for (size_t i = amount_bytes - (amount_bytes % 8); i < amount_bytes; i++)
		ciphered_data[i] = data[i];

	
}

void decipher(const unsigned char *ciphered_data, unsigned char *data, size_t amount_bytes,
	      const char *str_key, size_t key_size)
{
	assert(data != NULL && ciphered_data != NULL && amount_bytes > 0
	       && "Can't be null the data and the buffer where the ciphered data will be dump");
	assert(str_key != NULL && key_size > 0 && "The key can't be null");
	

	uint64_t hashed_key = hash_str_to_64bit(str_key, key_size);

	/* Iterate through chunks of 8 bytes */
	for (size_t i = 0; i < amount_bytes; i += 8)
		des_decipher((const unsigned char *) ciphered_data + i, data + i,
			     (unsigned char *) &hashed_key);

	/* The last bytes just copy the bytes */
	for (size_t i = amount_bytes - (amount_bytes % 8); i < amount_bytes; i++)
		data[i] = ciphered_data[i];
}


