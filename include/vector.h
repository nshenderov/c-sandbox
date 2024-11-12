/*******************************************************************************
*
* FILENAME : vector.h
*
* DESCRIPTION : Vector is a dynamic array data structure that can grow or
* shrink in size. It provides efficient random access to elements and allows for
* insertion and deletion at the end in constant time. Unlike static arrays,
* vectors automatically resize themselves when their capacity is exceeded,
* making them useful for collections of variable size.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 04.04.2023
* 
*******************************************************************************/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h> /* size_t */

typedef struct dynamic_vector vector_t;

/*
DESCRIPTION
    Creates vector of the defined capacity and element size 
	and allocates memory. Function may fail during memory
	allocation. User is responsible for memory deallocation.
RETURN
	Function returns a pointer to a vector or NULL if it fails
	to allocate data.
INPUT
    capacity: number of elements in the vector.
    elem_size: size of a single element in bytes.
*/
vector_t *VectorCreate(size_t capacity, size_t element_size);

/*
DESCRIPTION
	Frees the memory allocated for the vector.
RETURN
	There is no return for this function.
INPUT
    vector: pointer to the vector.
*/
void VectorDestroy(vector_t *vector);

/*
DESCRIPTION
    Function returns the number of elements in the vector.
RETURN
    Number of elements in the vector
INPUT
    vector: pointer to the a vector
*/
size_t VectorSize(const vector_t *vector);

/*
DESCRIPTION
	Function returns the current capacity of the vector.
RETURN
    Number of elements the vector can store at the moment.
INPUT
	vector: pointer to the the vector.	
*/
size_t VectorCapacity(const vector_t *vector);

/*
DESCRIPTION
    Function returns the element at index. Trying to access the index
    that exceeds size may result in undefined behavior.
RETURN
    A pointer to the element.
INPUT
    vector: pointer to the vector
    index: index of the element
*/
void *VectorGetElement(const vector_t *vector, size_t index);


/*
DESCRIPTION
    Adds the element to the end of the vector. If the element is larger than 
    the element size of the vector, it will be truncated. Resizes the vector if
    it reaches its capacity. The function may fail due to vector resize failure.
RETURN
    0: success;
    1: failure.
INPUT
    vector: pointer to the vector.
    element: pointer to the element to insert.
*/
int VectorPushBack(vector_t *vector, const void *element);

/*
DESCRIPTION
    Removes the element from the end of the vector. Trying to remove the element 
    from the empty vector may result in underfinded behavior.
RETURN
	There is no return for this function.
INPUT
    vector: pointer to the vector.
*/
void VectorPopBack(vector_t *vector);

/*
DESCRIPTION
    Reeduces the capacity of the vector to the current size Reallocation of
    the memory may fail.
RETURN
    0: reallocation is successful;
    1: reallocation is not succeful.
INPUT
    vector: pointer to the vector;
*/
int VectorShrink(vector_t *vector); 

/*
DESCRIPTION
    Expands the capacity of the vector according to the user's needs.
    Reallocation of the memory may fail.
RETURN
    0: reallocation is successful;
    1: reallocation is not succeful.
INPUT
    vector: pointer to the vector;
    new_size: new size of the vector.
*/
int VectorReserve(vector_t *vector, size_t new_size); 

#endif /* __VECTOR_H__ */
