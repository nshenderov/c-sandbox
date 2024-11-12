/*******************************************************************************
*
* FILENAME : bitarray_test.c
*
* DESCRIPTION : Bitarray unit tests.

* AUTHOR : Nick Shenderov
*
* DATE : 31.03.2023
* 
*******************************************************************************/

#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "bitarray.h"
#include "testing.h"


static void TestSetAll(void);
static void TestResetAll(void);
static void TestSetOn(void);
static void TestSetOff(void);
static void TestSetBit(void);
static void TestFlip(void);
static void TestRotateRight(void);
static void TestRotateLeft(void);
static void TestCountOn(void);
static void TestCountOnLUT(void);
static void TestCountOff(void);
static void TestMirror(void);
static void TestMirrorLUT(void);
static void TestGetVal(void);
static void TestToString(void);

int main()
{
    TH_TEST_T tests[] = {
		{"SetAll", TestSetAll},
		{"ResetAll", TestResetAll},
		{"SetOn", TestSetOn},
		{"SetOff", TestSetOff},
		{"SetBit", TestSetBit},
		{"Flip", TestFlip},
		{"RotateRight", TestRotateRight},
		{"RotateLeft", TestRotateLeft},
		{"CountOn", TestCountOn},
		{"CountOnLUT", TestCountOnLUT},
		{"CountOff", TestCountOff},
		{"Mirror", TestMirror},
		{"MirrorLUT", TestMirrorLUT},
		{"GetVal", TestGetVal},
		{"ToString", TestToString},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestSetAll(void)
{
	bitsarr_t bit_array = 0;

	bit_array = BitArraySetAll(bit_array);
    TH_ASSERT(0xFFFFFFFFFFFFFFFF == bit_array);

	bit_array = 0xFFFFF3232FFFFFFF;
	bit_array = BitArraySetAll(bit_array);
    TH_ASSERT(0xFFFFFFFFFFFFFFFF == bit_array);
 }

static void TestResetAll(void)
{
	bitsarr_t bit_array = 0;

	bit_array = BitArrayResetAll(bit_array);
    TH_ASSERT(0 == bit_array);

	bit_array = 0xFFFFF3232FFFFFFF;
	bit_array = BitArrayResetAll(bit_array);
	TH_ASSERT(0 == bit_array);
}
static void TestSetOn(void)
{
	bitsarr_t bit_array = 0;

	bit_array = BitArraySetOn(bit_array, 2);
    TH_ASSERT(4 == bit_array);

	bit_array = 0xFFFFF3232FFFFFFF;
	bit_array = BitArraySetOn(bit_array, 35);
	bit_array = BitArraySetOn(bit_array, 34);
	bit_array = BitArraySetOn(bit_array, 28);
    TH_ASSERT(0xFFFFF32F3FFFFFFF == bit_array);
}

static void TestSetOff(void)
{
	bitsarr_t bit_array = 0;

	bit_array = BitArraySetOff(bit_array, 2);
	TH_ASSERT(0 == bit_array);

	bit_array = 0xFFFFF3232FFFFFFF;
	bit_array = BitArraySetOff(bit_array, 63);
	bit_array = BitArraySetOff(bit_array, 32);
	bit_array = BitArraySetOff(bit_array, 0);
    TH_ASSERT(0x7FFFF3222FFFFFFE == bit_array);
}

static void TestSetBit(void)
{
	bitsarr_t bit_array = 0;

	bit_array = BitArraySetBit(bit_array, 2, 0);
	TH_ASSERT(0 == bit_array);

	bit_array = 0xFFFFF3232FFFFFFF;
	bit_array = BitArraySetBit(bit_array, 63, 0);
	bit_array = BitArraySetBit(bit_array, 32, 0);
	bit_array = BitArraySetBit(bit_array, 0, 0);
    TH_ASSERT(0x7FFFF3222FFFFFFE == bit_array);

	bit_array = 0xFFFFF3232FFFFFFF;
	bit_array = BitArraySetBit(bit_array, 43, 1);
	bit_array = BitArraySetBit(bit_array, 42, 1);
	bit_array = BitArraySetBit(bit_array, 28, 1);
    TH_ASSERT(0xFFFFFF233FFFFFFF == bit_array);
}

static void TestFlip(void)
{
	bitsarr_t bit_array = 0;

	bit_array = 0xFFFFF3232FFFFFFF;
	bit_array = BitArrayFlip(bit_array, 63);
	bit_array = BitArrayFlip(bit_array, 32);
	bit_array = BitArrayFlip(bit_array, 0);
    TH_ASSERT(0x7FFFF3222FFFFFFE == bit_array);

	bit_array = 0xFFFFF3232FFFFFFF;
	bit_array = BitArrayFlip(bit_array, 43);
	bit_array = BitArrayFlip(bit_array, 42);
	bit_array = BitArrayFlip(bit_array, 28);
    TH_ASSERT(0xFFFFFF233FFFFFFF == bit_array);
}

static void TestRotateRight(void)
{
	bitsarr_t bit_array = 0;

	bit_array = 16;
	bit_array = BitArrayRotateRight(bit_array, 2);
    TH_ASSERT(4 == bit_array);
}

static void TestRotateLeft(void)
{
	bitsarr_t bit_array = 0;

	bit_array = 16;
	bit_array = BitArrayRotateLeft(bit_array, 2);
    TH_ASSERT(64 == bit_array);
}

static void TestCountOn(void)
{
	bitsarr_t bit_array = 0;
	size_t bits = 0;

	bit_array = 123213;
	bits = BitArrayCountOn(bit_array);
    TH_ASSERT(9 == bits);

	bit_array = 0;
	bits = BitArrayCountOn(bit_array);
    TH_ASSERT(0 == bits);
}

static void TestCountOnLUT(void)
{
	bitsarr_t bit_array = 0;
	size_t bits = 0;

	bit_array = 123213;
	bits = BitArrayCountOnLUT(bit_array);
    TH_ASSERT(9 == bits);

	bit_array = 0;
	bits = BitArrayCountOnLUT(bit_array);
	TH_ASSERT(0 == bits);
}

static void TestCountOff(void)
{
	bitsarr_t bit_array = 0;
	size_t bits = 0;

	bit_array = 123213;
	bits = BitArrayCountOff(bit_array);
    TH_ASSERT(55 == bits);

	bit_array = 0;
	bits = BitArrayCountOff(bit_array);
    TH_ASSERT(64 == bits);
}

static void TestMirror(void)
{
	bitsarr_t bit_array = 0;

	bit_array = 16;
	bit_array = BitArrayMirror(bit_array);
    TH_ASSERT(0x800000000000000 == bit_array);

	bit_array = 0x3000000000000000;
	bit_array = BitArrayMirror(bit_array);
    TH_ASSERT(0xC == bit_array);
}

static void TestMirrorLUT(void)
{
	bitsarr_t bit_array = 0;

	bit_array = 16;
	bit_array = BitArrayMirror(bit_array);
    TH_ASSERT(0x800000000000000 == bit_array);

	bit_array = 0x3000000000000000;
	bit_array = BitArrayMirror(bit_array);
    TH_ASSERT(0xC == bit_array);
}

static void TestGetVal(void)
{
	bitsarr_t bit_array = 0;
	int bit = 0;

	bit_array = 0x800000000000000;
	bit = BitArrayGetVal(bit_array, 59);
    TH_ASSERT(1 == bit);

	bit_array = 0x800000000000000;
	bit = BitArrayGetVal(bit_array, 0);
	TH_ASSERT(0 == bit);
}

static void TestToString(void)
{
	bitsarr_t bit_array = 0;
	char str[65] = {0};

	bit_array = 0xFFFFFF233FFFFFF;
	BitArrayToString(bit_array, str);
    TH_ASSERT(0 == strcmp(str, "0000111111111111111111111111001000110011111111111111111111111111"));

	bit_array = 7;
	BitArrayToString(bit_array, str);
    TH_ASSERT(0 == strcmp(str, "0000000000000000000000000000000000000000000000000000000000000111"));
}