/*******************************************************************************
*
*  FILENAME : heap.h
*
* DESCRIPTION : Heap is a specialized tree-based data structure that satisfies
* the heap property. It is often implemented as a binary heap. A binary heap is
* a complete binary tree where each node satisfies the heap property. The heap
* property states that for every node i in the heap, the value of i is either
* greater than or equal to (in a max heap) or less than or equal to
* (in a min heap) the values of its children nodes. This is MAX heap API.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 12.06.2023
* 
*******************************************************************************/

#ifndef __NSRD_HEAP_H__
#define __NSRD_HEAP_H__

#include <stddef.h> /* size_t */

typedef struct heap heap_t;

/*
DESCRIPTION
    Pointer to the user's function that compares data1 and data2.
    The actual matching and types of the input are defined by the user.
    Elements will be arranged in the Heap based on this function.
RETURN
    Positive: data1 > data2.
    Negative: data1 < data2.
    0: data1 == data2.
INPUT
    data1: pointer to user's data.
    data2: pointer to user's data.
    params: pointer to the additional parameter.

*/
typedef int (*heap_compare_t)(const void *data1, const void *data2, void* params);

/*
DESCRIPTION
    Pointer to the user's function that compares if the data matches a certain
    criteria, using the param. The actual matching and types of the input are
    defined by the user.
RETURN
    1: matches.
    0: not matches.
INPUT
    data: pointer to the user's data.
    param: pointer to the additional parameter for matching.
*/
typedef int (*heap_is_match_t)(const void *data, void *param);

/*
DESCRIPTION
    Creates new heap. Heap use pattern of sorting, based on user's compare
    function - "compare". Heap stores data by reference. Creation may fail, due
    to memory allocation fail. User is responsible for memory deallocation.
RETURN
    Pointer to the created heap on success.
    NULL if allocation failed.
INPUT
    compare: pointer to the compare function.
    params: pointer to the additional parameter.
TIME COMPLEXITY:
        O(1)
*/
heap_t *HeapCreate(heap_compare_t compare, void* params);

/*
DESCRIPTION
    Frees the memory allocated for each element of a heap and
    the heap by itself. All data inside the heap will be lost.
    User is responsible for handling dangling pointers after destroy.
RETURN
    Doesn't return anything.
INPUT
    heap: pointer to the heap to be destroyed.
TIME COMPLEXITY:
    O(1)
*/
void HeapDestroy(heap_t *heap);

/*
DESCRIPTION
    Pushes new element to the heap on the position, according to saved in the
    heap pointer on compare function. Push may fail, due to 
    allocation fail.
RETURN
    0: success.
    1: fail.
INPUT
    heap: pointer to the heap.
    data: pointer to the user's data.
TIME COMPLEXITY:
    O(log n), worst - O(n)
*/
int HeapPush(heap_t *heap, void *data);

/*
DESCRIPTION
    Removes the (MAX) element from passed heap.
    Trying to pop empty heap may cause undefined behavior.
RETURN
    Pointer to user's data of removed element.
INPUT
    heap: pointer to the heap.
TIME COMPLEXITY:
    O(log n)
*/
void *HeapPop(heap_t *heap);

/*
DESCRIPTION
    Get data from (MAX) element.
    Data is stored in heap by reference.
    Trying to peek empty heap may cause undefined behavior. 
RETURN
    Pointer to user's data of (MAX) element.
INPUT
    heap: pointer to the heap.
TIME COMPLEXITY:
    O(1)
*/
void *HeapPeek(const heap_t *heap);

/*
DESCRIPTION
    Checks if heap is empty.
RETURN
    1: is empty.
    0: is not empty.
INPUT
    heap: pointer to the heap.
TIME COMPLEXITY:
    O(1)
*/
int HeapIsEmpty(const heap_t *heap);

/*
DESCRIPTION
    Returns number of elemenents that are currently in the heap.
RETURN
    Number of elements in heap.
INPUT
    heap: pointer to the heap.
TIME COMPLEXITY:
    O(1)
*/
size_t HeapSize(const heap_t *heap);

/*
DESCRIPTION
	Traverses the heap for element that satisfies is_match 
	function's criteria. Removes the first occurance.
RETURN
	Pointer to user's data of removed element. on success.
	NULL if nothing found (user's data also might be NULL, so need to
	check HeapSize to see if element was found and removed).
INPUT
    heap: pointer to the heap.
    is_match: user's function that checks if the criteria is matched.
    param: parameter for the is_match function.
TIME COMPLEXITY:
    O(n)
*/
void *HeapRemove(heap_t *heap, heap_is_match_t is_match, void *param);

#endif /* __NSRD_HEAP_H__ */    