/*******************************************************************************
*
* FILENAME : mt_qsort.c
*
* DESCRIPTION : Multithreaded quick sort implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 12.07.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <pthread.h> /* threads */
#include <stdlib.h> /* srand, rand */
#include <stdio.h> /* perror */

#include "mt_qsort.h"

typedef int (*cmp_func_t)(const void *, const void *);

typedef struct {
    size_t size;
    size_t threads_num;
    char *low;
    char *high;
    cmp_func_t compar;
} qs_thread_args_t;

void MTQuickSort(void *base, size_t nmemb, size_t size, cmp_func_t compar,
                                                      size_t threads_num);
static void QuickSorting(char *low, char *high, size_t size, cmp_func_t compar,
                                                           size_t threads_num);
static void *QuickSortingThread(void *args);
static char *Partition(char *low, char *high, size_t size, cmp_func_t compar);
static void ChoosePivotRandomly(char *low, char *high, size_t size);                                              
static void GenericSwap(char *p1, char *p2, size_t size);


void MTQuickSort(void *base, size_t nmemb, size_t size, cmp_func_t compar,
                                                       size_t threads_num)
{
    char *low = (char *) base;
    char *high = (char *) base + (nmemb - 1) * size;

    assert(NULL != base);
    assert(NULL != compar);

    srand(time(NULL));

    QuickSorting(low, high, size, compar, threads_num);
}

static void QuickSorting(char *low, char *high, size_t size, cmp_func_t compar,
                                                            size_t threads_num)
{
    char *pivot = NULL;

    assert(NULL != low);
    assert(NULL != high);
    assert(NULL != compar);

    if (low >= high)
    {
        return;
    }
    
    pivot = Partition(low, high, size, compar);

    if (0 < threads_num - 1)
    {
        pthread_t thread;
        qs_thread_args_t args = {0};

        args.size = size;
        args.threads_num = threads_num - 1;
        args.low = low;
        args.high = pivot - size;
        args.compar = compar;
        
        if (0 != pthread_create(&thread, NULL, QuickSortingThread, &args))
        {
            perror("Error creating thread");
            return;
        }

        QuickSorting(pivot + size, high, size, compar, threads_num);

        pthread_join(thread, NULL);
    }
    else
    {
        QuickSorting(low, pivot - size, size, compar, threads_num);
        QuickSorting(pivot + size, high, size, compar, threads_num);
    }
}

static void *QuickSortingThread(void *args)
{
    qs_thread_args_t *unpck_args = args;
    QuickSorting(unpck_args->low, unpck_args->high, unpck_args->size,
                        unpck_args->compar, unpck_args->threads_num);
    return (NULL);
}

static void ChoosePivotRandomly(char *low, char *high, size_t size)
{
    char *random = NULL;

    assert(NULL != low);
    assert(NULL != high);

    random = low + ((size_t) rand() % ((high - low) / size)) * size;

    GenericSwap(random, high, size);
}

static char *Partition(char *low, char *high, size_t size, cmp_func_t compar)
{
    char *low_end = NULL;
    char *runner = NULL;

    assert(NULL != low);
    assert(NULL != high);
    assert(NULL != compar);

    ChoosePivotRandomly(low, high, size);

    for (low_end = low, runner = low; runner < high; runner += size)
    {
        if (0 > compar(runner, high))
        {
            GenericSwap(runner, low_end, size);
            low_end += size;
        }
    }

    GenericSwap(high, low_end, size);

    return (low_end);
}

static void GenericSwap(char *p1, char *p2, size_t size)
{
    assert(NULL != p1);
    assert(NULL != p2);

    while (0 != size--)
    {
        char temp = p1[size];
        p1[size] = p2[size];
        p2[size] = temp;
    }
}