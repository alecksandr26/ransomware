#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include "../include/hash.h"

#define DBJ2_VAL 1844674407370955161

uint64_t hash_str_to_64bit(const char *str, size_t str_size)
{
	assert(str != NULL && str_size > 0 && "Can't receive null or zero size strings");
	
	uint64_t hash = DBJ2_VAL;

	/* DJB2 hash function style */
	for (size_t i = 0; i < str_size; i++)
		hash = ((hash << 5) + hash) ^ str[i];
	
	return hash;
	
}









