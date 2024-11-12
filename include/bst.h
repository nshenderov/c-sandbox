/*******************************************************************************
*
* FILENAME : bst.h
*
* DESCRIPTION : Binary search tree (BST) is an abstract data type (ADT) that
* organizes its elements in a hierarchical manner, allowing for efficient
* search, insertion, and deletion operations. It is a type of binary tree where
* each node has at most two children, referred to as the left child and the
* right child.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 28.05.2023
* 
*******************************************************************************/

#ifndef __NSRD_BST_H__
#define __NSRD_BST_H__

#include <stddef.h> /* size_t */

typedef struct bst bst_t;
typedef struct bst_node *bst_iter_t;


/*
DESCRIPTION
	Pointer to the user's function that compares data1 and data2.
	The actual matching and types of the input are defined by the user.
	Elements will be in BST in ascending order based on this function.
RETURN
	1: data1 > data2.
	-1: data1 < data2.
	0: data1 == data2.
INPUT
	data1: pointer to user's data.
	data2: pointer to user's data.
	params: pointer to user's params.
*/
typedef int(*bst_compare_t)(const void *data1, const void *data2, void *params);

/*
DESCRIPTION
	Pointer to the user's function that executes the action on data using the
	param. The actual action and types of the input are defined by the user.
RETURN
	0: success.
	non-zero: fail.
INPUT
	data: pointer to user's data.
	params: pointer to a some parameter for action.
*/
typedef int(*bst_action_t)(void *data, void *params);

/*
DESCRIPTION
    Creates new BST. Tree will use pattern of sorting, based on user's
    compare function - "compare".
    Creation may fail, due to memory allocation fail.
    User is responsible for memory deallocation.
RETURN
    Pointer to the created BST on success.
    NULL if allocation failed.
INPUT
    compare: pointer to the compare function.
    params: pointer to user's params.
TIME COMPLEXITY
    O(1)
*/
bst_t *BSTCreate(bst_compare_t compare, void *params);

/*
DESCRIPTION
    Frees the memory allocated for each element of the BST and the BST itself.
    All data inside the BST will be lost.
    User is responsible for handling dangling pointers after destroy.
RETURN
    Doesn't return anything.
INPUT
    bst: pointer to the BST.
TIME COMPLEXITY
    O(n)
*/
void BSTDestroy(bst_t *bst);

/*
DESCRIPTION
    Traverses the BST and returns the amount of elements.
RETURN
    Number of elements in the BST.
INPUT
    bst: pointer to the BST.
TIME COMPLEXITY
    O(n)
*/
size_t BSTSize(const bst_t *bst);

/*
DESCRIPTION
    Checks if BST is empty.
RETURN
    1: BST is empty.
    0: BST is not empty.
INPUT
    bst: pointer to the BST.
TIME COMPLEXITY
    O(1)
*/
int BSTIsEmpty(const bst_t *bst);

/*
DESCRIPTION
    Inserts new element to the BST on the position, according to saved in BST
    pointers on compare function and params.
    Trying to insert element with equal values according to compare function
    may cause undefined behavior.
    Insertion may fail, due to memory allocation fail.
RETURN
    Iterator representing new element if success.
    Iterator representing the end of a BST (theoretical element) if fail.
INPUT
    bst: pointer to the BST.
    data: pointer to the user's data.
TIME COMPLEXITY
    worst O(n), avg: O(log n)
*/
bst_iter_t BSTInsert(bst_t *bst, void *data);

/*
DESCRIPTION
    Removes the element represented by the iterator from the BST.
    Passing an iterator received from the BSTEnd() function may cause
    undefined behavior.
RETURN
    Iterator representing the next element.
INPUT
    iter: iterator representing the element to remove.
TIME COMPLEXITY
    worst O(n), avg: O(log n)
*/
bst_iter_t BSTRemove(bst_iter_t iter);

/*
DESCRIPTION
    Returns an iterator representing the first element in passed BST.
RETURN
    Iterator representing the first element in BST.
INPUT
    bst: a pointer to a BST.
TIME COMPLEXITY
    worst O(n), avg: O(log n)
*/
bst_iter_t BSTBegin(const bst_t *bst);

/*
DESCRIPTION
    Returns the iterator representing the end of passed BST.
    The end is theoretical element which follows the last element of the BST.
    It doesn't contain actual data.
RETURN
    Iterator representing the the end of a BST (theoretical element).
INPUT
    bst: pointer to the BST.
TIME COMPLEXITY
    O(1)
*/
bst_iter_t BSTEnd(const bst_t *bst);

/*
DESCRIPTION
    Returns the iterator representing the previous element in BST.
    Passing an iterator received from the BSTBegin() function may cause
    undefined behavior.
RETURN
    Iterator representing the previous element.
INPUT
    iter: iterator representing the element.
TIME COMPLEXITY
    worst O(n), avg: O(log n)
*/
bst_iter_t BSTPrev(bst_iter_t iter);

/*
DESCRIPTION
    Returns the iterator representing the next element in BST.
    Passing an iterator received from the BSTEnd() function may cause
    undefined behavior.
RETURN
    Iterator representing the next element.
INPUT
    iter: iterator representing the element.
TIME COMPLEXITY
    worst O(n), avg: O(log n)
*/
bst_iter_t BSTNext(bst_iter_t iter);

/*
DESCRIPTION
    Compares two iterators to check if they are the same.
RETURN
    1: iterators are the same.
    0: iterators are not the same.
INPUT
    iterator1: iterator representing an element in the BST.
    iterator2: iterator representing another element in the BST.
TIME COMPLEXITY
    O(1)
*/
int BSTIsSameIterator(bst_iter_t iterator1, bst_iter_t iterator2);

/*
DESCRIPTION
    Get data from the element represented by the given iterator.
    Data is stored in BST by reference.
    Passing an iterator received from the BSTEnd() function may cause
    undefined behavior. 
RETURN
    Pointer to user's data inside element.
INPUT
    iter: iterator representing element in the BST.
TIME COMPLEXITY
    O(1)
*/
void *BSTGetData(bst_iter_t iter);

/*
DESCRIPTION
    Traverses the BST with binary search for element that corresponds to the
    passed data according to the BST compare function.
RETURN
    The iterator representing found element.
    If nothing found - iterator representing end of BST (theoretical element).
INPUT
    bst: pointer to the BST.
    data: pointer to the user's data.
TIME COMPLEXITY
    worst O(n), avg: O(log n)
*/
bst_iter_t BSTFind(bst_t *bst, void *key);

/*
DESCRIPTION
    Traverses the BST in a certain range for all elements and call user's
    action function passing element's data and param as input.
    The "to" element is marks the end of the range, but is not included in it.
    Passing "from" and "to" from different lists may cause undefined behavior.
    User is responsible for sequences on action func results, cause changing
    compared values (keys) may cause undefined behavior.
    Actions perfomed might fail.
RETURN
    0: no actions fail.
    non-zero: at least one action fail.
INPUT
    from: iterator representing the element that starts the range.
    to: iterator marking the end of the range (isn't a part of the range).
    action: user's function that executes an action.
    param: parameter for the is_match function.
TIME COMPLEXITY
    O(n)
*/
int BSTForeach(bst_iter_t from, bst_iter_t to,bst_action_t action,void *params);

#endif /* __NSRD_BST_H__ */     