/*******************************************************************************
*
* FILENAME : pqueue_heap.c
*
* DESCRIPTION : Priority queue implementation using heap.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 07.05.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "pqueue_heap.h"

enum {SUCCESS, FAILURE};

struct pq
{
    heap_t *heap;
};

typedef struct param_wrapper
{
    pqueue_compare_func_t compare_func;
} param_wrapper_t;

static int CompareWrapper(const void *data1, const void *data2, void *params)
{
    param_wrapper_t *param_wrapper = params;

    assert(NULL != data1);
    assert(NULL != data2);
    assert(NULL != params);

    return (param_wrapper->compare_func(data1, data2));
}

pq_t *PQCreate(pqueue_compare_func_t compare)
{
    pq_t *new_hpqueue = NULL;
    heap_t *new_heap = NULL;

    static param_wrapper_t param_wrapper = {0};
    param_wrapper.compare_func = compare;

    assert(NULL != compare);

    new_hpqueue = (pq_t *) malloc(sizeof(pq_t));
    if (NULL == new_hpqueue)
    {
        return (NULL);
    }

    new_heap = HeapCreate(CompareWrapper, &param_wrapper);
    if (NULL == new_heap)
    {
        free(new_hpqueue);
        new_hpqueue = NULL;
        return (NULL);
    }

    new_hpqueue->heap = new_heap;

    return (new_hpqueue);
}

void PQDestroy(pq_t *hpqueue)
{
    assert(NULL != hpqueue);
    assert(NULL != hpqueue->heap);

    HeapDestroy(hpqueue->heap);
    hpqueue->heap = NULL;

    free(hpqueue);
    hpqueue = NULL;
}

int PQEnqueue(pq_t *hpqueue, void *data)
{
    assert(NULL != hpqueue);
    assert(NULL != hpqueue->heap);
    assert(NULL != data);

    if (HeapPush(hpqueue->heap, data))
    {
        return (FAILURE);
    }

    return (SUCCESS);
}

void *PQDequeue(pq_t *hpqueue)
{
    assert(NULL != hpqueue);
    assert(NULL != hpqueue->heap);

    return (HeapPop(hpqueue->heap));
}

void *PQPeek(const pq_t *hpqueue)
{
    assert(NULL != hpqueue);
    assert(NULL != hpqueue->heap);

    return (HeapPeek(hpqueue->heap));
}

int PQIsEmpty(const pq_t *hpqueue)
{
    assert(NULL != hpqueue);
    assert(NULL != hpqueue->heap);

    return (HeapIsEmpty(hpqueue->heap));
}

size_t PQSize(const pq_t *hpqueue)
{
    assert(NULL != hpqueue);
    assert(NULL != hpqueue->heap);
    
    return (HeapSize(hpqueue->heap)); 
}

void PQClear(pq_t *hpqueue)
{
    heap_t *heap = NULL;

    assert(NULL != hpqueue);
    assert(NULL != hpqueue->heap);

    heap = hpqueue->heap;

    while(!HeapIsEmpty(heap))
    {
        HeapPop(heap);
    }
}

void *PQErase(pq_t *hpqueue, pqueue_is_match_func_t is_match, void *param)
{
    assert(NULL != hpqueue);
    assert(NULL != hpqueue->heap);
    assert(NULL != is_match);
    assert(NULL != param);

    return (HeapRemove(hpqueue->heap, is_match, param));
}