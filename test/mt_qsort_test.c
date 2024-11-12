/*******************************************************************************
*
* FILENAME : mt_qsort_test.c
*
* DESCRIPTION : Multithreaded quick sort unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 12.07.2023
* 
*******************************************************************************/
#define _POSIX_C_SOURCE 199309L

#include <stdlib.h> /* rand, qsort */
#include <time.h> /* time_t, clock */
#include <string.h> /* strlen */
#include <stdio.h> /* perror */

#include "mt_qsort.h"
#include "testing.h"


#define PATH_TO_DIC ("/usr/share/dict/words")
#define DIC_COPIES (100)
#define NUM_OF_THREADS (4)

typedef int (*sort_func_t)(char *arr, size_t size, size_t num_threads);

static char *ReadDictionary(size_t *size);
static size_t GetFileSize(FILE *stream_from);
static char *RemoveNewline(char *string);
static char **CreateStrsArrToSort(char *dic_buffer, size_t dic_size, size_t n_copies);
static char **CreateCopyArrToSort(char **src, size_t dic_size, size_t n_copies);
static char **AllocStringsArray(size_t dic_size, size_t n_copies);
static void GenerateCopies(char **strings, size_t dic_size, size_t n_copies);
static void ShuffleArr(void *base, size_t nmeb);
static int RandomCompare(const void *p1, const void *p2);
static int SortStrings(const void *p1, const void *p2);
static double CalcTimeDiff(struct timespec *start, struct timespec *end);


static void TestSorting(void);
static void TestBenchmark(void);

int main()
{
	TH_TEST_T TESTS[] = {
        {"Sorting", TestSorting},
        {"Benchmark", TestBenchmark},
        TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(TESTS);

	return (0);
}

static void TestSorting(void)
{
    size_t i = 0, dic_size = 0, match = 1, n_copies = 1;
    char *dic_buffer = NULL;
    char **strs_to_mtqsort = NULL;
    char **strs_to_mtqsort_th1 = NULL;
    char **strs_to_qsort = NULL;

    srand(time(NULL));

    dic_buffer = ReadDictionary(&dic_size);
    strs_to_mtqsort = CreateStrsArrToSort(dic_buffer, dic_size, n_copies);
    strs_to_mtqsort_th1 = CreateCopyArrToSort(strs_to_mtqsort, dic_size, n_copies);
    strs_to_qsort = CreateCopyArrToSort(strs_to_mtqsort, dic_size, n_copies);

    MTQuickSort(strs_to_mtqsort, dic_size * n_copies, sizeof(char *), SortStrings, NUM_OF_THREADS);
    MTQuickSort(strs_to_mtqsort_th1, dic_size * n_copies, sizeof(char *), SortStrings, 1);
    qsort(strs_to_qsort, dic_size * n_copies, sizeof(char *), SortStrings);
   
    for (; i < dic_size * n_copies; ++i)
    {
        if (0 != strcmp(strs_to_mtqsort[i], strs_to_qsort[i]) 
            || 0 != strcmp(strs_to_mtqsort_th1[i], strs_to_qsort[i]))
        {
            match = 0;
            break;
        }
    }

    TH_ASSERT(1 == match);
}

static void TestBenchmark(void)
{
    size_t dic_size = 0;
    struct timespec start_t, end_t;
    char *dic_buffer = NULL;
    char **strs_to_mtqsort = NULL;
    char **strs_to_mtqsort_1th = NULL;
    char **strs_to_qsort = NULL;

    srand(time(NULL));

    printf("Preparing arrays...\n");

    dic_buffer = ReadDictionary(&dic_size);
    strs_to_mtqsort = CreateStrsArrToSort(dic_buffer, dic_size, DIC_COPIES);
    strs_to_mtqsort_1th = CreateCopyArrToSort(strs_to_mtqsort, dic_size, DIC_COPIES);
    strs_to_qsort = CreateCopyArrToSort(strs_to_mtqsort, dic_size, DIC_COPIES);

    printf("Size of the dictionary array\nto sort is %ld words\n\n", dic_size * DIC_COPIES);


    printf("Sorting mtqsort...\nThreads amount is %d\n", NUM_OF_THREADS);

    clock_gettime(CLOCK_MONOTONIC, &start_t);
    MTQuickSort(strs_to_mtqsort, dic_size * DIC_COPIES, sizeof(char *), SortStrings, NUM_OF_THREADS);
    clock_gettime(CLOCK_MONOTONIC, &end_t);

    printf("Total time taken: %f\n\n", CalcTimeDiff(&start_t, &end_t));


    printf("Sorting mtqsort...\nThreads amount is %d\n", 1);

    clock_gettime(CLOCK_MONOTONIC, &start_t);
    MTQuickSort(strs_to_mtqsort_1th, dic_size * DIC_COPIES, sizeof(char *), SortStrings, 1);
    clock_gettime(CLOCK_MONOTONIC, &end_t);

    printf("Total time taken: %f\n\n", CalcTimeDiff(&start_t, &end_t));


    printf("Sorting stdlib qsort...\n"); 

    clock_gettime(CLOCK_MONOTONIC, &start_t);
    qsort(strs_to_qsort, dic_size * DIC_COPIES, sizeof(char *), SortStrings);
    clock_gettime(CLOCK_MONOTONIC, &end_t);

    printf("Total time taken: %f\n", CalcTimeDiff(&start_t, &end_t));


    free(dic_buffer);
    free(strs_to_mtqsort);
    free(strs_to_mtqsort_1th);
    free(strs_to_qsort);
}

static int SortStrings(const void *p1, const void *p2)
{
    return (strcmp(*(char **) p1, *(char **) p2));
}

static double CalcTimeDiff(struct timespec *start, struct timespec *end)
{
    return ((end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9);
}

static char *ReadDictionary(size_t *size)
{
    size_t file_size = 0;
    size_t word_counter = 0;
	char *dictionary_buffer = NULL;
	char *buffer_runner = NULL;

	FILE *stream_from = fopen(PATH_TO_DIC,"r");
	if (NULL == stream_from)
	{
		return (NULL);
	}

    file_size = GetFileSize(stream_from);

    dictionary_buffer = (char *) malloc(file_size + 50);
    if (NULL == dictionary_buffer)
    {
    	fclose(stream_from);
        stream_from = NULL;
        return (NULL);
    }

    buffer_runner = dictionary_buffer;

    while (NULL != fgets(buffer_runner, 50, stream_from))
    {
        char *fixed_buffer = NULL;

        fixed_buffer = RemoveNewline(buffer_runner);

        ++word_counter;

        buffer_runner = fixed_buffer + 1;
    }

    *size = word_counter;
    
    if (ferror(stream_from))
    {
    	free(dictionary_buffer);
    	dictionary_buffer = NULL;
        fclose(stream_from);
        stream_from = NULL;

        return (NULL);
    }

    if (EOF == fclose(stream_from))
    {
    	free(dictionary_buffer);
    	dictionary_buffer = NULL;

        return (NULL);
    }

    return (dictionary_buffer);
}

static char *RemoveNewline(char *string)
{
    size_t word_length = strcspn(string, "\n");
    string[word_length] = 0;

    return (string + word_length);
}

static size_t GetFileSize(FILE *stream_from)
{
    size_t file_size = 0;

    if (-1 == fseek(stream_from, 0, SEEK_END))
    {
    	fclose(stream_from);
        stream_from = NULL;
        return (0);
    }

    file_size = ftell(stream_from);
    rewind(stream_from);

    return (file_size);
}

static int RandomCompare(const void *p1, const void *p2)
{
    return (rand() % 2);

    (void) p1;
    (void) p2;
}

static void ShuffleArr(void *base, size_t nmeb)
{
    qsort(base, nmeb, sizeof(char *), RandomCompare);
}

static char **AllocStringsArray(size_t dic_size, size_t n_copies)
{
    char **arr = (char **) malloc((sizeof(char *) * dic_size) * n_copies);
    if (NULL == arr)
    {
        perror("Array llocation failed");
        return (NULL);
    }
    
    return arr;
}

static char **CreateStrsArrToSort(char *dic_buffer, size_t dic_size, size_t n_copies)
{
    size_t i = 0;

    char **strings = AllocStringsArray(dic_size, n_copies);

    for (; i < dic_size; ++i)
    {
        strings[i] = dic_buffer;

        dic_buffer += strlen(dic_buffer) + 1;
    }

    GenerateCopies(strings, dic_size, n_copies);
    ShuffleArr(strings, dic_size * n_copies);

    return (strings);
}

static char **CreateCopyArrToSort(char **src, size_t dic_size, size_t n_copies)
{
    char **arr = AllocStringsArray(dic_size, n_copies);
    memcpy(arr, src, (sizeof(char *) * dic_size) * n_copies);

    return (arr);
}

static void GenerateCopies(char **strings, size_t dic_size, size_t n_copies)
{
    char **runner = strings + dic_size;
    size_t i = 0;

    for (i = 0; i < n_copies - 1; ++i)
	{
		memcpy(runner, strings, dic_size * sizeof(char *));
        runner += dic_size;
	}
}