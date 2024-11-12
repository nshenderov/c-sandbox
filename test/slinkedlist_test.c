/*******************************************************************************
*
* FILENAME : slinkedlist_test.c
*
* DESCRIPTION : Single linked list unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 08.04.2023
*
*******************************************************************************/

#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "slinkedlist.h"
#include "testing.h"


static int EqualsInt(void *data, void *param);
static int AddInt(void *data, void *param);


static void TestList(void);

int main()
{
    TH_TEST_T TESTS[] = {
		{"List", TestList},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(TESTS);

	return (0);
}

static void TestList(void)
{
	int t1 = 123;
	float t2 = 99.123;
	double t3 = 3.22;
	int t4 = 0;
	int t5 = 100;
	int t6 = 20200;

	slist_iterator_t *list_head = NULL;
	slist_iterator_t *list_head_next = NULL;
	slist_iterator_t *list_end = NULL;
	slist_iterator_t *search_res = NULL;

    slist_t *list = SLinkedListCreate();

	list_head = SlinkedListBegin(list);
    TH_ASSERT(0 == SlinkedListCount(list));
    TH_ASSERT(NULL == SlinkedListNext(list_head));
	
	SlinkedListInsert(list_head, &t1);
    TH_ASSERT(t1 == *(int *) SlinkedListGetData(list_head));
	SlinkedListInsert(list_head, &t2);
    TH_ASSERT(t2 == *(float *) SlinkedListGetData(list_head));
	SlinkedListInsert(list_head, &t3);
	TH_ASSERT(t3 == *(double *) SlinkedListGetData(list_head));

    TH_ASSERT(3 == SlinkedListCount(list));

	list_end = SlinkedListEnd(list);
    TH_ASSERT(NULL == SlinkedListNext(list_end));

	SlinkedListInsert(list_end, &t1);
	TH_ASSERT(t1 == *(int *) SlinkedListGetData(list_end));

    TH_ASSERT(NULL == SlinkedListNext(SlinkedListNext(list_end)));
    
	TH_ASSERT(4 == SlinkedListCount(list));

	list_head_next = SlinkedListNext(list_head);
    TH_ASSERT(t2 == *(float *) SlinkedListGetData(list_head_next));

	list_head_next = SlinkedListNext(list_end);
    TH_ASSERT(NULL == SlinkedListNext(list_head_next));

	list_head = SlinkedListRemove(list_head);
    TH_ASSERT(3 == SlinkedListCount(list));
    TH_ASSERT(t2 == *(float *) SlinkedListGetData(list_head));

	list_head = SlinkedListRemove(list_head);
    TH_ASSERT(2 == SlinkedListCount(list));
    TH_ASSERT(t1 == *(int *) SlinkedListGetData(list_head));

	list_head = SlinkedListRemove(list_head);
	list_head = SlinkedListRemove(list_head);
    TH_ASSERT(0 == SlinkedListCount(list));

	for (; t4 < 200; ++t4)
	{
		SlinkedListInsert(list_head, &t4);
	}

    TH_ASSERT(200 == SlinkedListCount(list));

	list_end = SlinkedListEnd(list);
	SlinkedListForEach(list_head, list_end, AddInt, &t5);

	search_res = SLinkedListFind(list_head, list_end, EqualsInt, &t6);
    TH_ASSERT(t6 == *(int *) SlinkedListGetData(search_res));
    TH_ASSERT(NULL != SlinkedListNext(search_res));

	SlinkedListSetData(list_head, &t1);
    TH_ASSERT(t1 == *(int *) SlinkedListGetData(list_head));

	SlinkedListSetData(list_head, &t2);
	TH_ASSERT(t2 == *(float *) SlinkedListGetData(list_head));

	search_res = SLinkedListFind(list_head, list_end, EqualsInt, &t2);
    TH_ASSERT(t2 == *(float *) SlinkedListGetData(search_res));

	search_res = SLinkedListFind(list_head, list_end, EqualsInt, &t3);
	TH_ASSERT(NULL == SlinkedListNext(search_res));

	list_head = SlinkedListEnd(list);
	list_end = SlinkedListEnd(list);

    TH_ASSERT(1 == SlinkedListIsSameIterator(list_head, list_end));

	SLinkedListDestroy(list);
}

static int EqualsInt(void *data, void *param)
{
	if (*(int *) data == *(int *) param)
	{
		return (1);
	}

	return (0);
}

static int AddInt(void *data, void *param)
{
	*(int *) data += *(int *) param;
	return (0);
}