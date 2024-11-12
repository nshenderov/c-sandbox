/*******************************************************************************
*
* FILENAME : sorts_test.c
*
* DESCRIPTION : Sorting algorithms unit tests.
*
* AUTHOR : Nick Shenderov
*
* DATE : 22.05.2023
* 
*******************************************************************************/

#include <stdlib.h> /* rand */
#include <time.h> /* time_t, clock */

#include "sorts.h"
#include "testing.h"


enum {FALSE, TRUE};

#define TEST_10_VALUES 0,32,333,12,3,2,5,5,99,111
#define TEST_10_VALUES_EXPECTED 0,2,3,5,5,12,32,99,111,333

static int IsEqualArr(size_t size, int arr1[], int arr2[]);
static void FillRandom(int arr[], size_t size);
static void BenchmarkSorting(void (*SortFunc)(int arr[], size_t size));


static void TestBubbleSort(void);
static void TestInsertionSort(void);
static void TestSelectionSort(void);
static void TestCountingSort(void);
static void TestRadixSort(void);

int main()
{
   	TH_TEST_T TESTS[] = {
		{"BubbleSort", TestBubbleSort},
		{"InsertionSort", TestInsertionSort},
		{"SelectionSort", TestSelectionSort},
		{"CountSort", TestCountingSort},
		{"RadixSort", TestRadixSort},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(TESTS);

	return (0);
}


static void TestBubbleSort(void)
{
	int arr1[10] = {TEST_10_VALUES};
	int arr2[10] = {TEST_10_VALUES_EXPECTED};

	BubbleSort(arr1, 10);

	TH_ASSERT(1 == IsEqualArr(10, arr1, arr2));

 	BenchmarkSorting(BubbleSort);
}

static void TestInsertionSort(void)
{
	int arr1[10] = {TEST_10_VALUES};
	int arr2[10] = {TEST_10_VALUES_EXPECTED};

	InsertionSort(arr1, 10);

	TH_ASSERT(1 == IsEqualArr(10, arr1, arr2));

	BenchmarkSorting(InsertionSort);
}

static void TestSelectionSort(void)
{
	int arr1[10] = {TEST_10_VALUES};
	int arr2[10] = {TEST_10_VALUES_EXPECTED};

	SelectionSort(arr1, 10);

	TH_ASSERT(1 == IsEqualArr(10, arr1, arr2));

	BenchmarkSorting(SelectionSort);
}

static void TestCountingSort(void)
{
	int arr1[10] = {TEST_10_VALUES};
	int arr2[10] = {TEST_10_VALUES_EXPECTED};

	CountingSort(arr1, 10);

	TH_ASSERT(1 == IsEqualArr(10, arr1, arr2));

	BenchmarkSorting(CountingSort);
}

static void TestRadixSort(void)
{
	int arr1[10] = {TEST_10_VALUES};
	int arr2[10] = {TEST_10_VALUES_EXPECTED};

	RadixSort(arr1, 10);

	TH_ASSERT(1 == IsEqualArr(10, arr1, arr2));

	BenchmarkSorting(RadixSort);
}

static int IsEqualArr(size_t size, int arr1[], int arr2[])
{
	size_t i = 0;

	for (; i < size; ++i)
	{
		if (arr1[i] != arr2[i])
		{
			return (FALSE);
		}
	}

	return (TRUE);
}

static void FillRandom(int arr[], size_t size)
{	
	size_t i = 0;

	time_t t;

	srand((unsigned) time(&t));

	for (; i < size; ++i)
	{
    	arr[i] = (rand() % 20) + 50;
	}
}

static void BenchmarkSorting(void (*SortFunc)(int arr[], size_t size))
{
	clock_t start_t, end_t;

 	int arr[5000] = {0};

 	start_t = clock();

	FillRandom(arr, 5000);

 	SortFunc(arr, 5000);

 	end_t = clock();

 	printf("Benchmark:\n");
 	printf("Total time taken by CPU: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);
}