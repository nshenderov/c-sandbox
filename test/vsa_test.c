/*******************************************************************************
*
* FILENAME : vsa_test.c
*
* DESCRIPTION : Variable-size allocator unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 17.05.2023
* 
*******************************************************************************/

#include <stdlib.h> /* malloc, free */

#include "vsa.h"
#include "testing.h"


#define FREE_AND_SET_TO_NULL(ptr) \
{free(ptr); (ptr) = NULL;}


static void TestVSAInit(void);
static void TestVSAAlloc(void);
static void TestVSAAllocEdge1(void);
static void TestVSAAllocEdge2(void);
static void TestVSAFree(void);
static void TestVSALargestChunkAvailable(void);

int main()
{
	TH_TEST_T tests[] = {
		{"VSALargestChunkAvailable", TestVSALargestChunkAvailable},
		{"VSAInit", TestVSAInit},
		{"VSAAlloc", TestVSAAlloc},
		{"VSAFree", TestVSAFree},
		{"VSAAlloc edge1", TestVSAAllocEdge1},
		{"VSAAlloc edge2", TestVSAAllocEdge2},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestVSALargestChunkAvailable(void)
{
	int *pool = (int *) malloc(46);

	vsa_t *vsa = VSAInit(46, pool);

	TH_ASSERT(14 == VSALargestChunkAvailable(vsa));

	free(pool);
}

static void TestVSAInit(void)
{
	int *pool = (int *) malloc(33);

	vsa_t *vsa = VSAInit(33, pool);

	TH_ASSERT(0 == VSALargestChunkAvailable(vsa));

	free(pool);
}

static void TestVSAFree(void)
{
	int *pool = (int *) malloc(100);
	int *block1 = NULL;
	
	vsa_t *vsa = VSAInit(100, pool);
	
	block1 = VSAAlloc(vsa, 8);

	TH_ASSERT(44 == VSALargestChunkAvailable(vsa));

	free(pool);

	(void) block1;
}

static void TestVSAAlloc(void)
{
	int *pool = (int *) malloc(100);
	int *block1 = NULL;
	
	vsa_t *vsa = VSAInit(100, pool);
	
	block1 = VSAAlloc(vsa, 8);

	TH_ASSERT(44 == VSALargestChunkAvailable(vsa));

	VSAFree(block1);

	TH_ASSERT(68 == VSALargestChunkAvailable(vsa));

	free(pool);

	(void) block1;
}

static void TestVSAAllocEdge1(void)
{
	int *pool = (int *) malloc(129);
	int *block1 = NULL;
	size_t *block2 = NULL;
	size_t *block3 = NULL;

	vsa_t *vsa = VSAInit(129, pool);

	block1 = VSAAlloc(vsa, 8);
	block2 = VSAAlloc(vsa, 24);
	block3 = VSAAlloc(vsa, 32);

	TH_ASSERT(0 == VSALargestChunkAvailable(vsa));

	VSAFree(block3);
	VSAFree(block1);

	TH_ASSERT(32 == VSALargestChunkAvailable(vsa));

	block3 = VSAAlloc(vsa, 32);

	TH_ASSERT(8 == VSALargestChunkAvailable(vsa));

	VSAFree(block2);

	TH_ASSERT(48 == VSALargestChunkAvailable(vsa));

	free(pool);

	(void) block1;
	(void) block2;
	(void) block3;
}

static void TestVSAAllocEdge2(void)
{
	int *pool = (int *) malloc(129);
	int *block1 = NULL;
	size_t *block2 = NULL;
	size_t *block3 = NULL;

	vsa_t *vsa = VSAInit(129, pool);

	block1 = VSAAlloc(vsa, 8);
	block2 = VSAAlloc(vsa, 16);
	block3 = VSAAlloc(vsa, 32);

	TH_ASSERT(0 == VSALargestChunkAvailable(vsa));

	VSAFree(block3);
	VSAFree(block1);

	TH_ASSERT(40 == VSALargestChunkAvailable(vsa));

	free(pool);

	(void) block1;
	(void) block2;
	(void) block3;
}
