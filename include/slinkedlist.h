/*******************************************************************************
*
* FILENAME : slinkedlist.h
*
* DESCRIPTION : Singly linked list is a linear collection of data elements whose
* order is not given by their physical placement in memory. Instead, each
* element points to the next. It is a data structure consisting of a collection
* of nodes which together represent a sequence. In its most basic form, each
* node contains data, and a reference (in other words, a link) to the next node
* in the sequence.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 08.04.2023
* 
*******************************************************************************/

#ifndef __SLINKEDLIST_H__
#define __SLINKEDLIST_H__

#include <stddef.h> /* size_t */

typedef struct list slist_t;
typedef struct node slist_iterator_t;

/*
DESCRIPTION
    Pointer to the function that executes an action on data using the param. 
    The actual action and types of the input are defined by the user.
RETURN
    0: success.
    non-zero value: failure.
INPUT
    data: pointer to the data;
    param: pointer to the parameter.
*/
typedef int (*action_func_t)(void *data, void *param);

/*
DESCRIPTION
    Pointer to the function that validates if the data matches a certain 
    criteria using the param.
    The actual matching and types of the input is defined by the user.
RETURN
    1: matches.
    0: not matches.
INPUT
    data: pointer to the data;
    param: pointer to the parameter.    
*/
typedef int (*is_match_func_t)(void *data, void *param);            

/*
DESCRIPTION
    Creates a singly linked list.
    Creation may fail, due to memory allocation fail. 
    User is responsible for memory deallocation.
RETURN
    Returns pointer to the created linked list on success.
    Returns NULL on failure.
INPUT
    Doesn't recieve any input from the user.
TIME_COMPLEXITY
    O(1)
*/
slist_t *SLinkedListCreate(void);

/*
DESCRIPTION
    Frees the memory allocated for each element of the singly linked list.
RETURN
    There is no return for this function.
INPUT
    list: pointer to the singly linked list.
TIME_COMPLEXITY
    O(n)
*/
void SLinkedListDestroy(slist_t *list);

/*
DESCRIPTION
    Inserts a new element to the singly linked list in the position.
RETURN
    The address of the new iterator if success.
    The iterator corresponding to the last element if insertion failed.
INPUT
    iterator: iterator to the newly inserted data
    data: pointer to the user's data.
TIME_COMPLEXITY
    O(1), O(n) if fails
*/
slist_iterator_t *SlinkedListInsert(slist_iterator_t *iterator, void *data); 

/*
DESCRIPTION
    Removes the element represented by iterator from the single linked
    list and returns the iterator representing the next element. 
    Trying to remove from an empty list may cause undefined behaviour
RETURN
    Iterator representing the next element. 
INPUT
    iterator: iterator representing the element to remove
TIME_COMPLEXITY
    O(1)
*/
slist_iterator_t *SlinkedListRemove(slist_iterator_t *iterator);

/*
DESCRIPTION
    Traverses the singly linked list and returns the amount of elements.
RETURN
    The amount of elements currently in the list. 
INPUT
    list: pointer to the singly linked list.
TIME_COMPLEXITY
    O(n)
*/
size_t SlinkedListCount(const slist_t *list);

/*
DESCRIPTION
    Searches in the single linked list in the range for the element that
    satisfies is_match function and returns the first occurance or the iterator
    representing the "to" element of the list if the elemnt isn't found. 
    The "to" element is marks the end of the searched range 
    but is not included in it.
RETURN
    If found - the iterator representing the first element in the range that 
    contains the value when it finds the value.
    If not - the iterator representing the ending element.
INPUT
    from: iterator representing the element that starts the range;
    to: iterator marking the end of the range (isn't a part of the range);
    is_match: function that checks values
    param: parameter for the is_match function.
TIME_COMPLEXITY
    O(n)
*/
slist_iterator_t *SLinkedListFind(const slist_iterator_t *from, 
    const slist_iterator_t *to, is_match_func_t is_match, void *param); 

/*
DESCRIPTION
    Traverses the linked list from one point to another and performs 
    a specific action specified by the user. The "to" element is marks 
    the end of the searched range but is not included in it.
    Actions performed might fail. 
RETURN
    0: no actions fail; 
    non-zero value: an action failed. 
INPUT
    from: iterator representing the element that starts the range;
    to: iterator marking the end of the range (isn't a part of the range);
    action: pointer to the action function;
    param: a parameter for the action function.
TIME_COMPLEXITY
    O(log n)
*/
int SlinkedListForEach(const slist_iterator_t *from, const slist_iterator_t *to, 
                        action_func_t action, void *param);

/*
DESCRIPTION
    Receives pointer to the single linked list and returns an iterator 
    that represents the beginning of the list.                                                  
RETURN
    Iterator that points to the beginning of the list.
INPUT
    list: pointer to the single linked list.
TIME_COMPLEXITY
    O(1)
*/
slist_iterator_t *SlinkedListBegin(const slist_t *list);

/*
DESCRIPTION
    Recieves pointer to the singly linked list and returns the iterator
    representing the ending element of that list. The end is 
    theoretical element which follows the last element of the list. 
    It neither contain actual data nor point to any other element.
RETURN
    Returns the iterator representing the ending element of the list.
INPUT
    list: pointer to the singly linked list.
TIME_COMPLEXITY
    O(1)
*/
slist_iterator_t *SlinkedListEnd(const slist_t *list);

/*
DESCRIPTION
    Returns the address of the iterator next to the one passed by the user.
    Passing an iterator received from the function SLinkedListEnd may cause
    undefined behavior.
RETURN
    Function returns pointer to the iterator
INPUT
    iterator: pointer to the iterator.
TIME_COMPLEXITY
    O(1)
*/
slist_iterator_t *SlinkedListNext(slist_iterator_t *iterator);

/*
DESCRIPTION
    Set data in the element represented by the given iterator.
    Trying to put data in the element representing the end of the list 
    may result in undefined behavior.
RETURN
    Function returns pointer to the iterator
INPUT
    iterator: pointer to the iterator.
TIME_COMPLEXITY
    O(1)
*/
slist_iterator_t *SlinkedListSetData(slist_iterator_t *iterator, void *data); 

/*
DESCRIPTION
    Get data from the element represented by the given iterator.
RETURN
    Return a generic pointer to data.
INPUT
    iterator: pointer to the iterator.
TIME_COMPLEXITY
    O(1)
*/
void *SlinkedListGetData(slist_iterator_t *iterator);

/*
DESCRIPTION
    Compares two iterators to check if they are the same.
RETURN
    1: iterators are the same.
    0: iterators are not the same.
INPUT
    iterator1: iterator representing an element in the list.
    iterator2: iterator representing an element in the list.
TIME_COMPLEXITY
    O(1)
*/
int SlinkedListIsSameIterator(slist_iterator_t *iterator1, 
                            slist_iterator_t *iterator2);

/*
DESCRIPTION
    Appends src to dest. After merge dest will contain all the nodes, and src
    will be empty.
RETURN
    Returns pointer to the dest merged list.
INPUT
    dest: pointer to the singly linked list where to append.
    src: pointer to the singly linked list what to append.
TIME_COMPLEXITY
    O(1)
*/
void SlinkedListAppend(slist_t *dest, slist_t *src);

#endif  /* __SLINKEDLIST_H__ */