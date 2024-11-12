/*******************************************************************************
*
* FILENAME : recursion_test.c
*
* DESCRIPTION : Recursive functions unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 03.06.2023
* 
*******************************************************************************/

#include <string.h> /* strlen, strcmp, strstr */

#include "recursion.h"
#include "testing.h"


static void TestFibonacci(void);
static void TestFlipList(void);
static void TestStrLen(void);
static void TestStrCmp(void);
static void TestStrCpy(void);
static void TestStrCat(void);
static void TestStrStr(void);
static void TestSortStack(void);

int main()
{
	TH_TEST_T tests[] = {
		{"Fibonacci", TestFibonacci},
		{"Flip list", TestFlipList},
		{"strlen", TestStrLen},
		{"strcmp", TestStrCmp},
		{"strcpy", TestStrCpy},
		{"strcat", TestStrCat},
		{"strstr", TestStrStr},
		{"Sort stack", TestSortStack},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestFibonacci(void)
{
	TH_ASSERT(13 == Fibonacci(7));
}

static void TestFlipList(void)
{
	node_t head = {0};
	node_t node1 = {0};
	node_t node2 = {0};
	node_t node3 = {0};
	node_t node4 = {0};
	node_t node5 = {0};

	head.next = &node1;
	node1.next = &node2;
	node2.next = &node3;
	node3.next = &node4;
	node4.next = &node5;
	node5.next = NULL;

	TH_ASSERT(&node5 == FlipList(&head));

	TH_ASSERT(node5.next == &node4);
	TH_ASSERT(node4.next == &node3);
	TH_ASSERT(node3.next == &node2);
	TH_ASSERT(node2.next == &node1);
	TH_ASSERT(node1.next == &head);
	TH_ASSERT(NULL == head.next);
}

static void TestStrLen(void)
{
	char *test_str1 = "HelloWorld";
	char *test_str2 = " !@#! AD AD";

	TH_ASSERT(10 == StrLen(test_str1));
	TH_ASSERT(strlen(test_str1) == StrLen(test_str1));

	TH_ASSERT(11 == StrLen(test_str2));
	TH_ASSERT(strlen(test_str2) == StrLen(test_str2));
}

static void TestStrCmp(void)
{
	char *test_str1 = "HelloWorld";
	char *test_str2 = "HelloWorld";
	char *test_str3 = "Hello2";

	TH_ASSERT(0 == StrCmp(test_str1, test_str2));
	TH_ASSERT(strcmp(test_str1, test_str2) == StrCmp(test_str1, test_str2));

	TH_ASSERT(0 < StrCmp(test_str2, test_str3));

	TH_ASSERT(0 < StrCmp(test_str2, test_str3));

	TH_ASSERT(0 > StrCmp(test_str3, test_str1));
	
}

static void TestStrCpy(void)
{
	char dest[16] = {0};

	char *test_str1 = "HelloWorld";

	TH_ASSERT(dest == StrCpy(dest, test_str1));
	TH_ASSERT(0 == strcmp(dest, test_str1));
}

static void TestStrCat(void)
{
	char dest[16] = "Hello";

	TH_ASSERT(dest == StrCat(dest, " World!"));
	TH_ASSERT(0 == strcmp(dest, "Hello World!"));
}

static void TestStrStr(void)
{
	char *test_str = "Hello World";

	char *needle1 = "Hello";
	char *needle2 = "rld";
	char *needle3 = "ld";

	TH_ASSERT(test_str == StrStr(test_str, needle1));
	TH_ASSERT(strstr(test_str, needle1) == StrStr(test_str, needle1));
	TH_ASSERT(strstr(test_str, needle2) == StrStr(test_str, needle2));
	TH_ASSERT(strstr(test_str, needle3) == StrStr(test_str, needle3));
	TH_ASSERT(strstr(test_str, "    ") == StrStr(test_str, "    "));
}

static void TestSortStack(void)
{
	stack_t *stack = StackCreate(5, sizeof(int));

	int test_nums[5] = {1,2,3,4,5};

	StackPush(stack, &test_nums[3]);
	StackPush(stack, &test_nums[0]);
	StackPush(stack, &test_nums[1]);
	StackPush(stack, &test_nums[4]);
	StackPush(stack, &test_nums[2]);

	TH_ASSERT(test_nums[2] == *(int *) StackPeek(stack));

	SortStack(stack);

	TH_ASSERT(test_nums[0] == *(int *) StackPeek(stack));
	StackPop(stack);
	TH_ASSERT(test_nums[1] == *(int *) StackPeek(stack));
	StackPop(stack);
	TH_ASSERT(test_nums[2] == *(int *) StackPeek(stack));
	StackPop(stack);
	TH_ASSERT(test_nums[3] == *(int *) StackPeek(stack));
	StackPop(stack);
	TH_ASSERT(test_nums[4] == *(int *) StackPeek(stack));
	StackPop(stack);

	StackDestroy(stack);
}