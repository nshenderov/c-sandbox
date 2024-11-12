/*******************************************************************************
*
* FILENAME : hash_table.h
*
* DESCRIPTION : Hash table is a data structure that implements an associative
* array, also called a dictionary or simply map, an associative array is an
* abstract data type that maps keys to values. A hash table uses a hash
* function to compute an index, also called a hash code, into an array of
* buckets or slots, from which the desired value can be found. During lookup,
* the key is hashed and the resulting hash indicates where the corresponding
* value is stored. A map implemented by a hash table is called a hash map.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 05.06.2023
* 
*******************************************************************************/

#ifndef __NSRD_HASH_TABLE_H__
#define __NSRD_HASH_TABLE_H__

#include <stddef.h> /* size_t */

typedef struct h_table h_table_t;


/*
DESCRIPTION
	Pointer to the function that executes an action on data using param. 
	The actual action and types of the input are defined by the user.
RETURN
	0: success.
	non-zero value: failure.
INPUT
	data: pointer to data.
	param: pointer to the parameter.
*/
typedef int (*h_table_action_t)(void *data, void *param);

/*
DESCRIPTION:
    Pointer to the function that compares two key-value pairs.
RETURN:
    1: same.
    0: not.
INPUT:
    data1: pointer to the key-value pair.
    data2: pointer to the key-value pair.
*/
typedef int (*h_table_is_match_t)(const void *data1, const void *data2);

/*
DESCRIPTION:
	Pointer to the function that extract the key out of the user's data and
    hashes it to use for indexing the hash table. The hashing method is defined
    by the user.
RETURN:
    The calculated hash of the key.
INPUT:
    data: pointer to the key-value pair.
*/
typedef size_t (*h_table_hash_t)(const void *data);



/*
DESCRIPTION:
    Creates a hash table with the specified table size, 
    hash function, and key matching function.
    Creation may fail if memory allocation fails. 
    User is responsible for memory deallocation.
RETURN:
    Returns pointer to the created hash table on success.
    Returns NULL on failure.
INPUT:
    table_size: the size of the hash table.`
    hash_func: function that calculates the hash value for a given data
    element.
    is_match: function that checks if two data elements are considered a match 
    based on their keys.
TIME COMPLEXITY:
    O(n)
*/
h_table_t *HashTableCreate(size_t table_size, h_table_hash_t hash_func, h_table_is_match_t is_match);

/*
DESCRIPTION:
    Destroys the specified hash table, freeing the memory allocated for it and
    its elements.
RETURN:
    There is no return for this function.
INPUT:
    table: pointer to the hash table to be destroyed.
TIME COMPLEXITY:
    O(n)
*/
void HashTableDestroy(h_table_t *table);

/*
DESCRIPTION:
    Finds the element in the hash table based on the specified key.
RETURN:
    Pointer to the found data element if success.
    Returns NULL if the key is not found.
INPUT:
    table: pointer to the hash table.
    key: pointer to the key of the element to find.
TIME COMPLEXITY:
    O(1) - best, O(n) - worst
*/
void *HashTableFind(h_table_t *table, void *key);

/*
DESCRIPTION:
    Inserts a new data element into the hash table.
    If the key already exists in the hash table, overwrites old data.
RETURN:
    Returns 0 if the insertion is successful.
    Returns a non-zero value if an error occurs.
INPUT:
    table: pointer to the hash table.
    data: pointer to the data element to be inserted.
TIME COMPLEXITY:
    O(1) - best, O(n) - worst
*/
int HashTableInsert(h_table_t *table, void *data);

/*
DESCRIPTION:
    Removes the element with the specified key from the hash table.
RETURN:
    There is no return for this function.
INPUT:
    table: pointer to the hash table.
    key: pointer to the key of the element to remove.
TIME COMPLEXITY:
    O(1) - best, O(n) - worst
*/
void HashTableRemove(h_table_t *table, void *key);

/*
DESCRIPTION
    Returns the number of elements in the hash table.
RETURN
    The number of elements in the hash table.
INPUT
    table: pointer to the hash table.
TIME COMPLEXITY:
    O(n)
*/
size_t HashTableSize(const h_table_t *table);

/*
DESCRIPTION
    Checks whether the hash table is empty.
RETURN
    1: the hash table is empty.
    0: the hash table is not empty.
INPUT
    table: pointer to the hash table.
TIME COMPLEXITY:
    O(n)
*/
int HashTableIsEmpty(const h_table_t *table);

/*
DESCRIPTION
	Traverses the hash table and performs the action function on each element.
	Actions may fail.
	User should use this function responsibly because changing the data may 
	invalidate the hashed indexes.
RETURN
	0: all actions are successful;
	1: any action fails.
INPUT
	table: pointer to the hash table;
	action: pointer to the action function;
	param: generic pointer to the parameters for the action function.
TIME COMPLEXITY:
    O(n)
*/
int HashTableForEach(h_table_t *table, h_table_action_t action, void *param);

#endif /* __NSRD_HASH_TABLE_H__ */