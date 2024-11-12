/*******************************************************************************
*
* FILENAME : fsa_test.c
*
* DESCRIPTION : Fixed-size allocator unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 16.05.2023
* 
*******************************************************************************/

#include <stdlib.h> /* malloc, free*/

#include "fsa.h"
#include "testing.h"


#define WORD_SIZE (sizeof(unsigned long))
#define IS_MEMORY_ALIGN(POINTER) \
(0 == ((unsigned long) POINTER & (WORD_SIZE - 1)))


static void TestFSASuggestSize(void);
static void TestFSAAlloc(void);
static void TestFSAFree(void);
static void TestFSACountFree(void);
static void TestFSAInit(void);


int main()
{
	TH_TEST_T tests[] = {
		{"FSASuggestSize", TestFSASuggestSize},
		{"FSACountFree", TestFSACountFree},
		{"FSAAlloc", TestFSAAlloc},
		{"FSAFree", TestFSAFree},
		{"FSAInit", TestFSAInit},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestFSASuggestSize(void)
{
	TH_ASSERT(24 == FSASuggestSize(2, 8));

	TH_ASSERT(24 == FSASuggestSize(2, 3));

	TH_ASSERT(40 == FSASuggestSize(2, 15));
}

static void TestFSACountFree(void)
{
	int *pool = NULL;
	fsa_t *fsa = NULL;

	size_t size = FSASuggestSize(2, 8);

	TH_ASSERT(24 == FSASuggestSize(2, 8));

	pool = (int *) malloc(size);
	
	fsa = FSAInit(8, 2, pool);

	TH_ASSERT(2 == FSACountFree(fsa));
	
	fsa = FSAInit(8, 1, pool);

	TH_ASSERT(1 == FSACountFree(fsa));

	free(pool);
}

static void TestFSAAlloc(void)
{
	int *pool = NULL;
	fsa_t *fsa = NULL;
	size_t *block1 = NULL;
	int *block2 = NULL;
	char *block3 = NULL;

	size_t size = FSASuggestSize(2, 8);

	TH_ASSERT(24 == FSASuggestSize(2, 8));

	pool = (int *) malloc(size);
	
	fsa = FSAInit(8, 2, pool);

	block1 = FSAAlloc(fsa);
	block2 = FSAAlloc(fsa);
	block3 = FSAAlloc(fsa);

	TH_ASSERT(0 == FSACountFree(fsa));

	*block1 = 123;
	*block2 = 555;

	TH_ASSERT(NULL == block3);
	
	free(pool);
}

static void TestFSAFree(void)
{
	int *pool = NULL;
	fsa_t *fsa = NULL;
	size_t *block1 = NULL;
	int *block2 = NULL;
	char *block3 = NULL;

	size_t size = FSASuggestSize(2, 8);

	TH_ASSERT(24 == FSASuggestSize(2, 8));

	pool = (int *) malloc(size);
	
	fsa = FSAInit(8, 2, pool);

	block1 = FSAAlloc(fsa);
	block2 = FSAAlloc(fsa);
	block3 = FSAAlloc(fsa);

	TH_ASSERT(0 == FSACountFree(fsa));

	*block1 = 123;
	*block2 = 555;

	FSAFree(fsa, block1);
	TH_ASSERT(1 == FSACountFree(fsa));

	FSAFree(fsa, block2);
	TH_ASSERT(2 == FSACountFree(fsa));

	TH_ASSERT(NULL == block3);

	block1 = FSAAlloc(fsa);

	*block1 = 123;

	TH_ASSERT(1 == FSACountFree(fsa));

	FSAFree(fsa, block1);

	TH_ASSERT(2 == FSACountFree(fsa));

	free(pool);
}

static void TestFSAInit(void)
{
	int *pool = NULL;
	fsa_t *fsa = NULL;
	size_t *block1 = NULL;
	int *block2 = NULL;
	char *block3 = NULL;

	size_t size = FSASuggestSize(2, 3);

	TH_ASSERT(24 == FSASuggestSize(2, 3));

	pool = (int *) malloc(size);
	
	fsa = FSAInit(3, 2, pool);

	block1 = FSAAlloc(fsa);
	block2 = FSAAlloc(fsa);
	block3 = FSAAlloc(fsa);

	TH_ASSERT(1 == IS_MEMORY_ALIGN(block1));

	TH_ASSERT(1 == IS_MEMORY_ALIGN(block2));

	TH_ASSERT(0 == FSACountFree(fsa));

	*block1 = 123;
	*block2 = 555;

	FSAFree(fsa, block1);
	TH_ASSERT(1 == FSACountFree(fsa));

	FSAFree(fsa, block2);
	TH_ASSERT(2 == FSACountFree(fsa));

	TH_ASSERT(NULL == block3);

	block1 = FSAAlloc(fsa);

	*block1 = 123;

	TH_ASSERT(1 == FSACountFree(fsa));

	FSAFree(fsa, block1);

	TH_ASSERT(2 == FSACountFree(fsa));

	free(pool);
}