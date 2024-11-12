/*******************************************************************************
*
* FILENAME : bitarray.h
*
* DESCRIPTION : Bit Array is a data structure that efficiently stores a
* collection of binary values, or "bits" in a compact format. Each bit in the
* array represents a binary state (0 or 1) and is typically used to track flags,
* represent sets, or optimize storage for boolean data.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 31.03.2023
* 
*******************************************************************************/

#ifndef __NSRD_BITARRAY_H__
#define __NSRD_BITARRAY_H__

#include <stddef.h> /* size_t */ 

typedef size_t bitsarr_t;

/*
DESCRIPTION
	Sets all bits in the bit_array to 1.
RETURN
	Function returns the bit array.
INPUT
	bit_array: value of the bits array.
*/
bitsarr_t BitArraySetAll(bitsarr_t bit_array);

/*
DESCRIPTION
	Clears all bits of the bit_array to 0.
RETURN
	Function returns the bit array.
INPUT
	bit_array: value of the bit array.
*/
bitsarr_t BitArrayResetAll(bitsarr_t bit_array);

/*
DESCRIPTION
	Sets a bit with an index idx to 1.	
	The index should be in the range of 0-63. If the index falls out of 
	this range undefined behavior is to be expected.
RETURN
	Function returns the bit array.
INPUT
	bit_array: value of the bit array
	idx: index (from 0 to 63) of the element to set.
*/
bitsarr_t BitArraySetOn(bitsarr_t bit_array, size_t idx);

/*
DESCRIPTION
	Sets the bit with the index idx to 0.
	The index should be in the range of 0-63. If the index falls out of 
	this range undefined behavior is to be expected.
RETURN
	Function returns the bit array.
INPUT
	bit_array: value of the bit array
	idx: index (from 0 to 63) of the bit to set.
*/
bitsarr_t BitArraySetOff(bitsarr_t bit_array, size_t idx);

/*
DESCRIPTION
	Sets the indexed bit in the bit array to the passed value.
	The index should be in the range of 0-63. If the index falls out of 
	this range undefined behavior is to be expected.
	The value should be 0 or 1.
RETURN
	Function returns the bit array.
INPUT
	bit_array: value of the bit array;
	idx: index of the bit (from 0 to 63);
	value: 1 or 0.
*/
bitsarr_t BitArraySetBit(bitsarr_t bit_array, size_t idx, int value);

/*
DESCRIPTION
	Checks the bit with indexed idx in the bit-array and returns
	its value. The index should be in the range of 0-63. If the index 
	falls out of this range undefined behavior is to be expected.
RETURN
	1 if the bit is set.
	0 if the bit is clear.
INPUT
	bit_array: value of the bit array;
	idx: index of the bit (from 0 to 63). 
*/
int BitArrayGetVal(bitsarr_t bit_array, size_t idx);

/*
DESCRIPTION
	Flips value of the bit with index idx in the bit array to the 
	oposite value. The index should be in the range of 0-63. If the index 
	falls out of this range undefined behavior is to be expected.
RETURN
	Function returns the bit array.
INPUT
	bit_array: value of the bit array;
	idx: index of the bit (from 0 to 63).
*/
bitsarr_t BitArrayFlip(bitsarr_t bit_array, size_t idx);

/*
DESCRIPTION: 
	Reverses the bit array.
	BitArrayMirror (basic implementation).
    BitArrayMirrorLUT (lookup table implementation).
RETURN: 
	Function returns the bit array.
INPUT:
   bit_array: value of the bit array.
*/
bitsarr_t BitArrayMirror(bitsarr_t bit_array);
bitsarr_t BitArrayMirrorLUT(bitsarr_t bit_array);

/*
DESCRIPTION
	Rotates all bits in the bit array to the right.
	Number of steps to rotate should be no more than 64 (size of the bit array).
	If number of steps is larger undefined behavior is to be expected.
RETURN
	Function returns the bit array.
INPUT
	bit_array: value of the bit array;
	num: number of steps to rotate (1 to 64).
*/
bitsarr_t BitArrayRotateRight(bitsarr_t bit_array, size_t steps);

/*
DESCRIPTION
	Rotates all bits in the bit array to the left.
	Number of steps to rotate should be no more than 64 (size of the bit array).
	If number of steps is larger undefined behavior is to be expected.
RETURN
	Function returns the bit array.
INPUT
	bit_array: value of the bit array;
	num: number of steps to rotate (1 to 64).
*/
bitsarr_t BitArrayRotateLeft(bitsarr_t bit_array, size_t steps);

/*
DESCRIPTION
	Counts the number of bits set to 1.
	BitArrayCountOn implements the function without a lookup table.
    BitArrayCountOnLUT implements the function with a lookup table.	
RETURN
	Function returns the number of set bits.
INPUT
	bit_array: value of the bit array.
*/
size_t BitArrayCountOn(bitsarr_t bit_array);
size_t BitArrayCountOnLUT(bitsarr_t bit_array);

/*
DESCRIPTION
	Counts the number of bits set to 0.
RETURN
	Function returns the number of set bits.
INPUT
	bit_array: value of the bit array.
*/
size_t BitArrayCountOff(bitsarr_t bit_array);

/*
DESCRIPTION
	Writes the bit representation of a bit array 
	to the provided string. The string has to be at least 65 characters long. 
	If the string is shorter undefined behavior is to be expected.
RETURN
	Function returns a char pointer to the string.
INPUT
	bit_array: value of the bit array.
	string: pointer to the string.
*/
char *BitArrayToString(bitsarr_t bit_array, char *string);


#endif /* __NSRD_BITARRAY_H__ */