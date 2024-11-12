/*******************************************************************************
*
* FILENAME : mt_qsort.h
*
* DESCRIPTION : Multithreaded quick sort implementation api.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 12.07.2023
* 
*******************************************************************************/

#ifndef __NSRD_MT_QUICK_SORT_H__
#define __NSRD_MT_QUICK_SORT_H__

#include <stddef.h> /* size_t */

/*
DESCRIPTION:
    Multithreaded implementation of the quick sort.
RETURN:
    Nothing
INPUT:
    base: pointer to the array to be sorted.
    nmemb: amount of elements in the array.
    size: size of an element.
    compare: pointer to the comparison function.
    threads_amount: desired amount of threads to be used in sorting.    
*/
void MTQuickSort(void *base, size_t nmemb, size_t size,
                 int (*compare)(const void *, const void *),
                 size_t threads_amount);

#endif /* __NSRD_MT_QUICK_SORT_H__ */
