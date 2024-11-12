/*******************************************************************************
*
* FILENAME : sorts.c
*
* DESCRIPTION : Sorting algorithms implemetations.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 22.05.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, calloc */
#include <time.h> /* time */

#include "sorts.h"

enum {SUCCESS, FAILURE};

static void Swap(int *a, int *b);
static int GetMax(int arr[], size_t size);
static void RadixCountSort(int arr[], size_t size, int exp);

static void MergeSorting(int *array, int *temp, int low, int high);
static void Merge(int *array, int *temp, int left, int middle, int right);
static void QuickSorting(void *base, int low, int high, size_t size,
                                    int (*compar)(const void *, const void *));
static int Partition(char *base, int low, int high, size_t size,
                                    int (*compar)(const void *, const void *));
static void SwapQuickSort(char *a, char *b, size_t size);

int MergeSort(int *arr_to_sort, size_t num_elements)
{
    int *temp = NULL;

    assert(NULL != arr_to_sort);
    assert(0 < num_elements);

    temp = (int *) malloc(num_elements * sizeof(int));
    if (temp == NULL)
    {
        return (FAILURE);
    }

    MergeSorting(arr_to_sort, temp, 0, num_elements - 1);

    return (SUCCESS);
}

static void MergeSorting(int *array, int *temp, int low, int high)
{
    assert(NULL != array);
    assert(NULL != temp);

    if (low < high)
    {
        int middle = (high - low) / 2 + low;

        MergeSorting(array, temp, low, middle);
        MergeSorting(array, temp, middle + 1, high);

        Merge(array, temp, low, middle, high);
    }
}

static void Merge(int *array, int *temp, int left, int middle, int right)
{
    int i = left;
    int j = middle + 1;
    int k = left;

    assert(NULL != array);
    assert(NULL != temp);

    while (i <= middle && j <= right)
    {
        if (array[i] <= array[j])
        {
            temp[k++] = array[i++];
        }
        else
        {
            temp[k++] = array[j++];
        }
    }

    while (i <= middle)
    {
        temp[k++] = array[i++];
    }

    while (j <= right)
    {
        temp[k++] = array[j++];
    }

    for (i = left; i <= right; ++i)
    {
        array[i] = temp[i];
    }
}

void QuickSort(void *base, size_t nmemb, size_t size,
                  int (*compar)(const void *, const void *))
{
    assert(NULL != base);
    assert(NULL != compar);
    assert(0 < nmemb);

    QuickSorting(base, 0, nmemb - 1, size, compar);
}

static void QuickSorting(void *base, int low, int high, size_t size,
                                    int (*compar)(const void *, const void *))
{
    assert(NULL != base);
    assert(NULL != compar);

    srand(time(NULL));

    if (low < high)
    {
        int pi = Partition(base, low, high, size, compar);
 
        QuickSorting(base, low, pi - 1, size, compar);
        QuickSorting(base, pi + 1, high, size, compar);
    }
}

static int Partition(char *base, int low, int high, size_t size,
                                    int (*compar)(const void *, const void *))
{
    int random = 0;
    int j = low;
    int index = low;
    void *pivot = base + high * size;

    assert(NULL != base);
    assert(NULL != compar);

    random = low + rand() % (high - low);
    SwapQuickSort(base + random * size, base + high * size, size);
 
    for (; j <= high - 1; ++j)
    {
        if (compar(base + j * size, pivot))
        {
            SwapQuickSort(base + index * size, base + j * size, size);
            ++index;
        }
    }

    SwapQuickSort(base + index * size, base + high * size, size);
    
    return (index);
}

static void SwapQuickSort(char *a, char *b, size_t size)
{
    char tmp;

    assert(NULL != a);
    assert(NULL != b);

    while(0 != size--)
    {
        tmp = *(a + size);
        *(a + size) = *(b + size);
        *(b + size) = tmp;
    }
}

void BubbleSort(int arr[], size_t size)
{
   size_t i = 0;
   size_t q = 0;
   int swapped = 0;

   for (; i < size - 1; ++i)
   {
     swapped = 0;
     
     for (q = 0; q < size - i - 1; ++q)
     {
        if (arr[q] > arr[q+1])
        {
           Swap(&arr[q], &arr[q+1]);
           swapped = 1;
        }
     }

     if (swapped == 0)
     {
        break;
     }
   }
}

void InsertionSort(int arr[], size_t size)
{
   size_t i = 1;
   size_t q = 0;

   for (; i < size; ++i)
   {
		q = i;

		while (q != 0 && arr[q - 1] > arr[q])
		{
		   Swap(&arr[q], &arr[q - 1]);
		   --q;
		}
   }
}

void SelectionSort(int arr[], size_t size)
{
   size_t i = 0;
   size_t q = 0;
   size_t min_idx = 0;

	for (; i < size - 1; ++i)
	{
		min_idx = i;

		for (q = i + 1; q < size; ++q)
		{
			if (arr[q] < arr[min_idx])
			{
				min_idx = q;
			}
		}
		if(min_idx != i)
		{
			Swap(&arr[min_idx], &arr[i]);
		}
	}
}

void CountingSort(int arr[], size_t size)
{
	int *count_arr = NULL;
	int *output_arr = NULL;

	int i = 0;

	int max = GetMax(arr, size);

	count_arr = (int *) calloc(max + 1, sizeof(int));
	if (NULL == count_arr)
	{
		return;
	}

	output_arr = (int *) malloc(sizeof(int) * size);
	if (NULL == output_arr)
	{
		free(count_arr);
		count_arr = NULL;
		return;
	}

	for (i = 0; i < (int) size; ++i)
	{
		++count_arr[arr[i]];
	}

	for (i = 1; i <= max; ++i)
	{
		count_arr[i] += count_arr[i - 1];
	}

	for (i = size - 1; 0 <= i; --i)
	{
	    output_arr[count_arr[arr[i]] - 1] = arr[i];
	    --count_arr[arr[i]];
	}

	for (i = 0; i < (int) size; ++i)
	{
		arr[i] = output_arr[i];
	}

	free(count_arr);
	count_arr = NULL;

	free(output_arr);
	output_arr = NULL;
}

void RadixSort(int arr[], size_t size)
{
	int max = GetMax(arr, size);
	int exp = 1;

	for (; 0 < max / exp; exp *= 10)
	{
		RadixCountSort(arr, size, exp);
	}
}


static void Swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

static int GetMax(int arr[], size_t size)
{
   size_t i = 0;

	int max = 0;

	for (; i < size; ++i)
	{
		if (max < arr[i])
		{
			max = arr[i];
		}
	}

	return (max);
}
 
static void RadixCountSort(int arr[], size_t size, int exp)
{
	int *output_arr = NULL;

	int i = 0;

	int count_arr[10] = { 0 };

	output_arr = (int *) malloc(sizeof(int) * size);
	if (NULL == output_arr)
	{
		return;
	}

	for (i = 0; i < (int) size; ++i)
	{
		++count_arr[(arr[i] / exp) % 10];
	}

	for (i = 1; i < 10; ++i)
	{
		count_arr[i] += count_arr[i - 1];
	}

	for (i = size - 1; 0 <= i; --i)
	{
	    output_arr[count_arr[(arr[i] / exp) % 10] - 1] = arr[i];

	    --count_arr[(arr[i] / exp) % 10];
	}

	for (i = 0; i < (int)size; ++i)
	{
		arr[i] = output_arr[i];
	}

	free(output_arr);
	output_arr = NULL;
}