/*******************************************************************************
*
* FILENAME : slinkedlist.c
*
* DESCRIPTION : Single linked list implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 08.04.2023
* 
* PUBLIC FUNCTIONS :
*       slist_t *SLinkedListCreate(void)
*       void SLinkedListDestroy(slist_t *list)
*       slist_iterator_t SlinkedListInsert(slist_iterator_t iterator,
                                                            void *data)
*       slist_iterator_t SlinkedListRemove(slist_iterator_t iterator)
*       size_t SlinkedListCount(const slist_t *list)
*       slist_iterator_t SlinkedListBegin(const slist_t *list)
*       slist_iterator_t SlinkedListEnd(const slist_t *list)
*       slist_iterator_t SlinkedListNext(slist_iterator_t iterator)
*       slist_iterator_t SlinkedListSetData(slist_iterator_t iterator, 
                                                            void *data)
*       void *SlinkedListGetData(slist_iterator_t iterator)
*       slist_iterator_t SLinkedListFind(const slist_iterator_t from, 
*              const slist_iterator_t to, is_match_func_t is_match, void *param)
*       int SlinkedListForEach(const slist_iterator_t from,
*                  const slist_iterator_t to, action_func_t action, void *param)
*       int SlinkedListIsSameIterator(slist_iterator_t iterator1, 
*               slist_iterator_t iterator2)
*
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "slinkedlist.h"

typedef struct node node_t;

struct node
{
    void *data;
    struct node *next_node;  
};

struct list
{
    struct node *head_node;
    struct node *tail_node;
};


#define FREE_MEMORY(ptr) \
{free(ptr); (ptr) = NULL;}

static node_t *CreateNode(void *data, node_t *next_node);
static slist_iterator_t *GetToEnd(slist_iterator_t *iterator);
static int IsEnd(slist_iterator_t *iterator);
static void ListUpdate(slist_iterator_t *iterator);
static void CopyNode(node_t *dest, node_t* src);
static int Counter(void *data, void *param);

slist_t *SLinkedListCreate(void)
{ 
    node_t *dummy_node = NULL;

    slist_t *new_list = (slist_t *) malloc(sizeof(slist_t));
    if (NULL == new_list)
    {
        return (NULL);
    }

    dummy_node = CreateNode(new_list, NULL);
    if (NULL == dummy_node)
    {
        FREE_MEMORY(new_list);
        return (NULL);
    }

    new_list -> head_node = dummy_node;
    new_list -> tail_node = dummy_node;

    return (new_list);
}

slist_iterator_t *SlinkedListInsert(slist_iterator_t *iterator, void *data)
{
    node_t *new_node = NULL;

    assert(NULL != iterator);

    new_node = CreateNode(iterator -> data, iterator -> next_node);
    if (NULL == new_node)
    {
       return (GetToEnd(iterator));
    }

    iterator -> data = data;
    iterator -> next_node = new_node;

    if (IsEnd(new_node))
    {
        ListUpdate(new_node);
    }

    return (iterator);
}

slist_iterator_t *SlinkedListRemove(slist_iterator_t *iterator)
{
    node_t *next_node = NULL;

    assert(NULL != iterator);

    next_node = iterator -> next_node;

    CopyNode(iterator, next_node);

    FREE_MEMORY(next_node);

    if (IsEnd(iterator))
    {
        ListUpdate(iterator);
    }

    return (iterator);
}

size_t SlinkedListCount(const slist_t *list)
{
    size_t counter = 0;

    assert(NULL != list);

    SlinkedListForEach(SlinkedListBegin(list),SlinkedListEnd(list), Counter,
                                                                   &counter);

    return (counter);
}

void SLinkedListDestroy(slist_t *list)
{
    node_t *runner = NULL;
    node_t *tmp = NULL;

    assert(NULL != list);
    assert(NULL != list -> head_node);

    runner = list -> head_node;

    while(NULL != runner -> next_node)
    {
        tmp = runner;
        runner = runner -> next_node;
        FREE_MEMORY(tmp);
    }

    FREE_MEMORY(runner);
    FREE_MEMORY(list);
}

slist_iterator_t *SLinkedListFind(const slist_iterator_t *from, 
    const slist_iterator_t *to, is_match_func_t is_match, void *param)
{
    node_t *from_runner = NULL;
    node_t *to_runner = NULL;
   
   assert(NULL != from);
   assert(NULL != to);
   assert(NULL != is_match);

    from_runner = (node_t *) from;
    to_runner = (node_t *) to;

    while(!SlinkedListIsSameIterator(from_runner, to_runner))
    {
        if (is_match(from_runner -> data, param))
        {
            return (from_runner);
        }

        from_runner = from_runner -> next_node;
    }

    return (to_runner);
}

int SlinkedListForEach(const slist_iterator_t *from, const slist_iterator_t *to, 
                                              action_func_t action, void *param)
{
    node_t *from_runner = NULL;
    node_t *to_runner = NULL;

   assert(NULL != from);
   assert(NULL != to);
   assert(NULL != action);

    from_runner = (node_t *) from;
    to_runner = (node_t *) to;

    while(!SlinkedListIsSameIterator(from_runner, to_runner))
    {
        if (0 != action(from_runner -> data, param))
        {
            return (1);
        }
        from_runner = from_runner -> next_node;
    }

    return (0);
}

slist_iterator_t *SlinkedListBegin(const slist_t *list)
{
    assert(NULL != list);

    return (list -> head_node);
}

slist_iterator_t *SlinkedListEnd(const slist_t *list)
{
    assert(NULL != list);

    return (list -> tail_node);
}

slist_iterator_t *SlinkedListNext(slist_iterator_t *iterator)
{
    assert(NULL != iterator);

    iterator = iterator -> next_node;

    return (iterator);
}

void *SlinkedListGetData(slist_iterator_t *iterator)
{
    assert(NULL != iterator);

    return (iterator -> data);
}

slist_iterator_t *SlinkedListSetData(slist_iterator_t *iterator, void *data)
{
    assert(NULL != iterator);

    iterator -> data = data;

    return (iterator);
}

int SlinkedListIsSameIterator(slist_iterator_t *iterator1, 
                                                    slist_iterator_t *iterator2)
{
    assert(NULL != iterator1);
    assert(NULL != iterator2);

    if (iterator1 -> data == iterator2 -> data 
        && iterator1 -> next_node == iterator2 -> next_node)
    {
        return (1);
    }

    return (0);
}

void SlinkedListAppend(slist_t *dest, slist_t *src)
{
    assert(NULL != dest);
    assert(NULL != src);

    CopyNode(dest -> tail_node, src -> head_node);

    src -> tail_node -> data = dest;
    dest -> tail_node  = src -> tail_node;

    src -> head_node -> data = src;
    src -> head_node -> next_node = NULL;

    src -> tail_node = src -> head_node; 
}


static node_t *CreateNode(void *data, node_t *next_node)
{
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    if (NULL == new_node)
    {
        return (NULL);
    }

    new_node -> data = data;
    new_node -> next_node = next_node;

    return (new_node);
}

static slist_iterator_t *GetToEnd(slist_iterator_t *iterator)
{
    assert(NULL != iterator);   

    while (NULL != iterator -> next_node)
    {
        iterator = iterator -> next_node;
    }
    
    return (iterator);
}

static int IsEnd(slist_iterator_t *iterator)
{
    return (NULL == iterator -> next_node);
}


static void ListUpdate(slist_iterator_t *iterator)
{
    slist_t *list_ptr = NULL;
    
    assert(NULL != iterator);
    
    list_ptr = iterator -> data;
    list_ptr -> tail_node = iterator;
}

static void CopyNode(node_t *dest, node_t* src)
{
    assert(NULL != dest);
    assert(NULL != src);
    
    dest -> data = src -> data;
    dest -> next_node = src -> next_node; 
}


static int Counter(void *data, void *param)
{
    *(size_t *) param += 1;

    return (0);
    (void) data;
}