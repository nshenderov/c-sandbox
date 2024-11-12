/*******************************************************************************
*
*  FILENAME : heap_test.c
*
* DESCRIPTION : Heap unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 12.06.2023
* 
*******************************************************************************/

#include "heap.h"
#include "testing.h"


static int CompareInts(const void *data1, const void *data2, void *params);
static int IsMatchInt(const void *data1, void *param);


static void TestHeapCreate(void);
static void TestHeapPush(void);
static void TestHeapPop(void);
static void TestHeapPeek(void);
static void TestHeapIsEmpty(void);
static void TestHeapRemove(void);
static void TestHeapSize(void);

int main()
{
	TH_TEST_T tests[] = {
		{"Create", TestHeapCreate},
		{"IsEmpty", TestHeapIsEmpty},
		{"Size", TestHeapSize},
		{"Push", TestHeapPush},
		{"Peek", TestHeapPeek},
		{"Pop", TestHeapPop},
		{"Remove", TestHeapRemove},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestHeapCreate(void)
{
	heap_t *heap = HeapCreate(CompareInts, NULL);

	TH_ASSERT(NULL != heap);

	HeapDestroy(heap);
}

static void TestHeapIsEmpty(void)
{
	heap_t *heap = HeapCreate(CompareInts, NULL);

	TH_ASSERT(1 == HeapIsEmpty(heap));

	HeapDestroy(heap);
}

static void TestHeapSize(void)
{
	heap_t *heap = HeapCreate(CompareInts, NULL);

	TH_ASSERT(0 == HeapSize(heap));

	HeapDestroy(heap);
}

static void TestHeapPush(void)
{
	int test_vals[5] = {1, 2, 3, 4, 5};
	heap_t *heap = HeapCreate(CompareInts, NULL);

	HeapPush(heap, &test_vals[4]);
	HeapPush(heap, &test_vals[3]);
	HeapPush(heap, &test_vals[2]);
	HeapPush(heap, &test_vals[1]);
	HeapPush(heap, &test_vals[0]);

	TH_ASSERT(0 == HeapIsEmpty(heap));
	TH_ASSERT(5 == HeapSize(heap));

	HeapDestroy(heap);
}

static void TestHeapPeek(void)
{
	int test_vals[5] = {1, 2, 3, 4, 5};
	heap_t *heap = HeapCreate(CompareInts, NULL);

	HeapPush(heap, &test_vals[4]);
	HeapPush(heap, &test_vals[3]);
	HeapPush(heap, &test_vals[2]);
	HeapPush(heap, &test_vals[1]);
	HeapPush(heap, &test_vals[0]);

	TH_ASSERT(&test_vals[0] == HeapPeek(heap));

	HeapDestroy(heap);
}

static void TestHeapPop(void)
{
	int test_vals[5] = {1, 2, 3, 4, 5};
	heap_t *heap = HeapCreate(CompareInts, NULL);

	HeapPush(heap, &test_vals[4]);
	HeapPush(heap, &test_vals[3]);
	HeapPush(heap, &test_vals[2]);
	HeapPush(heap, &test_vals[1]);
	HeapPush(heap, &test_vals[0]);

	
	TH_ASSERT(&test_vals[0] == HeapPop(heap));
	TH_ASSERT(&test_vals[1] == HeapPeek(heap));
	TH_ASSERT(4 == HeapSize(heap));

	TH_ASSERT(&test_vals[1] == HeapPop(heap));
	TH_ASSERT(&test_vals[2] == HeapPeek(heap));
	TH_ASSERT(3 == HeapSize(heap));

	TH_ASSERT(&test_vals[2] == HeapPop(heap));
	TH_ASSERT(&test_vals[3] == HeapPeek(heap));
	TH_ASSERT(2 == HeapSize(heap));

	TH_ASSERT(&test_vals[3] == HeapPop(heap));
	TH_ASSERT(&test_vals[4] == HeapPeek(heap));
	TH_ASSERT(1 == HeapSize(heap));

	HeapDestroy(heap);
}

static void TestHeapRemove(void)
{
	int test_vals[5] = {1, 2, 3, 4, 5};
	heap_t *heap = HeapCreate(CompareInts, NULL);

	HeapPush(heap, &test_vals[4]);
	HeapPush(heap, &test_vals[3]);
	HeapPush(heap, &test_vals[2]);
	HeapPush(heap, &test_vals[1]);
	HeapPush(heap, &test_vals[0]);

	
	TH_ASSERT(&test_vals[4] == HeapRemove(heap, IsMatchInt, &test_vals[4]));
	TH_ASSERT(&test_vals[0] == HeapPeek(heap));
	TH_ASSERT(4 == HeapSize(heap));

	TH_ASSERT(&test_vals[1] == HeapRemove(heap, IsMatchInt, &test_vals[1]));
	TH_ASSERT(&test_vals[0] == HeapPeek(heap));
	TH_ASSERT(3 == HeapSize(heap));

	TH_ASSERT(&test_vals[0] == HeapPop(heap));
	TH_ASSERT(&test_vals[2] == HeapPeek(heap));
	TH_ASSERT(2 == HeapSize(heap));

	HeapDestroy(heap);
}



static int CompareInts(const void *data1, const void *data2, void *params)
{
	if (*(int *) data1 < *(int *) data2)
	{
		return (-1);
	}
	else if (*(int *) data1 == *(int *) data2)
	{
		return (0);
	}

	return (1);
	(void) params;
}

static int IsMatchInt(const void *data1, void *param)
{
	return (*(int *) data1 == *(int *) param);
}