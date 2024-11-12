/*******************************************************************************
*
* FILENAME : fsa.c
*
* DESCRIPTION : Fixed-size allocator implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 16.05.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */

#include "fsa.h"


enum {FALSE, TRUE};

#define LAST_BLOCK (0)

#define FSA_STRUCT_SIZE (sizeof(struct fsa))

#define WORD_SIZE (sizeof(unsigned long))

#define IS_MEMORY_ALIGN(POINTER) \
(0 == ((unsigned long) POINTER & (WORD_SIZE - 1)))

#define ALIGN_NUMBER(NUMBER) \
(NUMBER = (((unsigned long)(NUMBER + (WORD_SIZE - 1))) & ~(WORD_SIZE - 1)))


struct fsa
{
    size_t next_free_offset;
};


fsa_t *FSAInit(size_t size_of_block, size_t number_of_blocks, void *memory_pool)
{
	fsa_t *new_fsa = NULL;
	void *runner = NULL;
	size_t offset_runner = FSA_STRUCT_SIZE;

	assert(NULL != memory_pool);
	assert(TRUE == IS_MEMORY_ALIGN(memory_pool));

	ALIGN_NUMBER(size_of_block);

	new_fsa = memory_pool;

	new_fsa->next_free_offset = FSA_STRUCT_SIZE;

	runner = (char *)new_fsa + new_fsa->next_free_offset;

	while(1 != number_of_blocks)
	{
		*(size_t *) runner = offset_runner + size_of_block;

		offset_runner += size_of_block;
		runner = (char *)runner + size_of_block;
		--number_of_blocks;
	}

	*(size_t *) runner = LAST_BLOCK;

	return (new_fsa);
}

void *FSAAlloc(fsa_t *fsa)
{
	void *free_block = NULL;

	assert(NULL != fsa);

	if (LAST_BLOCK == fsa->next_free_offset)
	{
		return (NULL);
	}

	free_block = (char *)fsa + (fsa->next_free_offset);

	fsa->next_free_offset = *(size_t *) free_block;

	return (free_block);
}

void FSAFree(fsa_t *fsa, void *block_to_free)
{
	size_t fsa_curr_offset = 0;
	size_t block_offset = 0;

	assert(NULL != fsa);
	assert(NULL != block_to_free);

	fsa_curr_offset = fsa->next_free_offset;

	block_offset = (char *)block_to_free - (char *)fsa;

	fsa->next_free_offset = block_offset;

	*(size_t *) block_to_free = fsa_curr_offset;
}

size_t FSASuggestSize(size_t numb_of_blocks, size_t block_size)
{
	ALIGN_NUMBER(block_size);

	return (FSA_STRUCT_SIZE + (numb_of_blocks * block_size));
}

size_t FSACountFree(const fsa_t *fsa)
{
	char *runner = NULL;
	size_t counter = 0;

	assert(NULL != fsa);

	if (LAST_BLOCK == fsa->next_free_offset)
	{
		return (0);
	}

	runner = ((char *)fsa + (fsa->next_free_offset));

	++counter;

	while(LAST_BLOCK != *(size_t *) runner)
	{
		++counter;
		runner = (char *)fsa + (*(size_t *) runner);
	}

	return (counter);
}


