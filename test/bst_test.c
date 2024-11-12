/*******************************************************************************
*
* FILENAME : bst_test.c
*
* DESCRIPTION : Binary search tree unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 28.05.2023
* 
*******************************************************************************/

#include <stdio.h> /* printf() */
#include <stdlib.h> /* rand() */
#include <assert.h> /* assert */

#include "bst.h"
#include "testing.h"

#define N_ELEMS_TO_INSERT (10)

static int CompareDummy(const void *data1, const void *data2, void *params);
static int CompareInts(const void *data1, const void *data2, void *params);
static int AddOneToInt(void *data, void *params);
static void FillIntArray(int arr[], size_t size);
static void FillBST(bst_t *bst, int arr[], size_t size);
static void FillBSTBalancedTen(bst_t *bst, int arr[]);
static int FindMinInt(int arr[], size_t size);
static int FindMaxInt(int arr[], size_t size);
static void TestNotFound(void);
static void TestRemoveWithoutChildren(void);
static void TestRemoveWithLeftChild(void);
static void TestRemoveWithRightChild(void);
static void TestRemoveWithTwoChildren(void);
static void TestRemoveWithTwoChildrenExtra(void);


static void TestCreate(void);
static void TestIsEmpty(void);
static void TestSize(void);
static void TestIsSameIterator(void);
static void TestGetData(void);
static void TestBegin(void);
static void TestEnd(void);
static void TestPrev(void);
static void TestNext(void);
static void TestInsert(void);
static void TestFind(void);
static void TestRemove(void);
static void TestForeach(void);

int main()
{
    TH_TEST_T tests[] = {
		{"Create", TestCreate},
		{"IsEmpty", TestIsEmpty},
		{"Size", TestSize},
		{"IsSameIterator", TestIsSameIterator},
		{"GetData", TestGetData},
		{"Begin", TestBegin},
		{"End", TestEnd},
		{"Prev", TestPrev},
		{"Next", TestNext},
		{"Insert", TestInsert},
		{"Find", TestFind},
		{"Remove", TestRemove},
		{"Foreach", TestForeach},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestCreate(void)
{
    bst_t *bst = BSTCreate(CompareDummy, NULL);
	
    TH_ASSERT(NULL != bst);

	BSTDestroy(bst);
}

static void TestIsEmpty(void)
{
    bst_t *bst = BSTCreate(CompareDummy, NULL);

    TH_ASSERT(1 == BSTIsEmpty(bst));
	BSTInsert(bst, NULL);
    TH_ASSERT(0 == BSTIsEmpty(bst));

	BSTDestroy(bst);
}

static void TestSize(void)
{
    bst_t *bst = BSTCreate(CompareDummy, NULL);

    TH_ASSERT(0 == BSTSize(bst));
	BSTInsert(bst, NULL);
	TH_ASSERT(1 == BSTSize(bst));
	BSTInsert(bst, NULL);
	TH_ASSERT(2 == BSTSize(bst));

	BSTDestroy(bst);
}

static void TestIsSameIterator(void)
{
    bst_t *bst = BSTCreate(CompareDummy, NULL);

    TH_ASSERT(1 == BSTIsSameIterator(BSTBegin(bst), BSTEnd(bst)));
	BSTInsert(bst, NULL);
    TH_ASSERT(0 == BSTIsSameIterator(BSTBegin(bst), BSTEnd(bst)));

	BSTDestroy(bst);
}

static void TestGetData(void)
{
    int arr[2] = {15, 16};
	bst_t *bst = BSTCreate(CompareInts, NULL);

	BSTInsert(bst, &arr[0]);
    TH_ASSERT(arr[0] == *(int *)BSTGetData(BSTBegin(bst)));

	BSTDestroy(bst);	
}

static void TestBegin(void)
{
    int arr[N_ELEMS_TO_INSERT] = {0};
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillIntArray(arr, N_ELEMS_TO_INSERT);
    FillBST(bst, arr, N_ELEMS_TO_INSERT);

    TH_ASSERT(FindMinInt(arr, N_ELEMS_TO_INSERT) == *(int *)BSTGetData(BSTBegin(bst)));

	BSTDestroy(bst);
}

static void TestEnd(void)
{
    int arr[N_ELEMS_TO_INSERT] = {0};
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillIntArray(arr, N_ELEMS_TO_INSERT);
    FillBST(bst, arr, N_ELEMS_TO_INSERT);

    TH_ASSERT(FindMaxInt(arr, N_ELEMS_TO_INSERT) == *(int *)BSTGetData(BSTPrev(BSTEnd(bst))));

	BSTDestroy(bst);
}

static void TestPrev(void)
{
    int arr[N_ELEMS_TO_INSERT] = {0};
	int last_element = 0;
	int success = 1;
	size_t i = 0;
	bst_iter_t runner = NULL;
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillIntArray(arr, N_ELEMS_TO_INSERT);
	FillBST(bst, arr, N_ELEMS_TO_INSERT);

	runner = BSTPrev(BSTEnd(bst));
	last_element = *(int *)BSTGetData(runner);

	for(i = 1; i < N_ELEMS_TO_INSERT; ++i)
	{
		runner = BSTPrev(runner);

		if(last_element < *(int *)BSTGetData(runner))
		{
            success = 0;
            break;
		}

		last_element = *(int *)BSTGetData(runner);
	}

    TH_ASSERT(1 == success);

	BSTDestroy(bst);
}

static void TestNext(void)
{
    int arr[N_ELEMS_TO_INSERT] = {0};
	int last_element = 0;
    int success = 1;
	size_t i = 0;
	bst_iter_t runner = NULL;
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillIntArray(arr, N_ELEMS_TO_INSERT);
	FillBST(bst, arr, N_ELEMS_TO_INSERT);

	runner = BSTBegin(bst);
	last_element = *(int *)BSTGetData(runner);

	for(i = 1; i < N_ELEMS_TO_INSERT; ++i)
	{
		runner = BSTNext(runner);

		if(last_element > *(int *)BSTGetData(runner))
		{
			success = 0;
            break;
		}

		last_element = *(int *)BSTGetData(runner);
	}

    TH_ASSERT(1 == success);

	BSTDestroy(bst);
}

static void TestInsert(void)
{
	int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int success = 1;
	size_t i = 0;
	bst_iter_t runner = NULL;
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillBSTBalancedTen(bst, arr);

	runner = BSTBegin(bst);

	for(i = 0; i < 10; ++i)
	{
		if(*(int *)BSTGetData(runner) != arr[i])
		{
			success = 0;
            break;
		}

		runner = BSTNext(runner);
	}

	TH_ASSERT(1 == success);

    TestNotFound();

	BSTDestroy(bst);
}

static void TestFind(void)
{
    int arr[N_ELEMS_TO_INSERT] = {0};
    int success = 1;
	size_t i = 0;
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillIntArray(arr, N_ELEMS_TO_INSERT);
    FillBST(bst, arr, N_ELEMS_TO_INSERT);

	for(i = 0; i < N_ELEMS_TO_INSERT; ++i)
	{
		if(arr[i] != *(int *)BSTGetData(BSTFind(bst, &arr[i])))
		{
			success = 0;
            break;
		}
	}

    TH_ASSERT(1 == success);

	BSTDestroy(bst);
}

static void TestRemove(void)
{
    TestRemoveWithoutChildren();
	TestRemoveWithLeftChild();
	TestRemoveWithRightChild();
	TestRemoveWithTwoChildren();
	TestRemoveWithTwoChildrenExtra();
}

static void TestForeach(void)
{
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int i = 0;
	int success = 1;
	bst_iter_t runner = NULL;
	bst_t *bst = BSTCreate(CompareInts, NULL);

    FillBSTBalancedTen(bst, arr);

    TH_ASSERT(0 == BSTForeach(BSTBegin(bst), BSTEnd(bst), AddOneToInt, NULL));

	runner = BSTBegin(bst);

	for(i = 0; i < 10; ++i)
	{
		if((i + 1) != *(int *)BSTGetData(runner))
		{
			success = 0;
            break;
		}

		runner = BSTNext(runner);
	}

    TH_ASSERT(1 == success);

	BSTDestroy(bst);
}

static int CompareDummy(const void *data1, const void *data2, void *params)
{
	return (1);
	(void)data1;
	(void)data2;
	(void)params;
}

static int CompareInts(const void *data1, const void *data2, void *params)
{
	int result = 0;

    assert(NULL != data1);
    assert(NULL != data2);

	if(*(int *)data1 > *(int *)data2)
	{
		result = 1;
	}

	if(*(int *)data1 < *(int *)data2)
	{
		result = -1;
	}

	return (result);
	(void)params;
}

static int AddOneToInt(void *data, void *params)
{
    assert(NULL != data);

	*(int *)data += 1;

	return 0;
	(void)params;
}

static void FillIntArray(int arr[], size_t size)
{
    size_t i = 0;

    assert(NULL != arr);

    for(i = 0; i < size; ++i)
    {
        arr[i] = rand();
    }
}

static void FillBST(bst_t *bst, int arr[], size_t size)
{
	size_t i = 0;
	int success = 1;

    assert(NULL != bst);
    assert(NULL != arr);

	for(i = 0; i < size; ++i)
	{
		if(BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[i])))
		{
			success = 0;
            break;
		}
	}

    TH_ASSERT(1 == success);
}

static void FillBSTBalancedTen(bst_t *bst, int arr[])
{
    assert(NULL != bst);
    assert(NULL != arr);

	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[6])));
	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[3])));
	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[8])));
	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[1])));
	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[5])));
	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[7])));
	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[9])));
	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[0])));
	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[2])));
	TH_ASSERT(0 == BSTIsSameIterator(BSTEnd(bst), BSTInsert(bst, &arr[4])));
}

static int FindMinInt(int arr[], size_t size)
{
    int min = 0;
    size_t i = 0;

    assert(NULL != arr);

    min = arr[0];

    for(i = 1; i < size; ++i)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }

    return (min);
}

static int FindMaxInt(int arr[], size_t size)
{
    int max = 0;
    size_t i = 0;

    assert(NULL != arr);

    max = arr[0];

    for(i = 1; i < size; ++i)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }

    return (max);
}

static void TestNotFound(void)
{
	int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillBSTBalancedTen(bst, arr);
	BSTRemove(BSTFind(bst, &arr[7]));
    TH_ASSERT(1 == BSTIsSameIterator(BSTEnd(bst), BSTFind(bst, &arr[7])));

	BSTDestroy(bst);
}

static void TestRemoveWithoutChildren(void)
{
	int arr[2] = {0, -1};
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillBST(bst, arr, 2);

	BSTRemove(BSTFind(bst, &arr[1]));

    TH_ASSERT(arr[0] == *(int *)BSTGetData(BSTFind(bst, &arr[0])));

	BSTDestroy(bst);
}

static void TestRemoveWithLeftChild(void)
{
	int arr[2] = {0, -1};
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillBST(bst, arr, 2);

	BSTRemove(BSTFind(bst, &arr[0]));

	TH_ASSERT(arr[1] == *(int *)BSTGetData(BSTFind(bst, &arr[1])));

	BSTDestroy(bst);
}

static void TestRemoveWithRightChild(void)
{
	int arr[2] = {0, 1};
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillBST(bst, arr, 2);

	BSTRemove(BSTFind(bst, &arr[0]));

	TH_ASSERT(arr[1] == *(int *)BSTGetData(BSTFind(bst, &arr[1])));

	BSTDestroy(bst);
}

static void TestRemoveWithTwoChildren(void)
{
	int arr[3] = {0, -1, 1};
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillBST(bst, arr, 3);
	BSTRemove(BSTFind(bst, &arr[0]));

    TH_ASSERT(arr[1] == *(int *)BSTGetData(BSTFind(bst, &arr[1])));
    TH_ASSERT(arr[2] == *(int *)BSTGetData(BSTFind(bst, &arr[2])));

	BSTDestroy(bst);
}

static void TestRemoveWithTwoChildrenExtra(void)
{
	int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int success = 1;
	size_t i = 0;
	bst_iter_t runner = NULL;
	bst_t *bst = BSTCreate(CompareInts, NULL);

	FillBSTBalancedTen(bst, arr);

	BSTRemove(BSTFind(bst, &arr[7]));
	BSTRemove(BSTFind(bst, &arr[6]));

	runner = BSTBegin(bst);

	for(i = 0; i < 10; ++i)
	{
		if(6 == i || 7 == i)
		{
			continue;
		}

		if(*(int *)BSTGetData(runner) != arr[i])
		{
			success = 0;
            break;
		}

		runner = BSTNext(runner);
	}

    TH_ASSERT(1 == success);

	BSTDestroy(bst);
}
