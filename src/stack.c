/*******************************************************************************
*
* FILENAME : stack.c
*
* DESCRIPTION : Stack is a linear data structure that follows the Last In,
* First Out (LIFO) principle. Elements are added and removed from the top of the
* stack only, with two primary operations: push (to add an element) and pop
* (to remove the top element). Stacks are used in applications like function
* call management, undo mechanisms, and expression evaluation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 02.04.2023
* 
* PUBLIC FUNCTIONS :
*		stack_t *StackCreate(size_t capacity, size_t elem_size); 
*		void StackDestroy(stack_t *stack);
*		void StackPush(stack_t *stack, const void *element);  
*		void *StackPeek(const stack_t *stack);
*		void StackPop(stack_t *stack);
*		size_t StackSize(const stack_t *stack);
*		int StackIsEmpty(const stack_t *stack);
*		size_t StackCapacity(const stack_t *stack);
*
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, realloc */
#include <string.h> /* memcpy, memset */

#include "stack.h"

struct stack
{
	size_t capacity;
	size_t elem_size;
	int top;
	void *elements;
};

stack_t *StackCreate(size_t capacity, size_t elem_size)
{
	stack_t *stack = NULL;
	size_t stack_size = sizeof(stack_t);

	stack = (stack_t *) malloc(stack_size + capacity * elem_size);
	stack -> capacity = capacity;
	stack -> elem_size = elem_size;
	stack -> top = -1;
	stack -> elements = stack + 1;

	return (stack);
}


void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);

	free(stack);
	stack = NULL;
}

void StackPush(stack_t *stack, const void *element)
{
	assert(NULL != stack);
	assert(NULL != element);

	stack -> top++;
	memcpy((char *)stack -> elements + (stack -> top * stack -> elem_size),
					element, stack -> elem_size);
}

size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);

	return (stack -> top + 1);
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);

	return ((char *)stack -> elements + (stack -> top *stack -> elem_size));
}

void StackPop(stack_t *stack)
{
	assert(NULL != stack);

	memset((char *)stack->elements + (stack -> top *stack -> elem_size),
					0, stack -> elem_size);
	stack -> top--;
}

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);

	return (0 > stack -> top);
}

size_t StackCapacity(const stack_t *stack)
{
	assert(NULL != stack);

	return (stack -> capacity);
}