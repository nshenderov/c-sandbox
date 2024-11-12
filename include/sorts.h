/*******************************************************************************
*
* FILENAME : sorts.h
*
* DESCRIPTION : Sorting algorithm is an algorithm that puts elements of a list
* into an order. The most frequently used orders are numerical order and
* lexicographical order, and either ascending or descending.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 22.05.2023
* 
*******************************************************************************/

#ifndef __SORTS_H__
#define __SORTS_H__

#include <stddef.h> /* size_t, NULL */

/*
DESCRIPTION
	Builds the final sorted array one element at a time by repeatedly
	inserting the next element into its correct position in the already
	sorted portion of the array. It is efficient for small datasets
	or partially sorted arrays and has a best-case time complexity of O(n)
RETURN
	Doesn't return anything.
INPUT
	arr: pointer to an array of ints.
	size: number of elements in the array.
TIME_COMPLEXITY
    O(n^2)
*/
void InsertionSort(int arr[], size_t size);

/*
DESCRIPTION
	Repeatedly selects the smallest (or largest) element and swaps it with
	the current position, gradually building the sorted portion of the array.
	It is an intuitive and straightforward algorithm but inefficient for
	large datasets.
RETURN
	Doesn't return anything.
INPUT
	arr: pointer to the array of ints.
	size: number of elements in the array.
TIME_COMPLEXITY
    O(n^2)
*/
void SelectionSort(int arr[], size_t size);

/*
DESCRIPTION
	Repeatedly compares adjacent elements and swaps them if they are in
	the wrong order, gradually moving the largest elements to the end
	of the array. It has a simple implementation but is not very efficient,
	especially for large datasets,
RETURN
	Doesn't return anything.
INPUT
	arr: pointer to the array of ints.
	size: number of elements in the array.
TIME_COMPLEXITY
    O(n^2)
*/
void BubbleSort(int arr[], size_t size);


/*
DESCRIPTION
	Non-comparison-based sorting algorithm suitable for sorting integers
	with a limited range. It counts the occurrences of each value and
	reconstructs the sorted array based on these counts. Counting Sort has
	a time complexity of O(n+k), where n is the number of elements and k is
	the range of values. It is efficient when the range is small compared
	to the number of elements. Creation may fail, due to memory allocation fail.
RETURN
	0: success;
	1: allocation failed
INPUT
	arr: pointer to the array of ints.
	size: number of elements in the array.
TIME_COMPLEXITY
    O(n)
*/
void CountingSort(int arr[], size_t size);

/*
DESCRIPTION
	Non-comparison-based sorting algorithm that sorts numbers by
	grouping them based on each digit's value. It processes the digits
	from the least significant digit to the most significant digit, sorting
	the elements based on each digit position. Radix Sort has a time
	complexity of O(d*(n+k)), where n is the number of elements, k is the
	range of values, and d is the number of digits. It is efficient for
	sorting integers with a large range of values. Creation may fail, due to
    memory allocation fail.
RETURN
	0: success;
	1: allocation failed
INPUT
	arr: pointer to the array of ints.
	size: number of elements in the array.
TIME_COMPLEXITY
    O(n)
*/
void RadixSort(int arr[], size_t size);

/*
DESCRIPTION
	Divide-and-conquer stable sorting algorithm that recursively splits the
    array into halves, sorts each half, and then merges the sorted halves. It
    has a stable time complexity of O(nlog⁡ n).
RETURN
	0: success;
	1: failure.
INPUT
	arr_to_sort: array of integers;
	num_elements: number of elements in the array.
TIME_COMPLEXITY
    O(nlog n)
*/
int MergeSort(int *arr_to_sort, size_t num_elements);

/*
DESCRIPTION
	Divide-and-conquer non stable sorting algorithm that selects a pivot
    element, partitions the array into elements less than and greater than the
    pivot, and recursively sorts each partition. It has an average time
    complexity of O(nlog⁡ n) but can degrade to O(n^2) in the worst case.
	User provides the logic for elements comparison.
RETURN
	There is no return for this function.
INPUT
	base: generic pointer to the collection of elements;
	nmemb: number of elements in the collection.
	size: size of an element.
	compar: pointer to the function that compares elements.
TIME_COMPLEXITY
    O(log n), worst O(n^2)
*/
void QuickSort(void *base, size_t nmemb, size_t size,
                  int (*compar)(const void *, const void *));

#endif  /* __SORTS_H__ */