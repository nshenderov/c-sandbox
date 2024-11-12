/*******************************************************************************
*
* FILENAME : cbuffer.h
*
* DESCRIPTION : Circular buffer, circular queue, cyclic buffer or ring buffer
* is a data structure that uses a single, fixed-size buffer as if it were
* connected end-to-end. This structure lends itself easily to buffering data
* streams.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 01.03.2023
* 
*******************************************************************************/

#ifndef __NSRD_CBUFFER_H__
#define __NSRD_CBUFFER_H__

#include <stddef.h> /* size_t */

typedef struct cbuffer cbuffer_t;

/*
DESCRIPTION
	Create a new circular buffer.
	Creating circular buffer with 0 capacity will cause undefined behavior.
	User is responsible for memory deallocation.
RETURN
	Pointer to the created circular buffer.
	Or NULL if allocation failed.
INPUT
	capacity: number of bytes to allocate the circular buffer.
*/
cbuffer_t *CBufferCreate(size_t capacity);

/*
DESCRIPTION
	Destroys the circular buffer by freeing all the allocated memory.
	All remaining data will be lost.
	The user is responsible for handling dangling pointers.
RETURN
	There is no return for this function.
INPUT
	cbuffer: pointer to the circular buffer.
*/
void CBufferDestroy(cbuffer_t *cbuffer);

/*
DESCRIPTION
	Returns how many bytes are still availible for writing
	in a circular buffer.
RETURN
	Number of free bytes in the circular buffer.
INPUT
	cbuffer: pointer to the circular buffer.
*/
size_t CBufferFreeSpace(const cbuffer_t *cbuffer);

/*
DESCRIPTION
	Returns the capacity of the circular buffer.
RETURN
	Capacity of the circular buffer in bytes.
INPUT
	cbuffer: pointer to the circular buffer.
*/
size_t CBufferBufSize(const cbuffer_t *cbuffer);

/*
DESCRIPTION
	Checks wherther the circular buffer is empty.
RETURN
	1: is empty.
	0: is not empty.	
INPUT
	cbuffer: pointer to the circular buffer.
*/
int CBufferIsEmpty(const cbuffer_t *cbuffer);

/*
DESCRIPTION
	Writes bytes from src to the circular buffer.
RETURN
	Number of written bytes.
INPUT
	cbuffer: pointer to the circular buffer.
	scr: pointer to data that should be written to the buffer.
	count: number of bytes that should be written.
*/
size_t CBufferWrite(cbuffer_t *cbuffer, const void *src, size_t count);

/*
DESCRIPTION
	Reads bytes from the circular buffer to user's dest.
RETURN
	Number of read bytes.
INPUT
	cbuffer: pointer to the circular buffer.
	dest: pointer to data that should be read from the buffer.
	count: number of bytes that should be read.
*/
size_t CBufferRead(cbuffer_t *cbuffer, void *dest, size_t count);

#endif /* __NSRD_CBUFFER_H__ */