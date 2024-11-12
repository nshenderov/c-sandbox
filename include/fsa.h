/*******************************************************************************
*
* FILENAME : fsa.h
*
* DESCRIPTION : Fixed-size allocator also called as memory pool or fixed-size
* block allocator. Fixed-size allocator provides memory management that allows
* dynamic memory allocation comparable to malloc.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 16.05.2023
* 
*******************************************************************************/

#ifndef __NSRD_FSA_H__
#define __NSRD_FSA_H__

#include <stddef.h>

typedef struct fsa fsa_t;

/*
DESCRIPTION:
    Initializes a fixed-size allocator.
	User must provide pointer to aligned pool of the memory.
	Provided memory pool should be not less than (size of fsa_t + WORD_SIZE)
	and should be big enough to accommodate the requested amount of blocks
	otherwise, the behavior is undefined.
	Specified size_of_block should be number aligned with WORD_SIZE and is 
	going to be aligned during initialization if is not.
	It is advised to use FSASuggestSize function to compute needed
	amount of bytes to allocate for a memory pool.
RETURN:
    Returns pointer to the initialized fixed-size allocator.
INPUT:
    size_of_block: size of initialized blocks.
    number_of_blocks: number of blocks to be initialized.
    memory_pool: pointer to a memory pool.
TIME COMPLEXITY:
    O(n)
*/
fsa_t *FSAInit(size_t size_of_block, size_t number_of_blocks, void *memory_pool);


/*
DESCRIPTION:
    Allocates one block of memory out from provided fixed-size allocator.
	The Allocation may fail if there is no free block to allocate, in that
	case, the function returns a NULL pointer otherwise returns the pointer
	to the newly allocated block of memory.
RETURN:
    Returns the pointer to the newly allocated block of memory.
	NULL pointer in case of failure.
INPUT:
    fsa: pointer to the fixed-size allocator.
TIME COMPLEXITY:
    O(1)
*/
void *FSAAlloc(fsa_t *fsa);


/*
DESCRIPTION:
    Frees allocated block of memory. Provided block of memory should be
	previously allocated by a fixed-size allocator otherwise, the behavior
	is undefined.
RETURN:
    There is no return for this function.
INPUT:
    fsa: pointer to the fixed-size allocator.
	block_to_free: pointer to the block of memory.
TIME COMPLEXITY:
    O(1)
*/
void FSAFree(fsa_t *fsa, void *block_to_free);


/*
DESCRIPTION:
    Computes the needed amount of bytes to be allocated for a memory pool
	that is going to be used by a fixed-size allocator based on the passed
	parameters. block_size should be a number aligned with WORD_SIZE and is
	going to be aligned otherwise.
RETURN:
    Returns the computed number.
INPUT:
    number_of_blocks: needed amount of blocks.
    size_of_block: size of each block.
TIME COMPLEXITY:
    O(1)
*/
size_t FSASuggestSize(size_t numb_of_blocks, size_t block_size);


/*
DESCRIPTION:
    Computes the current amount of free blocks in a fixed-size allocator.
RETURN:
	Returns the computed number.
INPUT:
	fsa: pointer to the fixed-size allocator.
TIME COMPLEXITY:
    O(n)
*/
size_t FSACountFree(const fsa_t *fsa);

#endif /* __NSRD_FSA_H__ */