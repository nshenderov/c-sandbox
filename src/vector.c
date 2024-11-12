/*******************************************************************************
*
* FILENAME : vector.c
*
* DESCRIPTION : Dynamic vector implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 04.04.2023
* 
* PUBLIC FUNCTIONS :
*		vector_t *VectorCreate(size_t capacity, size_t element_size);
*		void VectorDestroy(vector_t *vector);
*		size_t VectorSize(const vector_t *vector);
*		size_t VectorCapacity(const vector_t *vector);
*		void *VectorGetElement(const vector_t *vector, size_t index);
*		int VectorPushBack(vector_t *vector, const void *element);
*		void VectorPopBack(vector_t *vector);
*		int VectorShrink(vector_t *vector); 
*		int VectorReserve(vector_t *vector, size_t new_size); 
*
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, realloc */
#include <string.h> /* memcpy, memset */

#include "vector.h"

#define GROWTH_FACTOR (2)

#define GET_POINTER_TO_POSITION(vector, index) \
((char *) vector -> elements + index * vector -> element_size)

#define FREE_MEMORY(ptr) \
{free(ptr); (ptr) = NULL;}

struct dynamic_vector
{
	int idx_top;
    size_t capacity;
    size_t element_size;
	void* elements;
};

vector_t *VectorCreate(size_t capacity, size_t element_size)
{
	vector_t *vector = NULL;

	vector = (vector_t *) malloc(sizeof(vector_t));
	if (NULL == vector)
	{
		return NULL;
	}
	vector -> capacity = capacity;
	vector -> element_size = element_size;
	vector -> idx_top = -1;

	vector -> elements = malloc(capacity * element_size);
	if (NULL == vector -> elements)
	{
		FREE_MEMORY(vector);
		return NULL;
	}

	return (vector);
}

int VectorPushBack(vector_t *vector, const void *element)
{

	assert(NULL != vector);
	assert(NULL != element);

	if (VectorSize(vector) == VectorCapacity(vector)
		&& 0 != VectorReserve(vector, GROWTH_FACTOR * vector -> capacity))
	{
		return (1);
	}

	vector -> idx_top += 1;
	memcpy(GET_POINTER_TO_POSITION(vector, vector -> idx_top),
															element, vector -> element_size);

	return (0);
}

int VectorReserve(vector_t *vector, size_t new_size)
{
	void *tmp = NULL;

	assert(NULL != vector);

	tmp = realloc(vector -> elements, vector -> element_size  * new_size);
	if (NULL == tmp)
	{
		return (1);
	}
	vector -> capacity = new_size;
	vector -> elements = tmp;

	return (0);
}

int VectorShrink(vector_t *vector)
{
	size_t curr_size = 0;

	assert(NULL != vector);

	curr_size = VectorSize(vector);

	if (curr_size < vector -> capacity && 0 != VectorReserve(vector, curr_size))
	{
		return (1);
	}

	return (0);
}


void *VectorGetElement(const vector_t *vector, size_t index)
{
	assert(NULL != vector);
	assert(index < VectorSize(vector));

	return (GET_POINTER_TO_POSITION(vector, index));
}

void VectorPopBack(vector_t *vector)
{
	assert(NULL != vector);

	memset(GET_POINTER_TO_POSITION(vector, vector -> idx_top),
																		0, vector -> element_size);

	vector -> idx_top -= 1;
}

void VectorDestroy(vector_t *vector)
{
	assert(NULL != vector);
	assert(NULL != vector -> elements);

	FREE_MEMORY(vector -> elements);
	FREE_MEMORY(vector);
}

size_t VectorSize(const vector_t *vector)
{
	assert(NULL != vector);

	return (vector -> idx_top + 1);
}

size_t VectorCapacity(const vector_t *vector)
{
	assert(NULL != vector);

	return (vector -> capacity);
}
