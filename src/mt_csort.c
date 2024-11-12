/*******************************************************************************
*
* FILENAME : mt_csort.c
*
* DESCRIPTION : Multithreaded counting sort implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 10.07.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* size_t, malloc */
#include <pthread.h> /* threads */
#include <string.h> /* memcpy */

#include "mt_csort.h"

#define SUCCESS (0)
#define FAIL (1)

typedef struct thread_params
{
    pthread_t thread;
    char *array;
    size_t size;
    char start;
    char end;
} th_params_t;

static size_t CreateThreads(char *array, size_t size, size_t *num_threads,
                                                        th_params_t *threads);
static void InitThread(th_params_t *thread, char *array, size_t size,
                                        char range, char min, size_t thread_n);
static void *ThreadBucket(void *params);
static size_t CountElemsInRange(char *arr, size_t size, char start, char end);
static void FillBuckedFromArr(char *dest, char *src, size_t size,
                                                        char start, char end);
static int CountingSort(char arr[], size_t size);
static void FindMinMax(const char arr[], size_t size, char *min, char *max);
static int JoinThreads(th_params_t *threads, size_t threads_num);
static void CleanUpBuckets(th_params_t *threads, size_t buckets_num);
static void MergeBuckets(th_params_t *threads, size_t buckets_num, char *dest);

int MTCountingSort(char *array, size_t size, size_t num_threads)
{
    th_params_t *threads = NULL;
    size_t created_threads = 0;

    assert(NULL != array);

    threads = (th_params_t *)malloc(sizeof(th_params_t) * num_threads);
    if (NULL == threads)
    {
        return (FAIL);
    }

    created_threads = CreateThreads(array, size, &num_threads, threads);
    if (num_threads != created_threads)
    {
        JoinThreads(threads, created_threads);
        CleanUpBuckets(threads, created_threads);
        free(threads);
        return (FAIL);
    }

    if (FAIL == JoinThreads(threads, created_threads))
    {
        CleanUpBuckets(threads, created_threads);
        free(threads);
        return (FAIL);
    }

    MergeBuckets(threads, created_threads, array);
    CleanUpBuckets(threads, created_threads);
    free(threads);
    threads = NULL;

    return (SUCCESS);
}

static size_t CreateThreads(char *array, size_t size, size_t *num_threads,
                                                        th_params_t *threads)
{
    size_t number_of_elements = 0;
    size_t best_num_threads = 0;
    size_t step = 0;
    size_t i = 0;
    char min = 0;
    char max = 0;

    assert(NULL != array);
    assert(NULL != threads);
    assert(NULL != num_threads);

    FindMinMax(array, size, &min, &max);
    number_of_elements = ((size_t)max - (size_t)min + 1);

    step = (number_of_elements / *num_threads) + 1;
    best_num_threads = (number_of_elements / step) + 1;
    *num_threads = best_num_threads;

    for (i = 0; i < best_num_threads; ++i)
    {
        th_params_t *curr_thread = threads + i;

        InitThread(curr_thread, array, size, step, min, i);

        if (0 != pthread_create(&(curr_thread->thread), NULL,
                                                    ThreadBucket, curr_thread))
        {
            return (i + 1);
        }
    }

    return (i);
}

static void InitThread(th_params_t *thread, char *array, size_t size,
                                    char range, char min, size_t thread_n)
{
    assert(NULL != array);

    thread->array = array;
    thread->size = size;
    thread->start = range * thread_n + min;
    thread->end = range * (thread_n + 1) + min;
}

static void *ThreadBucket(void *params)
{
    th_params_t th_params = {0};
    char *bucket = NULL;
    size_t bucket_size = 0;

    assert(NULL != params);

    th_params = *(th_params_t *)params;
    bucket_size = CountElemsInRange(th_params.array, th_params.size,
                                    th_params.start, th_params.end);
    bucket = (char *)malloc(sizeof(char) * bucket_size);
    if (NULL == bucket)
    {
        return (NULL);
    }

    FillBuckedFromArr(bucket, th_params.array, th_params.size,
                                            th_params.start, th_params.end);
    if (FAIL == CountingSort(bucket, bucket_size))
    {
        return (NULL);
    }

    ((th_params_t *)params)->array = bucket;
    ((th_params_t *)params)->size = bucket_size;

    return (params);
}

static size_t CountElemsInRange(char *arr, size_t size, char start, char end)
{
    size_t counter = 0;
    size_t i = 0;

    assert(NULL != arr);

    for (i = 0; i < size; ++i)
    {
        char curr_elem = arr[i];
        if (curr_elem >= start && curr_elem < end)
        {
            ++counter;
        }
    }

    return (counter);
}

static void FillBuckedFromArr(char *dest, char *src, size_t size,
                                                        char start, char end)
{
    size_t i = 0;
    size_t j = 0;

    assert(NULL != dest);
    assert(NULL != src);

    for (i = 0; i < size; ++i)
    {
        char curr_elem = src[i];
        if ((curr_elem >= start && curr_elem < end))
        {
            dest[j++] = curr_elem;
        }
    }
}

static int CountingSort(char arr[], size_t size)
{
    size_t *count_arr = NULL;
    char curr_value = 0;
    size_t number_of_elements = 0;
    size_t i = 0;
    char min = 0;
    char max = 0;

    assert(NULL != arr);

    FindMinMax(arr, size, &min, &max);
    number_of_elements = (size_t)(max - min + 1);
    count_arr = (size_t *)calloc(number_of_elements, sizeof(size_t));
    if(NULL == count_arr)
    {
        return (FAIL);
    }

    for(i = 0; i < size; ++i)
    {
        count_arr[arr[i] - min] += 1;
    }

    for(i = 0, curr_value = min; curr_value <= max; ++curr_value)
    {
        size_t fill_n_times = count_arr[curr_value - min];
        size_t j = 0;

        for(j = 0; j < fill_n_times; ++j)
        {
            arr[i++] = curr_value;
        }
    }

    free(count_arr);
    count_arr = NULL;

    return (0);
}

static void FindMinMax(const char arr[], size_t size, char *min, char *max)
{
    size_t i = 0;
    char temp_min = 0;
    char temp_max = 0;

    assert(NULL != arr);
    assert(NULL != min);
    assert(NULL != max);

    temp_min = arr[0];
    temp_max = arr[0];

    for(i = 1; i < size; ++i)
    {
        int curr_elem = arr[i];

        if(curr_elem < temp_min)
        {
            temp_min = curr_elem;
        }

        if(curr_elem > temp_max)
        {
            temp_max = curr_elem;
        }
    }

    *min = temp_min;
    *max = temp_max;
}

static int JoinThreads(th_params_t *threads, size_t threads_num)
{
    size_t i = 0;
    int threads_status = SUCCESS;

    assert(NULL != threads);

    for (i = 0; i < threads_num; ++i)
    {
        th_params_t *curr_thread = threads + i;
        pthread_join(curr_thread->thread, (void **)&curr_thread);
        if(NULL == curr_thread)
        {
            threads_status = FAIL;
        }
    }

    return (threads_status);
}

static void CleanUpBuckets(th_params_t *threads, size_t buckets_num)
{
    size_t i = 0;

    assert(NULL != threads);

    for (i = 0; i < buckets_num; ++i)
    {
        th_params_t *curr_thread = threads + i;
        free(curr_thread->array);
    }
}

static void MergeBuckets(th_params_t *threads, size_t buckets_num, char *dest)
{
    size_t i = 0;

    assert(NULL != threads);
    assert(NULL != dest);

    for (i = 0; i < buckets_num; ++i)
    {
        th_params_t *curr_thread = threads + i;

        memcpy(dest, curr_thread->array, sizeof(char) * curr_thread->size);
        dest += curr_thread->size;
    }
}
