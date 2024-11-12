/*******************************************************************************
*
* FILENAME : recursion.c
*
* DESCRIPTION : Functions recursive implementations.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 03.06.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */

#include "recursion.h"

static node_t *FlipListRecursion(node_t *curr, node_t *prev, node_t *head);
static void StrCpyRecursion(char *runner, const char *src);
static void StrCatRecursion(char *dest, const char *src);
static char *StrStrRecursion(const char *haystack,  const char *needle);

int Fibonacci(int element_index)
{
	if (1 >= element_index)
	{
		return (element_index);
	}

	return (Fibonacci(element_index - 1) + Fibonacci(element_index - 2));
}

node_t *FlipList(node_t *head)
{
	node_t *new_head = NULL;

	assert(NULL != head);

	new_head = FlipListRecursion(head, NULL, head);

	return (new_head);
}

size_t StrLen(const char *s)
{
	assert(NULL != s);

	if ('\0' == *s)
	{
		return (0);
	}

	return (1 + StrLen(s + 1));
}

int StrCmp(const char *s1, const char *s2)
{
	assert(NULL != s1);
	assert(NULL != s2);

	if (0 != *s1 - *s2 || '\0' == *s1 || '\0' == *s2)
	{
		return ((int) *s1 - (int) *s2);
	}

	return (StrCmp(s1+1, s2+1));
}

char *StrCpy(char *dest, const char *src)
{
	assert(NULL != dest);
	assert(NULL != src);

	StrCpyRecursion(dest, src);

	return (dest);
}

char *StrCat(char *dest, const char *src)
{
	char *runner = dest;

	assert(NULL != dest);
	assert(NULL != src);

	runner += StrLen(dest);

	StrCatRecursion(runner, src);

	return (dest);
}

char *StrStr(const char *haystack, const char *needle)
{
	assert(NULL != haystack);
	assert(NULL != needle);

	if (*haystack == *needle && NULL != StrStrRecursion(haystack, needle))
	{
		return ((char *) haystack);
	}
	else if ('\0' == *haystack)
	{
		return (NULL);
	}

	return (StrStr(haystack + 1, needle));

}

static void SortStackInsert(stack_t *stack, int num)
{
	int peeked_num = 0;

    if (StackIsEmpty(stack) || num < *(int *) StackPeek(stack))
    {
        StackPush(stack, &num);
        return;
    }
 
    peeked_num = *(int *) StackPeek(stack);
    StackPop(stack);

    SortStackInsert(stack, num);
 
    StackPush(stack, &peeked_num);
}

void SortStack(stack_t *stack)
{
	int peeked_num = 0;

	if (!StackIsEmpty(stack))
	{
		peeked_num = *(int *) StackPeek(stack);
		StackPop(stack);

		SortStack(stack);

		SortStackInsert(stack, peeked_num);
	}
}

static node_t *FlipListRecursion(node_t *curr, node_t *prev, node_t *head)
{
	node_t *next = NULL;

    if (NULL == curr->next)
    {
        head = curr;
        curr->next = prev;
        return (head);
    }

    next = curr->next;
    curr->next = prev;

    return (FlipListRecursion(next, curr, head));
}

static void StrCpyRecursion(char *dest, const char *src)
{
	assert(NULL != dest);
	assert(NULL != src);

	if ('\0' == *src)
	{
		return;
	}

	*dest = *src;

	StrCpyRecursion(dest + 1, src + 1);
}

static void StrCatRecursion(char *dest, const char *src)
{
	assert(NULL != dest);
	assert(NULL != src);

	if ('\0' == *src)
	{
		return;
	}

	*dest = *src;

	StrCatRecursion(dest + 1, src + 1);
}

static char *StrStrRecursion(const char *haystack,  const char *needle)
{
	assert(NULL != haystack);
	assert(NULL != needle);

	if ('\0' == *needle)
	{
		return ((char *) haystack);
	}

	if (*haystack != *needle || '\0' == *haystack)
	{
		return (NULL);
	}

	return (StrStrRecursion(haystack + 1, needle + 1));
}