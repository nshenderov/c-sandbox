/*******************************************************************************
*
* FILENAME : vsa.h
*
* DESCRIPTION : VSA (Variable-Size Allocator) is a memory management technique
* that allows for the allocation and deallocation of blocks of varying sizes. It
* is more flexible than FSA (Fixed-Size Allocator).
* 
* AUTHOR : Nick Shenderov
*
* DATE : 17.05.2023
* 
*******************************************************************************/

#ifndef __NSRD_VSA_H__
#define __NSRD_VSA_H__

#include <stddef.h>


typedef struct block_header vsa_t;


/*
DESCRIPTION
    Initializes a variable-size allocator. User must provide pointer to the
    aligned pool of the memory and specify its size. memory_pool and pool_size
    must be aligned to the word size.
RETURN
    Returns pointer to the initialized variable-size allocator.
INPUT
    pool_size: size of the provided pool.
    memory_pool: pointer to the memory pool.
TIME COMPLEXITY
    O(1)
*/
vsa_t *VSAInit(size_t pool_size, void *memory_pool);

/*
DESCRIPTION
    Allocates a block of memory of the variable size allocator vsa at the first
    found place, which has enough space to accommodate block_size bytes in a
    contiguous manner. 
    It is recomended to use VSALargestChunkAvailable to determine the maximum
    size of a block that can be allocated taking into accound the current state
    of the vsa.
    One of the features of the function is the usage of the defragmentation
    mechanism.
RETURN
    Returns the pointer to the beggining of the allocated block of memory.
	NULL pointer in case of failure.
INPUT
    vsa: pointer to the variable-size allocator.
    bytes: size of the block to be allocated.
TIME COMPLEXITY
    O(n)
*/
void *VSAAlloc(vsa_t *vsa, size_t bytes);

/*
DESCRIPTION:
    Deallocates the block of memory block_to_free. Provided block of memory
    should previously be allocated by a variable-size allocator otherwise, the
	behavior is undefined.
RETURN:
    There is no return for this function.
INPUT:
	block_to_free: pointer to the beginning of the block.
TIME COMPLEXITY:
    O(1)
*/
void VSAFree(void *block_to_free);

/*
DESCRIPTION:
    Determines the maximum size of a block that can be allocated using the vsa
    taking into accound the current state of the vsa.
    One of the features of the function is the usage of the defragmentation
    mechanism.
RETURN:
     Returns the computed number.
INPUT:
    vsa: pointer to the variable-size allocator.
TIME COMPLEXITY:
    O(n)
*/
size_t VSALargestChunkAvailable(vsa_t *vsa);

#endif /* __NSRD_VSA_H__ */