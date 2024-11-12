/*******************************************************************************
*
* FILENAME : recursion.h
*
* DESCRIPTION : Recursion is a programming concept where a function calls
* itself during its execution. It involves solving a complex problem by breaking
* it down into smaller, simpler instances of the same problem.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 03.06.2023
* 
*******************************************************************************/

#ifndef __NSRD_RECURSION_H__
#define __NSRD_RECURSION_H__

#include <stddef.h>

#include "stack.h"

typedef struct node
{
	void *data;
	struct node *next;
} node_t;


/*
DESCRIPTION
	Takes an integer index and returns the Fibonacci number at that index.
    The Fibonacci sequence starts with 1 and 1, and each subsequent number is
    the sum of the two preceding numbers. The index should be a non-negative
    integer. Passing element_index less 1 may cause undefined behavior.
	This function implemented using recursion. Passing huge element_index
	may cause stack overflow.
RETURN
	The Fibonacci number at the given index.
INPUT
	element_index: the index of the Fibonacci number to calculate.
*/
int Fibonacci(int element_index);

/*
DESCRIPTION
	Takes the head of a singly linked list and recursively reverses the order
    of the nodes in the list.
RETURN
	The new head of the reversed list.
INPUT
	node: pointer to the head of the singly linked list.
*/
node_t *FlipList(node_t *head);

/*
DESCRIPTION
	Takes the stack of int values and recursively sorts its elements in
    ascending order. The elements in the stack are rearranged such that the
    smallest element is at the bottom of the stack, while the largest element is
    at the top.
RETURN
    This function does not return anything.
INPUT
	stack: pointer to the stack.
*/
void SortStack(stack_t *stack);

/*
DESCRIPTION
	Copies the string pointed to by src, including the terminating null byte
	('\0'), to the buffer pointed to by dest.
	The strings may not overlap, and the destination string dest must be
	large enough to receive the copy.
RETURN
	Returns pointer to the beginning of dest string.
INPUT
    dest: pointer to the destination buffer.
    src: pointer to the source string.
*/
char *StrCpy(char *dest, const char *src);

/*
DESCRIPTION
	Appends the src string to the dest string, overwriting the terminating null
	byte ('\0') at the end of dest,  and  then adds  a  terminating  null  byte.
	The strings may not overlap, and the dest string must have enough space for
	the result. If dest is not large enough, program behavior is unpredictable.
RETURN
	Pointer to the resulting string dest.
INPUT
	dest: destination string.
    src: source string.
*/
char *StrCat(char *dest, const char *src);

/*
DESCRIPTION
	Finds the first occurrence of the substring needle in the string haystack.
	The terminating null bytes ('\0') are not compared.
RETURN
	These functions return pointer to the beginning of the located sub‐
	string, or NULL if the substring is not found.
INPUT
	haystack: pointer to the string where to search for the needle.
    needle: pointer to the string to search for.
*/
char *StrStr(const char *haystack, const char *needle);

/*
DESCRIPTION
	Takes pointer to the null-terminated string and recursively
	calculates its length. The length is defined as the number of characters
	in the string before the null terminator ('\0') is encountered.
RETURN
	The length of the string.
INPUT
	s: pointer to the null-terminated string.
*/
size_t StrLen(const char *s);

/*
DESCRIPTION:
	Compares two null-terminated strings, s1 and s2, and returns an integer
    value based on their comparison. The return value indicates the
    relationship between the strings, similar to the standard library strcmp
    function.
RETURN:
	Negative: s1 is lexicographically less than s2.
	Positive: s1 is lexicographically greater than s2.
	0: s1 and s2 are equal.
INPUT:
	s1: pointer to the first null-terminated string.
	s2: pointer to the second null-terminated string.
*/
int StrCmp(const char *s1, const char *s2);

#endif /* __NSRD_RECURSION_H__ */