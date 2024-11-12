/*******************************************************************************
*
* FILENAME : vsa.c
*
* DESCRIPTION : Variable-size allocator implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 17.05.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */

#include "vsa.h"


enum {FALSE, TRUE};

#define END_MAGIC_NUMBER (0xDEADBEEF)
#define DGB_HEADER_MAGIC_NUMBER (0xDEADBABE)
#define HEADER_STRUCT_SIZE (sizeof(struct block_header))
#define WORD_SIZE (sizeof(unsigned long))
#define ALLOC_FLAG (1)
#define ALIGN_MASK ((WORD_SIZE - 1))

#define IS_MEMORY_ALIGN(POINTER) \
(0 == ((unsigned long) POINTER & ALIGN_MASK))

#define ALIGN_NUMBER(NUMBER) \
(NUMBER = (((unsigned long)(NUMBER + ALIGN_MASK)) & ~ALIGN_MASK))

#define IS_NUMBER_EVEN(NUMBER) (!(NUMBER & 0x1))

#define IS_THE_END(HEADER) \
(END_MAGIC_NUMBER != HEADER->block_size)

#define IS_BLOCK_FREE(HEADER) \
(TRUE == IS_NUMBER_EVEN(HEADER->block_size))

#define DEFLAG_SIZE(BLOCK_SIZE) (BLOCK_SIZE & ~ALLOC_FLAG)

typedef struct block_header block_t;

struct block_header
{
    size_t block_size;

    #ifndef NDEBUG
    size_t magic_number;
    #endif
};

static block_t *DefragFindSpace(vsa_t *header, size_t req_size);
static block_t *AccessHeader(block_t *block);
static void InitializeHeader(block_t *block, size_t size);
static void InitializeBlock(block_t *header, size_t alloc_size);
static block_t *GetNextHeader(block_t *block);
static size_t GetWholeBlockSize(block_t *block);

vsa_t *VSAInit(size_t pool_size, void *memory_pool)
{
	block_t *header = NULL;
	block_t *footer = NULL;

	assert(NULL != memory_pool);
	assert(TRUE == IS_MEMORY_ALIGN(memory_pool));
	assert(0 < pool_size);

	if (!IS_NUMBER_EVEN(pool_size))
	{
		pool_size = pool_size & ~ALIGN_MASK;
	}

	header = memory_pool;
	footer = (vsa_t *) ((char *) memory_pool + (pool_size - HEADER_STRUCT_SIZE));
	
	InitializeHeader(header, pool_size - (HEADER_STRUCT_SIZE * 2));
	InitializeHeader(footer, END_MAGIC_NUMBER);

    return (header);
}

void *VSAAlloc(vsa_t *vsa, size_t bytes)
{
	block_t *founded_block = NULL;
	char *initialized_block = NULL;

	assert(0 < bytes);
	assert(NULL != vsa);

	ALIGN_NUMBER(bytes);

	founded_block = DefragFindSpace(vsa, bytes);
	if (END_MAGIC_NUMBER == founded_block->block_size)
	{
		return (NULL);
	}

	InitializeBlock(founded_block, bytes);

	initialized_block = (char *) founded_block + HEADER_STRUCT_SIZE;

	return (initialized_block);
}

static void InitializeBlock(block_t *header, size_t alloc_size)
{
	block_t *new_header = NULL;
	size_t free_size = header->block_size;
	size_t remain_size = free_size - alloc_size;
 
	if (remain_size < HEADER_STRUCT_SIZE + WORD_SIZE)
	{
		header->block_size = free_size | ALLOC_FLAG;
	}
	else
	{
		new_header = (block_t *) ((char *) header + alloc_size + HEADER_STRUCT_SIZE);
		InitializeHeader(new_header, remain_size - HEADER_STRUCT_SIZE);
		header->block_size = alloc_size | ALLOC_FLAG;
	}
}

void VSAFree(void *block_to_free)
{
	vsa_t *header = NULL;

	assert(NULL != block_to_free);

	header = AccessHeader(block_to_free);

	assert(DGB_HEADER_MAGIC_NUMBER == header->magic_number);

	header->block_size = DEFLAG_SIZE(header->block_size);
}

size_t VSALargestChunkAvailable(vsa_t *vsa)
{
	block_t *header_runner = NULL;
	block_t *chunk_start = NULL;
	size_t max_available_size = 0;

	assert(NULL != vsa);

	header_runner = vsa;

	while(FALSE != IS_THE_END(header_runner))
	{
		if (TRUE == IS_BLOCK_FREE(header_runner))
		{
			if (NULL == chunk_start)
			{
				chunk_start = header_runner;
			}
			else
			{
				chunk_start->block_size += GetWholeBlockSize(header_runner);
			}

			if (chunk_start->block_size > max_available_size)
			{
				max_available_size = chunk_start->block_size;
			}
		}
		else if (NULL != chunk_start)
		{
			chunk_start = NULL;
		}

		header_runner = GetNextHeader(header_runner);
	}
	return (max_available_size);
}

static block_t *DefragFindSpace(vsa_t *header, size_t req_size)
{
	block_t *header_runner = NULL;
	block_t *chunk_start = NULL;

	assert(NULL != header);

	header_runner = header;

	while(FALSE != IS_THE_END(header_runner))
	{
		if (TRUE == IS_BLOCK_FREE(header_runner))
		{
			if (NULL == chunk_start)
			{
				chunk_start = header_runner;
			}
			else
			{
				chunk_start->block_size += GetWholeBlockSize(header_runner);
			}

			if (req_size <= chunk_start->block_size)
			{
				return (chunk_start);
			}

		}
		else if (NULL != chunk_start)
		{
			chunk_start = NULL;
		}

		header_runner = GetNextHeader(header_runner);
	}

	return (header_runner);
}

static block_t *AccessHeader(block_t *block)
{
	return ((block_t *) ((char *) block - HEADER_STRUCT_SIZE));
}

static void InitializeHeader(block_t *block, size_t size)
{
	block_t *header = NULL;

	assert(NULL != block);

	header = block;

	header->block_size = size;

	#ifndef NDEBUG
    header->magic_number = DGB_HEADER_MAGIC_NUMBER;
    #endif
}

static block_t *GetNextHeader(block_t *block)
{
	size_t block_size = 0;

	assert(NULL != block);

	block_size = block->block_size;

	if (TRUE == IS_NUMBER_EVEN(block_size))
	{
		return ((block_t *) ((char *) block + block_size + HEADER_STRUCT_SIZE));	
	}
	else
	{
		return ((block_t *) ((char *) block + DEFLAG_SIZE(block_size) + HEADER_STRUCT_SIZE));	
	}
}

static size_t GetWholeBlockSize(block_t *block)
{
	size_t block_size = 0;

	assert(NULL != block);

	block_size = block->block_size;

	return (block_size + HEADER_STRUCT_SIZE);
}