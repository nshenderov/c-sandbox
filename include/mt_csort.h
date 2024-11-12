/*******************************************************************************
*
* FILENAME : mt_csort.h
*
* DESCRIPTION : Multithreaded implementation of the counting sort api.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 10.07.2023
* 
*******************************************************************************/

#ifndef __NSRD_MT_COUNTING_SORT_H__
#define __NSRD_MT_COUNTING_SORT_H__

#include <stddef.h> /* size_t */

/*
DESCRIPTION:
    Multithreaded implementation of the counting sort.
RETURN:
    On success, MTCountingSort() returns 0.
    On error, it returns the error number.
INPUT:
    array: pointer to the array to be sorted.
    size: size of the array.    
    num_threads: desired amount of threads to be used during sorting.
*/
int MTCountingSort(char *array, size_t size, size_t num_threads);

#endif /* __NSRD_MT_COUNTING_SORT_H__ */