/*******************************************************************************
* FILENAME : cbuffer.c
*
* DESCRIPTION : Circular buffer implementation.
*
* AUTHOR : Nick Shenderov
*
* DATE : 01.03.2023
*
* PUBLIC FUNCTIONS :
*		cbuffer_t *CBufferCreate(size_t bufsiz);
*		void CBufferDestroy(cbuffer_t *cbuffer);
*		size_t CBufferFreeSpace(const cbuffer_t *cbuffer);
*		size_t CBufferBufSize(const cbuffer_t *cbuffer);
*		int CBufferIsEmpty(const cbuffer_t *cbuffer);
*		size_t CBufferWrite(cbuffer_t *cbuffer, const void *src, size_t count);
*		size_t CBufferRead(cbuffer_t *cbuffer, void *dest, size_t count);
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <stddef.h> /* size_t, NULL */
#include <stdlib.h> /* malloc */

#include "cbuffer.h"

#define GAP_BYTE (1)

#define GET_DISTANCE_TO_END(pointer, cbuffer) \
((cbuffer -> buffer + cbuffer -> capacity + GAP_BYTE) - pointer)

#define FREE_MEMORY(ptr) \
{free(ptr); (ptr) = NULL;}

struct cbuffer
{
    size_t capacity;
    char *read;
    char *write;
    char buffer[1];
};

cbuffer_t *CBufferCreate(size_t capacity)
{
	cbuffer_t *new_cbuffer = NULL;
	size_t padding = sizeof(struct cbuffer) - offsetof(struct cbuffer, buffer);
	size_t buffer_size = 0;

	assert(0 != capacity);

	buffer_size = (capacity < padding) ? 0 : capacity - padding + GAP_BYTE;

	new_cbuffer = \
	(cbuffer_t *) malloc((sizeof(struct cbuffer) + buffer_size) * sizeof(char));

	if (NULL == new_cbuffer)
	{
		return (NULL);
	}

	new_cbuffer -> capacity = capacity;
	new_cbuffer -> write = new_cbuffer -> buffer;
	new_cbuffer -> read = new_cbuffer -> buffer;

	return (new_cbuffer);
}

void CBufferDestroy(cbuffer_t *cbuffer)
{
	assert(NULL != cbuffer);
	FREE_MEMORY(cbuffer);
}

size_t CBufferWrite(cbuffer_t *cbuffer, const void *src, size_t count)
{
	size_t total = 0;
	size_t freespace = 0;
	size_t freespace_to_end = 0;
	char *runner_src = (char *) src;

	assert(NULL != src);
	assert(NULL != cbuffer);

	freespace = CBufferFreeSpace(cbuffer);

	freespace_to_end = \
	GET_DISTANCE_TO_END(cbuffer -> write, cbuffer);

	if (count > freespace)
    {
        count = freespace;
    }

	if (count > freespace_to_end)
	{
		memcpy(cbuffer -> write, runner_src, freespace_to_end);
		total += freespace_to_end;
		cbuffer -> write = cbuffer -> buffer;
		runner_src += total;
	}
	
	memcpy(cbuffer -> write, runner_src, count - total);
	cbuffer -> write += (count - total);
	total += (count - total);	

	return (total);
}

size_t CBufferRead(cbuffer_t *cbuffer, void *dest, size_t count)
{
	size_t total = 0;
	size_t distance_to_end = 0;
	size_t current_size = 0;
	char *runner_dest = (char *) dest;

	assert(NULL != dest);
	assert(NULL != cbuffer);

	distance_to_end = GET_DISTANCE_TO_END(cbuffer -> read, cbuffer);
	current_size = CBufferBufSize(cbuffer) - CBufferFreeSpace(cbuffer);

	if (count > current_size)
    {
        count = current_size;
    }

    if (count > distance_to_end)
	{
		memcpy(runner_dest, cbuffer -> read, distance_to_end);
		cbuffer -> read = cbuffer -> buffer;
		total += distance_to_end;
		runner_dest += distance_to_end;
	}

	memcpy(runner_dest, cbuffer -> read, count - total);
	cbuffer -> read += (count - total);
	total += (count - total);	

	return (total);
}

size_t CBufferFreeSpace(const cbuffer_t *cbuffer)
{
	assert(NULL != cbuffer);

	if (cbuffer -> write < cbuffer -> read)
	{
		return ((cbuffer -> read - cbuffer -> write) - 1);
	}

	return (cbuffer -> capacity - (cbuffer -> write - cbuffer -> read));
}

size_t CBufferBufSize(const cbuffer_t *cbuffer)
{
	assert(NULL != cbuffer);

	return (cbuffer -> capacity);
}

int CBufferIsEmpty(const cbuffer_t *cbuffer)
{
	assert(NULL != cbuffer);

	return (cbuffer -> read == cbuffer -> write);
}