/*******************************************************************************
*
* FILENAME : mt_csort_test.c
*
* DESCRIPTION : Multithreaded counting sort unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 10.07.2023
* 
*******************************************************************************/
#define _POSIX_C_SOURCE 199309L

#include <stdlib.h> /* rand */
#include <time.h> /* time_t, clock */
#include <string.h> /* strlen */

#include "mt_csort.h"
#include "testing.h"


typedef int (*sort_func_t)(char *arr, size_t size, size_t num_threads);

static void *ReadDictionary(void);
static void BMDicSorting(char *dic, sort_func_t SortFunc,size_t times_concat,
                                                         size_t num_threads);
static double CalcTimeDiff(struct timespec *start, struct timespec *end);


static void TestMTSort(void);
static void TestBenchmark(void);

int main()
{
	TH_TEST_T TESTS[] = {
        {"Sorting", TestMTSort},
        {"Benchmark", TestBenchmark},
        TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(TESTS);

	return (0);
}

static void TestMTSort(void)
{
    char arr1[] = "aaaaaaaaa";
    char arr2[] = "1234567ab";
    char arr3[] = "Q1CFR234567A9B21";
	
    MTCountingSort(arr1, 9, 2);
	
    TH_ASSERT(0 == strcmp(arr1, "aaaaaaaaa"));

    arr2[8] = -124;
    arr2[3] = -22;

	MTCountingSort(arr2, 9, 3);

    TH_ASSERT(-124 == arr2[0]);
    TH_ASSERT(-22 == arr2[1]);

	MTCountingSort(arr3, 16, 5);
    
	TH_ASSERT(0 == strcmp(arr3, "1122345679ABCFQR"));
}

static void TestBenchmark(void)
{
    char *dictionary = ReadDictionary();

    BMDicSorting(dictionary, MTCountingSort, 100, 1);
 	BMDicSorting(dictionary, MTCountingSort, 100, 2);
 	BMDicSorting(dictionary, MTCountingSort, 100, 4);
 	BMDicSorting(dictionary, MTCountingSort, 100, 6);

    BMDicSorting(dictionary, MTCountingSort, 1000, 1);
 	BMDicSorting(dictionary, MTCountingSort, 1000, 2);
 	BMDicSorting(dictionary, MTCountingSort, 1000, 4);
 	BMDicSorting(dictionary, MTCountingSort, 1000, 6);

    free(dictionary);
}

static void *ReadDictionary(void)
{
	char *path = "/usr/share/dict/words";

    size_t length = 0;

	char *dictionary_buffer = NULL;
	char *runner_buffer = NULL;

	char input_buffer[100] = {'\0'};

	int i = 0;

	FILE *stream_from = fopen(path,"r");
	if (NULL == stream_from)
	{
		return (NULL);
	}

	dictionary_buffer = (char *) malloc((sizeof(char) * 100) * 105000);
	if (NULL == dictionary_buffer)
	{
		return (NULL);
	}

    runner_buffer = dictionary_buffer;

	for (; NULL != fgets(input_buffer, 100, stream_from); ++i)
	{
        length = strcspn(input_buffer, "\n");
		input_buffer[length] = 0;
		strcpy(runner_buffer, input_buffer);
		runner_buffer += (length - 2);
	}

    runner_buffer = '\0';

	fclose(stream_from);

    return (dictionary_buffer);
}

static void BMDicSorting(char *dic, sort_func_t SortFunc, size_t times_concat, size_t num_threads)
{
    struct timespec start, end;

    size_t i = 0, length = 0;

    char *buffer_to_sort = (char *) malloc(sizeof(char) * (700000 * times_concat));
    
    for (; i < times_concat; ++i)
    {
        strcat(buffer_to_sort, dic);
    }

    length = strlen(buffer_to_sort);

    clock_gettime(CLOCK_MONOTONIC, &start);
 	SortFunc(buffer_to_sort, length, num_threads);
    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("[Size of sorted string is %ld chars]\n", length);
    printf("[Number of threads is %ld]\n", num_threads); 
 	printf("Total time taken: %f\n\n", CalcTimeDiff(&start, &end));

    free(buffer_to_sort);
}

static double CalcTimeDiff(struct timespec *start, struct timespec *end)
{
    return ((end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9);
}