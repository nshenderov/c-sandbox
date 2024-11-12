/*******************************************************************************
*
* FILENAME : bitarray.c
*
* DESCRIPTION : Bitarray implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 31.03.2023
*
* PUBLIC FUNCTIONS :
*		bitsarr_t BitArraySetAll(bitsarr_t bit_array);
*		bitsarr_t BitArrayResetAll(bitsarr_t bit_array);
*		bitsarr_t BitArraySetOn(bitsarr_t bit_array, size_t idx);
*		bitsarr_t BitArraySetOff(bitsarr_t bit_array, size_t idx);
*		bitsarr_t BitArraySetBit(bitsarr_t bit_array, size_t idx, int value);
*		int BitArrayGetVal(bitsarr_t bit_array, size_t idx);
*		bitsarr_t BitArrayFlip(bitsarr_t bit_array, size_t idx);
*		bitsarr_t BitArrayMirror(bitsarr_t bit_array);
*		bitsarr_t BitArrayMirrorLUT(bitsarr_t bit_array);
*		bitsarr_t BitArrayRotateRight(bitsarr_t bit_array, size_t steps);
*		bitsarr_t BitArrayRotateLeft(bitsarr_t bit_array, size_t steps);
*		size_t BitArrayCountOn(bitsarr_t bit_array);
*		size_t BitArrayCountOnLUT(bitsarr_t bit_array);
*		size_t BitArrayCountOff(bitsarr_t bit_array);
*		char *BitArrayToString(bitsarr_t bit_array, char *string);
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <limits.h> /* CHAR_BIT */

#include "bitarray.h"

#define MAX_BITS (sizeof(bitsarr_t) * CHAR_BIT)
#define MASK_FULL (0xFFFFFFFFFFFFFFFF)
#define MASK_HALF_FIRST (0x00000000FFFFFFFF)
#define MASK_HALF_SECOND (0xFFFFFFFF00000000)

#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)

#define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
#define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
#define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )

static const unsigned char g_lut[256]
    = { B6(0), B6(1), B6(1), B6(2) };

static const unsigned char g_lut_reverse[256]
    = { R6(0), R6(2), R6(1), R6(3) };


bitsarr_t BitArraySetAll(bitsarr_t bit_array)
{
	return (bit_array |= MASK_FULL);
}

bitsarr_t BitArrayResetAll(bitsarr_t bit_array)
{
	return (bit_array &= 0x0);
}

bitsarr_t BitArraySetOn(bitsarr_t bit_array, size_t idx)
{
	assert(MAX_BITS > idx);

	return (bit_array |= 1UL << idx);
}

bitsarr_t BitArraySetOff(bitsarr_t bit_array, size_t idx)
{
	assert(MAX_BITS > idx);

	return (bit_array &= ~(1UL << idx));
}

bitsarr_t BitArraySetBit(bitsarr_t bit_array, size_t idx, int value)
{
	assert(MAX_BITS > idx);
	assert(2 > value);
	assert(-1 < value);

	return (value ? (bit_array |= 1UL << idx) : (bit_array &= ~(1UL << idx)));
}

int BitArrayGetVal(bitsarr_t bit_array, size_t idx)
{
	assert(MAX_BITS > idx);

	return ((bit_array >> idx) & 0x1);
}

bitsarr_t BitArrayFlip(bitsarr_t bit_array, size_t idx)
{
	assert(MAX_BITS > idx);

	return (bit_array ^= 1UL << idx);
}

bitsarr_t BitArrayMirror(bitsarr_t bit_array)
{
	bitsarr_t mirror = bit_array;
	int s = sizeof(bitsarr_t) * CHAR_BIT - 1;

	for (bit_array >>= 1; bit_array; bit_array >>= 1)
	{   
	  mirror <<= 1;
	  mirror |= bit_array & 1;
	  s--;
	}
	mirror <<= s;

	return (mirror);
}

bitsarr_t BitArrayMirrorLUT(bitsarr_t bit_array)
{
	bitsarr_t mirrored = 0;
	unsigned char *p = (unsigned char *) &bit_array;
	unsigned char *q = (unsigned char *) &mirrored;

	q[7] = g_lut_reverse[p[0]]; 
	q[6] = g_lut_reverse[p[1]]; 
	q[5] = g_lut_reverse[p[2]]; 
	q[4] = g_lut_reverse[p[3]];
	q[3] = g_lut_reverse[p[4]]; 
	q[2] = g_lut_reverse[p[5]]; 
	q[1] = g_lut_reverse[p[6]]; 
	q[0] = g_lut_reverse[p[7]];

	bit_array = ((bit_array & MASK_HALF_SECOND) >> 32) |  ((bit_array & MASK_HALF_FIRST) << 32);

	return (mirrored);
}

bitsarr_t BitArrayRotateRight(bitsarr_t bit_array, size_t steps)
{
	assert(MAX_BITS > steps);

	return (bit_array >> steps) | (bit_array << (MAX_BITS - steps));
}

bitsarr_t BitArrayRotateLeft(bitsarr_t bit_array, size_t steps)
{
	assert(MAX_BITS > steps);
	
	return (bit_array << steps) | (bit_array >> (MAX_BITS - steps));
}

size_t BitArrayCountOn(bitsarr_t bit_array)
{
	size_t bit_counter = 0;
	
	while (0 != bit_array)
	{
		bit_array = bit_array & (bit_array - 1);
		++bit_counter;
	} 	
	return (bit_counter);
}
size_t BitArrayCountOnLUT(bitsarr_t bit_array)
{
	size_t bits_count = 0;

	bits_count = g_lut[bit_array & 0xff] + 
			g_lut[(bit_array >> 8) & 0xff] + 
			g_lut[(bit_array >> 16) & 0xff] + 
			g_lut[(bit_array >> 24) & 0xff] +
			g_lut[(bit_array >> 32) & 0xff] +
			g_lut[(bit_array >> 40) & 0xff] +
			g_lut[(bit_array >> 48) & 0xff] +
			g_lut[(bit_array >> 56) & 0xff];

	return (bits_count);
}

size_t BitArrayCountOff(bitsarr_t bit_array)
{
	return (MAX_BITS - BitArrayCountOnLUT(bit_array));
}

char *BitArrayToString(bitsarr_t bit_array, char *dest)
{
	int i = sizeof(bitsarr_t) * CHAR_BIT - 1;

	assert(NULL != dest);

	dest += i;

	for (; i >= 0; --i)
	{
        *dest = (bit_array & 1) + '0';
        bit_array >>= 1;
        --dest;
    }

    return (dest);
}