/*******************************************************************************
*
* FILENAME : avl_test.c
*
* DESCRIPTION : AVL tree unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 05.06.2023
* 
*******************************************************************************/

#include <stdlib.h> /* malloc, free*/

#include "avl.h"
#include "testing.h"


static int Compare(const void *data1, const void *data2, void *params);
static int Addition(void *data, void *param);


static void TestAVLCreate(void);
static void TestAVLIsEmpty(void);
static void TestAVLSize(void);
static void TestAVLHeight(void);
static void TestAVLInsert(void);
static void TestAVLRemove(void);
static void TestAVLFind(void);
static void TestAVLForEach(void);

int main()
{
	TH_TEST_T tests[] = {
		{"Create", TestAVLCreate},
		{"IsEmpty", TestAVLIsEmpty},
		{"Size", TestAVLSize},
		{"Height", TestAVLHeight},
		{"Insert", TestAVLInsert},
		{"Remove", TestAVLRemove},
		{"Find", TestAVLFind},
		{"For each", TestAVLForEach},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestAVLCreate(void)
{
	avl_t *avl = AVLCreate(Compare, NULL);

	TH_ASSERT(NULL != avl);

	AVLDestroy(avl);
}

static void TestAVLIsEmpty(void)
{
	avl_t *avl = AVLCreate(Compare, NULL);

	TH_ASSERT(1 == AVLIsEmpty(avl));

	AVLDestroy(avl);
}

static void TestAVLSize(void)
{
	avl_t *avl = AVLCreate(Compare, NULL);

	TH_ASSERT(0 == AVLSize(avl));

	AVLDestroy(avl);
}

static void TestAVLHeight(void)
{
	avl_t *avl = AVLCreate(Compare, NULL);

	TH_ASSERT(0 == AVLHeight(avl));

	AVLDestroy(avl);
}

static void TestAVLInsert(void)
{
	int arr1[3] = {6,3,4};
	int arr2[3] = {7,9,8};

	avl_t *avl = AVLCreate(Compare, NULL);

	AVLInsert(avl, &arr1[0]);
	AVLInsert(avl, &arr1[1]);
	AVLInsert(avl, &arr1[2]);

	TH_ASSERT(2 == AVLHeight(avl));
	TH_ASSERT(3 == AVLSize(avl));

	AVLInsert(avl, &arr2[0]);
	AVLInsert(avl, &arr2[1]);
	AVLInsert(avl, &arr2[2]);

	TH_ASSERT(3 == AVLHeight(avl));
	TH_ASSERT(6 == AVLSize(avl));

	AVLDestroy(avl);
}

static void TestAVLRemove(void)
{
	int arr1[3] = {6,3,4};
	int arr2[3] = {7,9,8};

	avl_t *avl = AVLCreate(Compare, NULL);

	AVLInsert(avl, &arr1[0]);
	AVLInsert(avl, &arr1[1]);
	AVLInsert(avl, &arr1[2]);

	TH_ASSERT(2 == AVLHeight(avl));
	TH_ASSERT(3 == AVLSize(avl));

	AVLInsert(avl, &arr2[0]);
	AVLInsert(avl, &arr2[1]);
	AVLInsert(avl, &arr2[2]);

	AVLRemove(avl, &arr1[0]);
	AVLRemove(avl, &arr2[2]);
	AVLRemove(avl, &arr2[1]);

	AVLRemove(avl, &arr1[2]);

	TH_ASSERT(2 == AVLHeight(avl));
	TH_ASSERT(2 == AVLSize(avl));

	AVLDestroy(avl);
}

static void TestAVLFind(void)
{
	int arr1[3] = {6,3,4};
	int arr2[3] = {7,9,8};

	void *founded_data = NULL;

	avl_t *avl = AVLCreate(Compare, NULL);

	AVLInsert(avl, &arr1[0]);
	AVLInsert(avl, &arr1[1]);
	AVLInsert(avl, &arr1[2]);

	founded_data = AVLFind(avl, &arr1[0]);
	TH_ASSERT(6 == *(int *) founded_data);
	founded_data = AVLFind(avl, &arr1[1]);
	TH_ASSERT(3 == *(int *) founded_data);
	founded_data = AVLFind(avl, &arr1[2]);
	TH_ASSERT(4 == *(int *) founded_data);
	founded_data = AVLFind(avl, &arr2[0]);
	TH_ASSERT(NULL == founded_data);

	AVLDestroy(avl);
}

static void TestAVLForEach(void)
{
	int arr1[3] = {6,3,4};

	int test_val = 100;

	void *founded_data = NULL;

	avl_t *avl = AVLCreate(Compare, NULL);

	AVLInsert(avl, &arr1[0]);
	AVLInsert(avl, &arr1[1]);
	AVLInsert(avl, &arr1[2]);

	AVLForEach(avl, Addition, &test_val);

	founded_data = AVLFind(avl, &arr1[0]);
	TH_ASSERT(106 == *(int *) founded_data);
	founded_data = AVLFind(avl, &arr1[1]);
	TH_ASSERT(103 == *(int *) founded_data);
	founded_data = AVLFind(avl, &arr1[2]);
	TH_ASSERT(104 == *(int *) founded_data);

	AVLDestroy(avl);
}

static int Compare(const void *data1, const void *data2, void *params)
{
	if (*(int *) data1 > *(int *) data2)
	{
		return (1);
	}
	else if (*(int *) data1 < *(int *) data2)
	{
		return (-1);
	}
	else
	{
		return (0);
	}

    (void) params;
}

static int Addition(void *data, void *param)
{
	*(int *) data += *(int *) param;

	return (0);
}