/*******************************************************************************
*
* FILENAME : hash_table.c
*
* DESCRIPTION : Hash table implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 05.06.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "dlist.h"
#include "hash_table.h"

#define SIZE_OF_HT_STRUCT (sizeof(struct h_table))
#define COMPUTE_SIZE_OF_BUCKET(SIZE) (sizeof(void *) * SIZE)
#define COMPUTE_INDEX(TABLE, DATA) (TABLE->hash_func(DATA) % TABLE->num_buckets)
#define FIND_LIST(TABLE, DATA) (table->bucket[COMPUTE_INDEX(TABLE, DATA)])

enum {SUCCESS, FAILURE};

struct h_table
{
    size_t num_buckets;
    h_table_hash_t hash_func;
    h_table_is_match_t is_match;
    dlist_t **bucket;
};

static void CreateBuckets(h_table_t *table);
static void DestroyBuckets(h_table_t *table);

h_table_t *HashTableCreate(size_t table_size, h_table_hash_t hash_func,
                                            h_table_is_match_t is_match)
{
    h_table_t *new_ht = NULL;

    assert(NULL != hash_func);
    assert(NULL != is_match);
    assert(0 < table_size);

    new_ht =
    (h_table_t *) malloc(SIZE_OF_HT_STRUCT + COMPUTE_SIZE_OF_BUCKET(table_size));

    new_ht->hash_func = hash_func;
    new_ht->is_match = is_match;
    new_ht->num_buckets = table_size;
    new_ht->bucket = (dlist_t **) (new_ht + 1);

    CreateBuckets(new_ht);

    return (new_ht);
}

void HashTableDestroy(h_table_t *table)
{
    assert(NULL != table);

    DestroyBuckets(table);

    free(table);
    table = NULL;
}

void *HashTableFind(h_table_t *table, void *key)
{
    dlist_t *founded_list = NULL;
    dlist_iterator_t list_begin = NULL;
    dlist_iterator_t list_end = NULL;
    dlist_iterator_t founded_item = NULL;

    assert(NULL != table);
    assert(NULL != key);

    founded_list = FIND_LIST(table, key);

    list_begin = DListBegin(founded_list);
    list_end = DListEnd(founded_list);

    founded_item =
    DListFind(list_begin,list_end, (dlist_is_match_func_t) table->is_match,key);

    if (DListIsSameIterator(founded_item, list_end))
    {
        return (NULL);
    }

    if (!DListIsSameIterator(founded_item, list_begin))
    {
        DListSplice(list_begin, founded_item, DListNext(founded_item));
    }

    return (DListGetData(founded_item));
}

int HashTableInsert(h_table_t *table, void *data)
{
    dlist_t *list_to_insert_to = NULL;
    dlist_iterator_t list_begin = NULL;
    dlist_iterator_t list_end = NULL;
    dlist_iterator_t founded_item = NULL;

    assert(NULL != table);
    assert(NULL != data);

    list_to_insert_to = FIND_LIST(table, data);

    list_begin = DListBegin(list_to_insert_to);
    list_end = DListEnd(list_to_insert_to);

    founded_item =
    DListFind(list_begin,list_end, (dlist_is_match_func_t)table->is_match,data);
    
    if (DListIsSameIterator(founded_item, list_end))
    {
        if(DListIsSameIterator(DListPushBack(list_to_insert_to,data), list_end))
        {
            return (FAILURE);
        }
    }
    else
    {
        DListSetData(founded_item, data);
    }

    return (SUCCESS);
}

void HashTableRemove(h_table_t *table, void *key)
{
    dlist_t *founded_list = NULL;
    dlist_iterator_t list_begin = NULL;
    dlist_iterator_t list_end = NULL;
    dlist_iterator_t founded_item = NULL;

    assert(NULL != table);
    assert(NULL != key);

    founded_list = FIND_LIST(table, key);

    list_begin = DListBegin(founded_list);
    list_end = DListEnd(founded_list);

    founded_item =
    DListFind(list_begin, list_end, (dlist_is_match_func_t)table->is_match,key);

    if (DListIsSameIterator(founded_item, list_end))
    {
        return;
    }
    else
    {
        DListRemove(founded_item);
    }

}

size_t HashTableSize(const h_table_t *table)
{
    size_t table_size = 0;
    size_t counter = 0;
    size_t i = 0;

    assert(NULL != table);

    table_size = table->num_buckets;
    
    for (; i < table_size; ++i)
    {
        counter += DListSize(table->bucket[i]);
    }

    return (counter);
}

int HashTableIsEmpty(const h_table_t *table)
{
    size_t table_size = table->num_buckets;
    size_t i = 0;
    int flag = 1;

    assert(NULL != table);

    for (; i < table_size && flag; ++i)
    {
        flag = DListIsEmpty(table->bucket[i]);
    }

    return (flag);
}

int HashTableForEach(h_table_t *table, h_table_action_t action, void *param)
{
    dlist_t *founded_list = NULL;
    dlist_iterator_t list_begin = NULL;
    dlist_iterator_t list_end = NULL;
    size_t table_size = 0;
    size_t i = 0;
    int status = 0;

    assert(NULL != table);
    assert(NULL != action);

    table_size = table->num_buckets;

    for (;i < table_size && 0 == status; ++i)
    {
        founded_list = table->bucket[i];
        list_begin = DListBegin(founded_list);
        list_end = DListEnd(founded_list);
        status = DListForEach(list_begin, list_end, action, param);
    }

    return (status);
}

static void CreateBuckets(h_table_t *table)
{
    size_t i = 0;
    size_t table_size = 0;
    dlist_t *new_list = NULL;

    assert(NULL != table);

    table_size = table->num_buckets;

    for (; i < table_size; ++i)
    {
        new_list = DListCreate();

        if (NULL == new_list)
        {
            table->num_buckets = i;
            HashTableDestroy(table);
            return;
        }

        table->bucket[i] = new_list;
    }
}

static void DestroyBuckets(h_table_t *table)
{
    size_t i = 0;
    size_t table_size = 0;

    assert(NULL != table);

    table_size = table->num_buckets;

    for (; i < table_size; ++i)
    {
        DListDestroy(table->bucket[i]);
    }
}