#include "../include/bit.h"

int bit_get(const unsigned char *bits, int pos)
{
	unsigned char mask;
	int i;

	/* Set the mask */
	mask = 0x80; /* 128 in decimal */

	for (i = 0; i < (pos % 8); i++)
		mask >>= 1;


	/* Get the bit */
	return (((mask & bits[(int) (pos / 8)]) == mask) ? 1 : 0);
}

void bit_set(unsigned char *bits, int pos, int state)
{
	unsigned char mask;
	int i;

	/* Set the mask */
	mask = 0x80; /* 128 in decimal */
    
	for (i = 0; i < (pos % 8); i++)
		mask >>= 1;

	/* Set the bit */
	if (state)
		bits[pos / 8] |= mask;
	else
		bits[pos / 8] &= (~mask);
}

void bit_xor(const unsigned char *bits1, const unsigned char *bits2,
             unsigned char *bitsx, int size)
{
	int i;

	/* Computed bitwise XOR (exclusive OR) of the two buffers */
	for (i = 0; i < size; i++) {
		if (bit_get(bits1, i) != bit_get(bits2, i))
			bit_set(bitsx, i, 1);
		else
			bit_set(bitsx, i, 0);
	}
}

void bit_rot_left(unsigned char *bits, int size, int count)
{
	int fbit, lbit, i, j;
	
	/* Rotate the buffer to the left the specified number of bits */
	if (size > 0) {
		for (j = 0; j < count; j++) {
			for (i = 0; i <= ((size - 1) / 8); i++) {
				/* Get the bit about to the shifted off current byte */
				lbit = bit_get(&bits[i], 0);
                
				if (i == 0)
					fbit = lbit;
				else
					bit_set(&bits[i - 7], 7, lbit);
                
				bits[i] <<= 1;
			}
            
			bit_set(bits, size - 1, fbit);
		}
	}
}
