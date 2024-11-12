/*******************************************************************************
*
* FILENAME : queue.h
*
* DESCRIPTION : Queue implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 15.04.2023
* 
* PUBLIC FUNCTIONS :
*		queue_t *QueueCreate(void); 
*		void QueueDestroy(queue_t *queue); 
*		int QueueEnqueue(queue_t *queue, void *data); 
*		void QueueDequeue(queue_t *queue); 
*		void *QueuePeek(const queue_t *queue); 
*		int QueueIsEmpty(const queue_t *queue); 
*		size_t QueueSize(const queue_t *queue); 
*		queue_t *QueueAppend(queue_t *dest, queue_t *src); 
*
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "queue.h"
#include "slinkedlist.h"

enum {SUCCESS, FAILURE};

struct queue
{
    slist_t *slist;
};

#define FREE_MEMORY(ptr) \
{free(ptr); (ptr) = NULL;}

queue_t *QueueCreate(void)
{
	queue_t *new_queue = (queue_t *) malloc(sizeof(queue_t));
	if (NULL == new_queue)
	{
		return (NULL);
	}

	new_queue -> slist = SLinkedListCreate();
	if (NULL == new_queue -> slist)
	{
		FREE_MEMORY(new_queue);
		return (NULL);
	}

	return (new_queue);
}

void QueueDestroy(queue_t *queue)
{
	assert(NULL != queue);

	SLinkedListDestroy(queue -> slist);

	FREE_MEMORY(queue);
}

int QueueEnqueue(queue_t *queue, void *data)
{
	slist_iterator_t *queue_rear = NULL;
    slist_iterator_t *enqueued_elem = NULL;

	assert(NULL != queue);

	queue_rear = SlinkedListEnd(queue -> slist);
	enqueued_elem = SlinkedListInsert(queue_rear, data);
	queue_rear = SlinkedListEnd(queue -> slist);

	if (SlinkedListIsSameIterator(enqueued_elem, queue_rear))
	{
		return (FAILURE);
	}

	return (SUCCESS);
}

void QueueDequeue(queue_t *queue)
{
	assert(NULL != queue);

	SlinkedListRemove(SlinkedListBegin(queue -> slist));
}

void *QueuePeek(const queue_t *queue)
{
	assert(NULL != queue);

	return (SlinkedListGetData(SlinkedListBegin(queue -> slist)));
}

int QueueIsEmpty(const queue_t *queue)
{
	slist_iterator_t *queue_front = NULL;
	slist_iterator_t *queue_rear = NULL;

	assert(NULL != queue);

	queue_front = SlinkedListBegin(queue -> slist);
	queue_rear = SlinkedListEnd(queue -> slist);

	return (SlinkedListIsSameIterator(queue_front, queue_rear));
}

size_t QueueSize(const queue_t *queue)
{
	assert(NULL != queue);

	return (SlinkedListCount(queue -> slist));
}

queue_t *QueueAppend(queue_t *dest, queue_t *src)
{
	assert(NULL != dest);
	assert(NULL != src);

	SlinkedListAppend(dest -> slist, src -> slist);

	return (dest);
}