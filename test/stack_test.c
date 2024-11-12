/*******************************************************************************
*
* FILENAME : stack_test.c
*
* DESCRIPTION : Stack unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 02.04.2023
* 
*******************************************************************************/

#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "stack.h"
#include "testing.h"


static void TestStack(void);

int main()
{
	TH_TEST_T TESTS[] = {
        {"Stack", TestStack},
        TH_TESTS_ARRAY_END
    };

    TH_RUN_TESTS(TESTS);

	return 0;
}

static void TestStack(void)
{
	stack_t *stack = StackCreate(64, 8);

	int t1 = 123;
	float t2 = 99.123;
	double t3 = 0.0;

    TH_ASSERT(0 == StackSize(stack));
    TH_ASSERT(64 == StackCapacity(stack));
    TH_ASSERT(1 == StackIsEmpty(stack));
	
	StackPush(stack, (void *)&t1);
    TH_ASSERT(t1 == *(int *) StackPeek(stack));
	
	StackPush(stack, (void *)&t2);
	TH_ASSERT(t2 == *(float *) StackPeek(stack));

	StackPush(stack, (void *)&t3);
	TH_ASSERT(t3 == *(double *) StackPeek(stack));

	TH_ASSERT(3 == StackSize(stack));
	TH_ASSERT(0 == StackIsEmpty(stack));
	TH_ASSERT(64 == StackCapacity(stack));

	StackPop(stack);
	StackPop(stack);
	StackPop(stack);

	TH_ASSERT(0 == StackSize(stack));
	TH_ASSERT(1 == StackIsEmpty(stack));

	StackDestroy(stack);
}
