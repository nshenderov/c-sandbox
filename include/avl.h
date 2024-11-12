/*******************************************************************************
*
* FILENAME : avl.h
*
* DESCRIPTION : AVL tree is a self-balancing binary search tree (BST) named
* after its inventors, Adelson-Velsky and Landis. It maintains a balance factor
* for each node, which is calculated as the difference between the heights of
* its left and right subtrees. By maintaining a balanced structure, AVL trees
* provide efficient search, insert, and delete operations with a guaranteed
* worst-case time complexity of O(log n), where n is the number of elements in
* the tree.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 05.06.2023
* 
*******************************************************************************/

#ifndef __NSRD_AVL_H__
#define __NSRD_AVL_H__

#include <stddef.h> /* size_t */

typedef struct avl avl_t;
typedef struct avl_node *avl_iter_t;


/*
DESCRIPTION:
    Compares two elements of the AVL.
RETURN:
    1: data1 > data2.
    -1: data1 < data2.
    0: data1 == data2.
INPUT:
    data1: pointer to the data to compare.
    data2: pointer to the data to compare.
    params: pointer to the additional parameters.
*/
typedef int(*avl_compare_t)(const void *data1, const void *data2, void *params);

/*
DESCRIPTION
    Performes the action on the element of the AVL.
RETURN
    0: success.
    non-zero value: failure.
INPUT
    data: pointer to the data;
    param: pointer to the additional parameter.
*/
typedef int(*avl_action_t)(void *data, void *param);


/*
DESCRIPTION:
    Creates an AVL.
    Creation may fail, due to memory allocation fail.
    User is responsible for memory deallocation.
RETURN:
    Returns pointer to the created AVL on success.
    Returns NULL on failure.
INPUT:
    compare: hash function used to compare elements.
    params: additional parameters used by the compare function.
TIME COMPLEXITY:
    O(1)
*/
avl_t *AVLCreate(avl_compare_t compare, void *params);

/*
DESCRIPTION:
    Frees the allocated for the AVL memory.
RETURN:
    There is no return for this function.
INPUT:
    avl: pointer to the AVL.
TIME COMPLEXITY:
    O(n)
*/
void AVLDestroy(avl_t *avl);

/*
DESCRIPTION:
    Inserts provided data into the AVL.
    Insertion may fail due to failed memory allocation.
RETURN:
    0: success.
    1: failure.
INPUT:
    avl: pointer to the AVL.
    data: pointer to the data to insert.
TIME COMPLEXITY:
    O(log n)
*/
int AVLInsert(avl_t *avl, void *data);

/*
DESCRIPTION:
    Removes the data from the AVL.
RETURN:
    There is no return for this function.
INPUT:
    avl: pointer to the AVL.
    data: pointer to the data to remove.
TIME COMPLEXITY:
    O(log n)
*/
void AVLRemove(avl_t *avl, void *data);

/*
DESCRIPTION:
    Searches for a data in the AVL.
RETURN:
    Pointer to the founded data on success.
    NULL if not found.
INPUT:
    avl: pointer to the AVL.
    data: pointer to the data to search for.
TIME COMPLEXITY:
    O(log n)
*/
void *AVLFind(const avl_t *avl, void *data);

/*
DESCRIPTION:
    Performes the action on each element of the AVL.
RETURN:
    1: success.
    0: failure.
INPUT:
	avl: pointer to the AVL.
    action: pointer to the action function.
    param: pointer to the additional parameter.
TIME COMPLEXITY:
    O(n)
*/
int AVLForEach(avl_t *avl, avl_action_t action, void *param);

/*
DESCRIPTION:
    Check whether the AVL is empty or not.
RETURN:
    1: empty.
    0: not empty.
INPUT:
    avl: pointer to the AVL.
TIME COMPLEXITY:
    O(1)
*/
int AVLIsEmpty(const avl_t *avl);

/*
DESCRIPTION:
    Computes the number of elements in the AVL.
RETURN:
    Computed value.
INPUT:
    avl: pointer to the AVL.
TIME COMPLEXITY:
    O(n)
*/
size_t AVLSize(const avl_t *avl);

/*
DESCRIPTION:
    Computes the current height of the AVL.
RETURN:
    Computed value.
INPUT:
    avl: pointer to the AVL.
TIME COMPLEXITY:
    O(1)
*/
size_t AVLHeight(const avl_t *avl);

#endif /* __NSRD_AVL_H__ */