#ifndef BIT_OPERATIONS_H
#define BIT_OPERATIONS_H

extern int bit_get(const unsigned char *bits, int pos);
extern void bit_set(unsigned char *bits, int pos, int state);
extern void bit_xor(const unsigned char *bits1, const unsigned char *bits2,
		    unsigned char *bitsx, int size);
extern void bit_rot_left(unsigned char *bits, int size, int count);

#endif

