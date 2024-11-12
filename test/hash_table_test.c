/*******************************************************************************
*
* FILENAME : hash_table_test.c
*
* DESCRIPTION : Hash table unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 05.06.2023
* 
*******************************************************************************/

#include <stdlib.h> /* malloc, free*/
#include <time.h> /* time_t, clock */
#include <string.h> /* printf */

#include "hash_table.h"
#include "testing.h"


typedef struct
{
	char *key;
	void *value;
} keyval_entity_t;

typedef struct
{
	int key;
	int value;
} keyval_entity_caching_t;

typedef struct dictionary_entity
{
	char key[100];
	char value[100];
} dictionary_entity_t;

static int IsMatch(const void *data1, const void *data2);
static int IsMatchDictionary(const void *data1, const void *data2);
static int IsMatchCashing(const void *data1, const void *data2);
static size_t HashDjb2(const void *data);
static size_t HashDjb2Cashing(const void *data);
static size_t HashDjb2Dictionary(const void *data);
static int AdditionAction(void *data, void *param);
static void HTInsertKeyValEnts(h_table_t *ht, keyval_entity_t *arr, size_t size);


static void TestHTCreate(void);
static void TestHTIsEmpty(void);
static void TestHTSize(void);
static void TestHTInsert(void);
static void TestHTRemove(void);
static void TestHTFind(void);
static void TestHTForEach(void);
static void TestHTCaching(void);
static void TestHTSpellChecker(void);

int main()
{
	TH_TEST_T tests[] = {
		{"Create", TestHTCreate},
		{"IsEmpty", TestHTIsEmpty},
		{"Size", TestHTSize},
		{"Insert", TestHTInsert},
		{"Remove", TestHTRemove},
		{"Find", TestHTFind},
		{"ForEach", TestHTForEach},
		{"Caching", TestHTCaching},
		{"SpellChecker", TestHTSpellChecker},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestHTCreate(void)
{
	h_table_t *ht = HashTableCreate(100, HashDjb2, IsMatch);

	TH_ASSERT(NULL != ht);

	HashTableDestroy(ht);
}

static void TestHTIsEmpty(void)
{
	h_table_t *ht = HashTableCreate(100, HashDjb2, IsMatch);

	TH_ASSERT(1 == HashTableIsEmpty(ht));

	HashTableDestroy(ht);
}

static void TestHTSize(void)
{
	h_table_t *ht = HashTableCreate(100, HashDjb2, IsMatch);

	TH_ASSERT(0 == HashTableSize(ht));

	HashTableDestroy(ht);
}

static void TestHTInsert(void)
{
	h_table_t *ht = HashTableCreate(100, HashDjb2, IsMatch);

	int test_nums[6] = {1001, 1002, 1003, 1004, 1005, 9999};
	keyval_entity_t test_ents[6] = {
		{"HELLO_1", NULL},
		{"HELLO_2", NULL},
		{"HELLO_3", NULL},
		{"HELLO_4", NULL},
		{"HELLO_5", NULL},
		{"HELLO_6", NULL},
	};

	test_ents[0].value = &test_nums[0];
	test_ents[1].value = &test_nums[1];
	test_ents[2].value = &test_nums[2];
	test_ents[3].value = &test_nums[3];
	test_ents[4].value = &test_nums[4];
	test_ents[5].value = &test_nums[5];

	TH_ASSERT(NULL != ht);
	TH_ASSERT(1 == HashTableIsEmpty(ht));
	TH_ASSERT(0 == HashTableSize(ht));

	HTInsertKeyValEnts(ht, test_ents, 6);

	TH_ASSERT(0 == HashTableIsEmpty(ht));
	TH_ASSERT(6 == HashTableSize(ht));

	HashTableDestroy(ht);
}

static void TestHTRemove(void)
{
	h_table_t *ht = HashTableCreate(100, HashDjb2, IsMatch);

	int test_nums[6] = {1001, 1002, 1003, 1004, 1005, 9999};
	keyval_entity_t test_ents[6] = {
		{"HELLO_1", NULL},
		{"HELLO_2", NULL},
		{"HELLO_3", NULL},
		{"HELLO_4", NULL},
		{"HELLO_5", NULL},
		{"HELLO_6", NULL},
	};

	test_ents[0].value = &test_nums[0];
	test_ents[1].value = &test_nums[1];
	test_ents[2].value = &test_nums[2];
	test_ents[3].value = &test_nums[3];
	test_ents[4].value = &test_nums[4];
	test_ents[5].value = &test_nums[5];

	HTInsertKeyValEnts(ht, test_ents, 6);

	HashTableRemove(ht, &test_ents[4]);
	TH_ASSERT(5 == HashTableSize(ht));

	HashTableRemove(ht, &test_ents[5]);
	TH_ASSERT(4 == HashTableSize(ht));

	HashTableDestroy(ht);
}

static void TestHTFind(void)
{
	h_table_t *ht = HashTableCreate(100, HashDjb2, IsMatch);
	keyval_entity_t *founded = NULL;
	int test_nums[6] = {1001, 1002, 1003, 1004, 1005, 9999};
	keyval_entity_t test_ents[6] = {
		{"HELLO_1", NULL},
		{"HELLO_2", NULL},
		{"HELLO_3", NULL},
		{"HELLO_4", NULL},
		{"HELLO_5", NULL},
		{"HELLO_6", NULL},
	};

	test_ents[0].value = &test_nums[0];
	test_ents[1].value = &test_nums[1];
	test_ents[2].value = &test_nums[2];
	test_ents[3].value = &test_nums[3];
	test_ents[4].value = &test_nums[4];
	test_ents[5].value = &test_nums[5];

	TH_ASSERT(NULL != ht);
	TH_ASSERT(1 == HashTableIsEmpty(ht));
	TH_ASSERT(0 == HashTableSize(ht));

	HTInsertKeyValEnts(ht, test_ents, 6);

	TH_ASSERT(0 == HashTableIsEmpty(ht));
	TH_ASSERT(6 == HashTableSize(ht));

	founded = HashTableFind(ht, &test_ents[4]);
	TH_ASSERT(*(int *) test_ents[4].value == *(int *) founded->value);

	HashTableRemove(ht, &test_ents[4]);
	founded = HashTableFind(ht, &test_ents[4]);
	TH_ASSERT(NULL == founded);
	TH_ASSERT(5 == HashTableSize(ht));

	HashTableDestroy(ht);
}

static void TestHTForEach(void)
{
	h_table_t *ht = HashTableCreate(100, HashDjb2, IsMatch);
	keyval_entity_t *founded = NULL;
	int action_test_val = 5;

	int test_nums[6] = {1001, 1002, 1003, 1004, 1005, 9999};
	keyval_entity_t test_ents[6] = {
		{"HELLO_1", NULL},
		{"HELLO_2", NULL},
		{"HELLO_3", NULL},
		{"HELLO_4", NULL},
		{"HELLO_5", NULL},
		{"HELLO_6", NULL},
	};

	test_ents[0].value = &test_nums[0];
	test_ents[1].value = &test_nums[1];
	test_ents[2].value = &test_nums[2];
	test_ents[3].value = &test_nums[3];
	test_ents[4].value = &test_nums[4];
	test_ents[5].value = &test_nums[5];


	HTInsertKeyValEnts(ht, test_ents, 6);

	HashTableForEach(ht, AdditionAction, &action_test_val);
	founded = HashTableFind(ht, &test_ents[3]);
	TH_ASSERT(1009 == *(int *) founded->value);

	HashTableDestroy(ht);
}

static void TestHTCaching(void)
{
	clock_t start_t, end_t;
	double first_search_time = 0.0;
	double second_search_time = 0.0;

	keyval_entity_t *founded = NULL;

	keyval_entity_caching_t test_arr[7000] = {0};

	int i = 0;

	h_table_t *ht = HashTableCreate(100, HashDjb2Cashing, IsMatchCashing);

	for (; i < 7000; ++i)
	{
		test_arr[i].key = i+i;
		test_arr[i].value = 0;
	}

	for (i = 0; i < 7000; ++i)
	{
		HashTableInsert(ht, &test_arr[i]);
	}

	TH_ASSERT(0 == HashTableIsEmpty(ht));
	TH_ASSERT(7000 == HashTableSize(ht));

	start_t = clock();

	founded = HashTableFind(ht, &test_arr[6999]);

	end_t = clock();

	first_search_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("Benchmark first search:\n");
 	printf("Total time taken by CPU: %f\n", first_search_time);

 	start_t = clock();

	founded = HashTableFind(ht, &test_arr[6999]);

	end_t = clock();

	second_search_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("Benchmark second search:\n");
 	printf("Total time taken by CPU: %f\n", second_search_time);

 	TH_ASSERT(second_search_time < first_search_time);

	HashTableDestroy(ht);

	(void) founded;
}

static void TestHTSpellChecker(void)
{
	h_table_t *ht =
	HashTableCreate(105000, HashDjb2Dictionary, IsMatchDictionary);

	char *path = "/usr/share/dict/words";

	dictionary_entity_t *dictionary_buffer = NULL;

	char input_buffer[100] = {'\0'};

	int i = 0;
	int n = 0;

	dictionary_entity_t ent_to_find = {"", ""};
	dictionary_entity_t *founded = NULL;

	FILE *stream_from = fopen(path,"r");
	if (NULL == stream_from)
	{
		return;
	}

	dictionary_buffer =
	(dictionary_entity_t *) malloc(sizeof(struct dictionary_entity) * 105000);
	if (NULL == dictionary_buffer)
	{
		return;
		HashTableDestroy(ht);
	}

	for (; NULL != fgets(input_buffer, 100, stream_from); ++i)
	{
		input_buffer[strcspn(input_buffer, "\n")] = 0;
		strcpy(dictionary_buffer[i].key, input_buffer);
		strcpy(dictionary_buffer[i].value, input_buffer);
		HashTableInsert(ht, &dictionary_buffer[i]);
	}

	fclose(stream_from);

	printf("Enter a word to find: ");

	n = scanf("%s", ent_to_find.key);
    if (n == 1) {
        founded = HashTableFind(ht, &ent_to_find);

		if (NULL != founded)
		{
			TH_ASSERT(0 == strcmp(founded->value, ent_to_find.key));
			printf("found\n");
		}
		else
		{
			printf("not found\n");
		}
    }
    else
    {
    	printf("scan error\n");
    }


	

	free(dictionary_buffer);
	dictionary_buffer = NULL;

	HashTableDestroy(ht);
}

static int IsMatch(const void *data1, const void *data2)
{
	keyval_entity_t *ent1 = (void *) data1;
	keyval_entity_t *ent2 = (void *) data2;

	return (0 == strcmp(ent1->key, ent2->key));
}

static int IsMatchDictionary(const void *data1, const void *data2)
{
	dictionary_entity_t *ent1 = (void *) data1;
	dictionary_entity_t *ent2 = (void *) data2;

	return (0 == strcmp(ent1->key, ent2->key));
}

static int IsMatchCashing(const void *data1, const void *data2)
{
	keyval_entity_caching_t *ent1 = (void *) data1;
	keyval_entity_caching_t *ent2 = (void *) data2;

	return (ent1->key == ent2->key);
}

static size_t HashDjb2(const void *data)
{
	size_t hash = 5381;
	keyval_entity_t *ent = (void *) data;
	char *str = ent->key;
	int c;

	while ((c = *str++))
	{
		hash = ((hash << 5) + hash) + c;
	}

	return (hash);
}

static size_t HashDjb2Cashing(const void *data)
{
	return (0);
	(void) data;
}

static size_t HashDjb2Dictionary(const void *data)
{
	size_t hash = 5381;
	dictionary_entity_t *ent = (void *) data;
	char *str = ent->key;
	int c;

	while ((c = *str++))	
	{
		hash = ((hash << 5) + hash) + c;
	}

	return (hash);
}

static int AdditionAction(void *data, void *param)
{
	keyval_entity_t *ent = (void *) data;

	*(int *) ent->value += *(int *) param;
	return (0);
}

static void HTInsertKeyValEnts(h_table_t *ht, keyval_entity_t *arr, size_t size)
{
	size_t i = 0;

	for (; i < size; ++i)
	{
		HashTableInsert(ht, &arr[i]);
	}
}