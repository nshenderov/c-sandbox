/*******************************************************************************
*
* FILENAME : stack.h
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
*******************************************************************************/

#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /* size_t */

typedef struct stack stack_t;

/*
DESCRIPTION
	Creates stack of the defined capacity and element size 
	and allocates memory. Function may fail during memory
	allocation. User is responsible mamory deallocation.
RETURN
	Function returns pointer to the stack or NULL if it fails to allocate data.
INPUT
    capacity: number of elements in the stack.
    elem_size: size of a single element in bytes.
*/
stack_t *StackCreate(size_t capacity, size_t elem_size); 

/*
DESCRIPTION
	Frees the memory allocated for the stack.
RETURN
	There is no return for this function.
INPUT
    stack: pointer to the stack.
*/
void StackDestroy(stack_t *stack);


/*
DESCRIPTION
	Removes the element from the top of the stack. Popping an empty stack is
    undefined behavior.
RETURN
	There is no return for this function.
INPUT
	stack: pointer to the stack.
*/
void StackPop(stack_t *stack);


/*
DESCRIPTION
	Adds an element to the top of the stack. The stack capaicity should not 
	be exceeded, otherwise undefined behavior is to be expected.
RETURN
	There is no return for this function.
INPUT
	stack: pointer to the stack.
	element: pointer to the element.
*/
void StackPush(stack_t *stack, const void *element);  

/*
DESCRIPTION
	Returns the topmost element of the stack. Peeking at empty stack is
    undefined behavior.
RETURN
	Pointer to the element.
INPUT
	stack: pointer to the stack.	
*/
void *StackPeek(const stack_t *stack);

/*
DESCRIPTION
	Function returns the number of elements in the stack.
RETURN
	Number of elements in the stack.
INPUT
	stack: pointer to the stack.
*/
size_t StackSize(const stack_t *stack);

/*
DESCRIPTION
	Checks whether the stack is empty.
RETURN
    1: is empty.
	0: is not empty.
INPUT
	stack: pointer to the stack.   	
*/
int StackIsEmpty(const stack_t *stack);

/*
DESCRIPTION
	Returns the capacity of the stack.
RETURN
    Number of elements the stack can store.
INPUT
	stack: pointer to the stack.	
*/
size_t StackCapacity(const stack_t *stack);

#endif /* __STACK_H__ */