/*******************************************************************************
*
* FILENAME : heap.c
*
* DESCRIPTION : Heap (MAX) implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 12.06.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "vector.h"
#include "heap.h"

#define DEFAULT_VECTOR_SIZE (2)

#define UNPACK(POINTER) (*(void **) POINTER)
#define GET_LAST_INDEX(HEAP) (VectorSize(HEAP->heap) - 1)
#define GET_RIGHT_CHILD_INDEX(INDEX) (index * 2 + 2)
#define GET_LEFT_CHILD_INDEX(INDEX) (index * 2 + 1)

#define HAS_RIGHT_CHILD(INDEX, HEAP) \
(index * 2 + 2 <= (int) GET_LAST_INDEX(HEAP))

#define HAS_LEFT_CHILD(INDEX, HEAP) \
(index * 2 + 1 <= (int) GET_LAST_INDEX(HEAP))

#define GET_RIGHT_CHILD(INDEX, HEAP) \
(VectorGetElement(HEAP->heap, GET_RIGHT_CHILD_INDEX(INDEX)))

#define GET_LEFT_CHILD(INDEX, HEAP) \
(VectorGetElement(HEAP->heap, GET_LEFT_CHILD_INDEX(INDEX)))

typedef enum {
	SECOND_IS_BIGGER = -1,
	EQUALS,
	FIRST_IS_BIGGER
} comparison_res_t;

struct heap
{
    vector_t *heap;
    heap_compare_t cmp;
    void *params;
};

static void Swap(void **p1, void **p2);
static void HeapifyUp(heap_t *heap);
static void HeapifyDown(heap_t *heap);
static int CompareChildren(int index, heap_t *heap);
static int CompareToParent(int index, int child_index, heap_t *heap);
static int FindElement(heap_t *heap, heap_is_match_t is_match, void *param);
static void ShiftElementsLeft(heap_t *heap, int index_from);


heap_t *HeapCreate(heap_compare_t compare, void *params)
{
	heap_t *new_heap = NULL;
	vector_t *new_vector = NULL;

	assert(NULL != compare);

	new_heap = (heap_t *) malloc(sizeof(struct heap));
	if (NULL == new_heap)
	{
		return (NULL);
	}

	new_vector = VectorCreate(DEFAULT_VECTOR_SIZE, sizeof(void *));
	if (NULL == new_vector)
	{
		free(new_heap);
		new_heap = NULL;
		return (NULL);
	}

	new_heap->cmp = compare;
	new_heap->params = params;
	new_heap->heap = new_vector;

	return (new_heap);
}

void HeapDestroy(heap_t *heap)
{
	assert(NULL != heap);

	VectorDestroy(heap->heap);
	heap->heap = NULL;

	free(heap);
	heap = NULL;
}

int HeapPush(heap_t *heap, void *data)
{
	vector_t *vector = NULL;

	assert(NULL != heap);
	assert(NULL != data);

	vector = heap->heap;

	if (VectorPushBack(vector, &data))
	{
		return (1);
	}

	HeapifyUp(heap);

	return (0);
}

void *HeapPop(heap_t *heap)
{
	vector_t *vector = NULL;
	void *pointer_to_return = NULL;
	void *element_first = NULL;
	void *element_last = NULL;
	size_t curr_size = 0;

	assert(NULL != heap);

	vector = heap->heap;
	curr_size = VectorSize(vector);
	pointer_to_return = NULL;

	if (1 == curr_size)
	{
		pointer_to_return = UNPACK(VectorGetElement(vector, 0));
		VectorPopBack(vector);
		return (pointer_to_return);
	}

	element_first = VectorGetElement(vector, 0);
	element_last = VectorGetElement(vector, curr_size - 1);

	pointer_to_return = UNPACK(element_first);

	Swap(element_first, element_last);

	VectorPopBack(vector);

	HeapifyDown(heap);

	return (pointer_to_return);
}

void *HeapRemove(heap_t *heap, heap_is_match_t is_match, void *param)
{
	vector_t *vector = NULL;
	void *pointer_to_return = NULL;
	void *founded_element = NULL;
	int founded_index = 0;

	assert(NULL != heap);
	assert(NULL != is_match);

	vector = heap->heap;

	founded_index = FindElement(heap, is_match, param);
	if (-1 == founded_index)
	{
		return (NULL);
	}

	founded_element = VectorGetElement(vector, founded_index);

	pointer_to_return = UNPACK(founded_element);

	Swap(founded_element, VectorGetElement(vector, VectorSize(vector) - 1));

	VectorPopBack(vector);

	ShiftElementsLeft(heap, founded_index);

	return (pointer_to_return);
}

void *HeapPeek(const heap_t *heap)
{
	assert(NULL != heap);
	assert(0 != VectorSize(heap->heap));

	return (UNPACK(VectorGetElement(heap->heap, 0)));
}

int HeapIsEmpty(const heap_t *heap)
{
	assert(NULL != heap);

	return(0 == VectorSize(heap->heap));
}

size_t HeapSize(const heap_t *heap)
{
	assert(NULL != heap);

	return(VectorSize(heap->heap));
}

static void HeapifyUp(heap_t *heap)
{
	int index = 0;
	vector_t *vector = NULL;

	assert(NULL != heap);

	vector = heap->heap;
	index = VectorSize(vector) - 1;

	while(heap->cmp(UNPACK(VectorGetElement(vector,index / 2)),
					UNPACK(VectorGetElement(vector, index)), heap->params))
	{
		Swap(VectorGetElement(vector,index / 2),VectorGetElement(vector,index));

		index /= 2;
	}
}

static void HeapifyDown(heap_t *heap)
{
	int index = 0;
	int child_index = 0;
	int swapped = 1;

	assert(NULL != heap);

	while(1 == swapped)
	{
		swapped = 0;

		child_index = CompareChildren(index, heap);

		swapped = CompareToParent(index, child_index, heap);

		index = child_index;
	}
}

static int CompareChildren(int index, heap_t *heap)
{
	assert(NULL != heap);

	if (HAS_RIGHT_CHILD(index, heap))
	{
		if (FIRST_IS_BIGGER == heap->cmp(UNPACK(GET_LEFT_CHILD(index, heap)),
							UNPACK(GET_RIGHT_CHILD(index, heap)), heap->params))
		{
			return (GET_RIGHT_CHILD_INDEX(index));
		}
		else
		{
			return (GET_LEFT_CHILD_INDEX(index));
		}
	}
	else if (HAS_LEFT_CHILD(index, heap))
	{
		return (GET_LEFT_CHILD_INDEX(index));
	}

	return (0);
}

static int CompareToParent(int index, int child_index, heap_t *heap)
{
	void *parent = NULL;
	void *child = NULL;
	vector_t *vector = NULL;
	heap_compare_t compare = NULL;

	assert(NULL != heap);

	vector = heap->heap;
	compare = heap->cmp;

	parent = VectorGetElement(vector, index);
	child = VectorGetElement(vector, child_index);

	if (0 != child_index && compare(UNPACK(parent),UNPACK(child), heap->params))
	{
		Swap(parent, child);
		return (1);
	}

	return (0);
}

static int FindElement(heap_t *heap, heap_is_match_t is_match, void *param)
{
	int index_runner = 0;
	int match = 0;
	int index = 0;
	vector_t *vector = NULL;

	assert(NULL != heap);
	assert(NULL != is_match);

	vector = heap->heap;
	index = VectorSize(vector) - 1;

	match = is_match(UNPACK(VectorGetElement(vector, index_runner)), param);

	while(index_runner < index && !match)
	{
		++index_runner;
		match = is_match(UNPACK(VectorGetElement(vector, index_runner)), param);
	}

	return (match ? index_runner : -1);
}

static void ShiftElementsLeft(heap_t *heap, int index_from)
{
	int last_index = 0;
	vector_t *vector = NULL;

	assert(NULL != heap);

	vector = heap->heap;
	last_index = VectorSize(vector) - 1;

	while(index_from < last_index)
	{
		void *next_elem = VectorGetElement(vector, index_from + 1);
		Swap(VectorGetElement(vector, index_from), next_elem);
		++index_from;
	}
}

static void Swap(void **p1, void **p2)
{
	void *tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}