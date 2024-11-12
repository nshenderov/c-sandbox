/*******************************************************************************
*
* FILENAME : vector_test.c
*
* DESCRIPTION : Dynamic vector unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 04.04.2023
* 
*******************************************************************************/

#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "vector.h"
#include "testing.h"


static void TestVector(void);

int main()
{
    TH_TEST_T TESTS[] = {
        {"Vector", TestVector},
        TH_TESTS_ARRAY_END
    };

    TH_RUN_TESTS(TESTS);

	return 0;
}

static void TestVector(void)
{
	vector_t *vector = VectorCreate(64, 8);

	int t1 = 123;
	float t2 = 99.123;
	double t3 = 0.0;
	
	VectorPushBack(vector, (void *)&t1);
    TH_ASSERT(t1 == *(int *) VectorGetElement(vector, 0));
	
	VectorPushBack(vector, (void *)&t2);
	TH_ASSERT(t2 == *(float *) VectorGetElement(vector, 1));

	VectorPushBack(vector, (void *)&t3);
	TH_ASSERT(t3 == *(double *) VectorGetElement(vector, 2));

    TH_ASSERT(3 == VectorSize(vector));
    TH_ASSERT(64 == VectorCapacity(vector));

	VectorPopBack(vector);
	VectorPopBack(vector);

    TH_ASSERT(1 == VectorSize(vector));

    TH_ASSERT(0 == VectorShrink(vector));

    TH_ASSERT(1 == VectorCapacity(vector));

	VectorPushBack(vector, (void *)&t1);
	VectorPushBack(vector, (void *)&t1);
	VectorPushBack(vector, (void *)&t1);
	TH_ASSERT(4 == VectorCapacity(vector));
    TH_ASSERT(t1 == *(int *) VectorGetElement(vector, 0));
    TH_ASSERT(t1 == *(int *) VectorGetElement(vector, 3));

	VectorPushBack(vector, (void *)&t1);
	VectorPushBack(vector, (void *)&t1);
	VectorPushBack(vector, (void *)&t1);
	TH_ASSERT(8 == VectorCapacity(vector));
    TH_ASSERT(t1 == *(int *) VectorGetElement(vector, 0));
    TH_ASSERT(t1 == *(int *) VectorGetElement(vector, 6));

	VectorDestroy(vector);
}