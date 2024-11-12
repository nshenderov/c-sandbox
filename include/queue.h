/*******************************************************************************
*
* FILENAME : queue.h
*
* DESCRIPTION : Queue is a collection of entities that are maintained in a
* sequence and can be modified by the addition of entities at one end of the
* sequence and the removal of entities from the other end of the sequence.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 15.04.2023
* 
*******************************************************************************/

#ifndef __NSRD_QUEUE_H__
#define __NSRD_QUEUE_H__

#include <stddef.h> /* size_t */

typedef struct queue queue_t;

/*
DESCRIPTION
    Creates a new queue. User is responsible for memory deallocation.
RETURN
    queue_t * - pointer to the created queue.
    NULL - if allocation failed.
INPUT
    No input for this function.
TIME_COMPLEXITY
    O(1)
*/
queue_t *QueueCreate(void);

/*
DESCRIPTION
    Destroys the queue by freeing all the allocated memory.
    All remaining data will be lost. 
    The user is responsible for handling dangling pointers.
RETURN
    There is no return for this function.
INPUT
    queue: pointer to the queue.
TIME_COMPLEXITY
    O(n)
*/
void QueueDestroy(queue_t *queue);

/*
DESCRIPTION
    Inserts new element at the end of the queue. 
RETURN
    0: success.
    1: allocation fails.
INPUT
    queue: pointer to the queue.
    data: ponter to the data.
TIME_COMPLEXITY
    O(1)
*/
int QueueEnqueue(queue_t *queue, void *data);

/*
DESCRIPTION
    Removes and returns the first element of the queue.
    Dequeueing on the empty queue is undefined behavior.
RETURN
    There is no return for this function.
INPUT
    queue: pointer to the queue.
TIME_COMPLEXITY
    O(1)
*/
void QueueDequeue(queue_t *queue);

/*
DESCRIPTION
    Returns the pointer to the First Out element of the queue. 
    Peeking empty queue may cause undefined behavior.
RETURN
    Pointer to the element.
INPUT
    queue: pointer to the queue
TIME_COMPLEXITY
    O(1)
*/
void *QueuePeek(const queue_t *queue);

/*
DESCRIPTION
    Checks whether the queue is empty.
RETURN
    1: is empty.
	0: is not empty.
INPUT
    queue: pointer to the queue
TIME_COMPLEXITY
    O(1)
*/
int QueueIsEmpty(const queue_t *queue);

/*
DESCRIPTION
    Returns the number of elements in the queue.
RETURN
    Number of elements
INPUT
    queue: pointer to the queue
TIME_COMPLEXITY
    O(n)
*/
size_t QueueSize(const queue_t *queue);

/*
DESCRIPTION
    Adds queue src to the end of the queue dest. The last element of the src
    becomes the last element of the appended queue. The src becomes empty after
    the appendage.
RETURN
    Pointer to the appended queue
INPUT
    dest: pointer to the destination queue
    src: pointer to the source queue
TIME_COMPLEXITY
    O(1)
*/
queue_t *QueueAppend(queue_t *dest, queue_t *src);

#endif /* __NSRD_QUEUE_H__ */
