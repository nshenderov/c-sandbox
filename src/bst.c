/*******************************************************************************
*
* FILENAME : bst.c
*
* DESCRIPTION : Binary search tree implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 28.05.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "bst.h"

#define SIZE_OF_BST_STRUCT (sizeof(struct bst))
#define SIZE_OF_BST_NODE_STRUCT (sizeof(struct bst_node))

#define HAS_PARENT(NODE) (NULL != NODE->parent)
#define HAS_CHILD(NODE, DIRECTION) (NULL != NODE->children[DIRECTION])

#define GET_CHILD(NODE, DIRECTION) (NODE->children[DIRECTION])
#define GET_PARENT(NODE) (NODE->parent)
#define GET_DATA(NODE) (NODE->data)
#define GET_ROOT_NODE(BST) (BST->dummy.children[LEFT])
#define GET_DUMMY_NODE(BST) ((bst_node_t *) &(BST->dummy))

#define UPDATE_PARENT(NODE, NEW_VALUE) (NODE->parent = NEW_VALUE)
#define UPDATE_DATA(NODE, NEW_VALUE) (NODE->data = NEW_VALUE)
#define UPDATE_CHILD(NODE, DIRECTION, NEW_VALUE) \
(NODE->children[DIRECTION] = NEW_VALUE)

#define IS_CHILDS_DIRECTION(NODE, DIRECTION) \
(GET_CHILD(NODE->parent, DIRECTION) \
	&& TRUE == BSTIsSameIterator(NODE, GET_CHILD(NODE->parent, DIRECTION)))


enum {SUCCESS, FAILURE};

typedef enum {
	SECOND_IS_BIGGER = -1,
	EQUALS,
	FIRST_IS_BIGGER
} comparison_res_t;

typedef enum {FALSE, TRUE} boolean_t;

typedef enum direction
{
	LEAF = -1,
	LEFT,
	RIGHT,
	AMOUNT_OF_DIRECTIONS
} direction_t;

typedef struct bst_node bst_node_t;
struct bst_node
{
	void *data; 
    bst_node_t *children[AMOUNT_OF_DIRECTIONS];
	bst_node_t *parent;
};
struct bst
{
    bst_node_t dummy;
    bst_compare_t compare;
    void *params;
};

static bst_iter_t TranslateNodeToIter(bst_node_t *node);

static bst_node_t *TranslateIterToNode(bst_iter_t iter);

static direction_t CheckForChildren(bst_node_t *node);

static void CopyNode(bst_node_t *from, bst_node_t *to);

static bst_node_t *GetToTheMostNode(bst_node_t *node, direction_t direction);

static bst_node_t *BinarySearch(bst_t *bst, void *data,
								comparison_res_t *res_out);

static bst_node_t *InitializeNode(bst_node_t *parent, bst_node_t *left,
										bst_node_t *right, void *data);

static void RemoveNode(bst_node_t *node_to_remove);

static bst_node_t *MoveInOrder(bst_node_t *runner, direction_t direction);


#ifndef NDEBUG

static bst_node_t *GetToTheDummy(bst_node_t *node);

static int IsNodeIsTheBeginning(bst_node_t *node);

static int IsNodeIsTheDummy(bst_node_t *node);

#endif


bst_t *BSTCreate(bst_compare_t compare, void *params)
{
	bst_t *new_bst = NULL;
	bst_node_t dummy = {0};

	assert(NULL != compare);

	new_bst = (bst_t *) malloc(SIZE_OF_BST_STRUCT);

	new_bst->dummy = dummy;
	new_bst->compare = compare;
	new_bst->params = params;

	return (new_bst);
}

void BSTDestroy(bst_t *bst)
{
	bst_node_t *runner = NULL;
	bst_node_t *temp = NULL;
	bst_node_t *end = NULL;

	assert(NULL != bst);

	if (NULL == GET_ROOT_NODE(bst))
	{
		free(bst);
		bst = NULL;
		return;
	}

	runner = BSTBegin(bst);
	end = BSTEnd(bst);

	while(!BSTIsSameIterator(runner, end))
    {	
    	switch(CheckForChildren(runner))
    	{
    		case(LEFT):
			{	
				runner = GetToTheMostNode(runner, LEFT);
				break;
			}
			case(RIGHT):
			{
				runner = GetToTheMostNode(runner, RIGHT);
				break;
			}
			default:
			{
				temp = runner->parent;
				RemoveNode(runner);
				runner = temp;
				break;
			}
    	}
    }

	free(bst);
	bst = NULL;
}

size_t BSTSize(const bst_t *bst)
{
	bst_node_t *runner = NULL;
	bst_node_t *end = NULL;
	size_t counter = 0;

	assert(NULL != bst);

	runner = BSTBegin(bst);
	end = BSTEnd(bst);

	while(!BSTIsSameIterator(runner, end))
	{
		runner = BSTNext(runner);
		++counter;
	}

	return (counter);
}

int BSTIsEmpty(const bst_t *bst)
{
	assert(NULL != bst);

	return (NULL == GET_ROOT_NODE(bst));
}

bst_iter_t BSTInsert(bst_t *bst, void *data)
{
	bst_node_t *founded_place = NULL;
	bst_node_t *new_node = NULL;
	comparison_res_t compare_result = EQUALS;

	assert(NULL != bst);

	founded_place = BinarySearch(bst, data, &compare_result);

	assert(EQUALS != compare_result);

	new_node = InitializeNode(founded_place, NULL, NULL, data);
	if (NULL == new_node)
	{
		return (BSTEnd(bst));
	}
	
	switch(compare_result)
	{
		case(FIRST_IS_BIGGER):
		{
			UPDATE_CHILD(founded_place, LEFT, new_node);
			break;
		}
		case(SECOND_IS_BIGGER):
		{
			UPDATE_CHILD(founded_place, RIGHT, new_node);
			break;
		}
		default:
		{
			break;
		}
	}

	return (TranslateNodeToIter(new_node));
}

bst_iter_t BSTRemove(bst_iter_t iter)
{
	bst_node_t *node_to_remove = NULL;
	bst_node_t *left_child = NULL;
	bst_node_t *right_child = NULL;
	bst_node_t *next_node = NULL;
	void *data = NULL;

	assert(NULL != TranslateIterToNode(iter));

	node_to_remove = TranslateIterToNode(iter);

	switch(CheckForChildren(node_to_remove))
	{
		case(LEAF):
		{
			next_node = BSTNext(node_to_remove);

			RemoveNode(node_to_remove);

			return (next_node);
		}

		case(AMOUNT_OF_DIRECTIONS):
		{
			next_node = BSTNext(node_to_remove);

			data = BSTGetData(next_node);

			UPDATE_DATA(node_to_remove, data);

			BSTRemove(next_node);

			break;
		}

		case(LEFT):
		{
			left_child = GET_CHILD(node_to_remove, LEFT);

			CopyNode(left_child, node_to_remove);

			free(left_child);
			left_child = NULL;

			break;
		}
		case(RIGHT):
		{
			right_child = GET_CHILD(node_to_remove, RIGHT);

			CopyNode(right_child, node_to_remove);

			free(right_child);
			right_child = NULL;

			break;
		}

		return (TranslateNodeToIter(node_to_remove));
	}

	return (node_to_remove);
}

bst_iter_t BSTBegin(const bst_t *bst)
{
	assert(NULL != bst);

	return (TranslateNodeToIter(GetToTheMostNode(GET_DUMMY_NODE(bst), LEFT)));
}

bst_iter_t BSTEnd(const bst_t *bst)
{
	assert(NULL != bst);

	return (TranslateNodeToIter(GET_DUMMY_NODE(bst)));
}

bst_iter_t BSTPrev(const bst_iter_t iter)
{
	bst_node_t *runner = NULL;

	assert(NULL != TranslateIterToNode(iter));

	assert(TRUE != IsNodeIsTheBeginning(TranslateIterToNode(iter)));

	runner = TranslateIterToNode(iter);

	runner = MoveInOrder(runner, LEFT);

	return (TranslateNodeToIter(runner));

}
bst_iter_t BSTNext(const bst_iter_t iter)
{
	bst_node_t *runner = NULL;

	assert(NULL != TranslateIterToNode(iter));

	assert(TRUE != IsNodeIsTheDummy(TranslateIterToNode(iter)));

	runner = TranslateIterToNode(iter);

	runner = MoveInOrder(runner, RIGHT);

	return (TranslateNodeToIter(runner));
}

int BSTIsSameIterator(const bst_iter_t iterator1, const bst_iter_t iterator2)
{
	assert(NULL != TranslateIterToNode(iterator1));
	assert(NULL != TranslateIterToNode(iterator2));

	if (TranslateIterToNode(iterator1) == TranslateIterToNode(iterator2))
	{
		return (TRUE);
	}
	return (FALSE);
}

void *BSTGetData(const bst_iter_t iter)
{
	assert(NULL != TranslateIterToNode(iter));
	assert(TRUE != IsNodeIsTheDummy(TranslateIterToNode(iter)));

	return (GET_DATA(TranslateIterToNode(iter)));
}

bst_iter_t BSTFind(bst_t *bst, void *key)
{
	bst_node_t *founded_node = NULL;
	comparison_res_t compare_result = EQUALS;

	assert(NULL != bst);
	assert(NULL != key);

	founded_node = BinarySearch(bst, key, &compare_result);

	return (EQUALS == compare_result ?
							TranslateNodeToIter(founded_node) : BSTEnd(bst));
}

int BSTForeach(bst_iter_t from, bst_iter_t to, bst_action_t action,
														void *params)
{
	bst_iter_t runner = NULL;

	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action);

	runner = from;

	while(!BSTIsSameIterator(runner, to))
	{
		if (SUCCESS != action(BSTGetData(runner), params))
        {
            return (FAILURE);
        }

		runner = BSTNext(runner);
	}

	return (SUCCESS);
}

static bst_node_t *BinarySearch(bst_t *bst, void *data,
								comparison_res_t *res_out)
{
	bst_node_t *runner = NULL;
	bst_node_t *founded = NULL;
	void *params = NULL;
	bst_compare_t compare = NULL;
	comparison_res_t compare_result = FIRST_IS_BIGGER;

	assert(NULL != bst);

	compare = bst->compare;
	params = bst->params;

	founded = GET_DUMMY_NODE(bst);

	runner = GET_ROOT_NODE(bst);

	while(NULL != runner)
    {
        founded = runner;

        compare_result = compare(BSTGetData(founded), data, params);

        switch(compare_result)
		{
			case(EQUALS):
			{	
				return (runner);
			}

			case(FIRST_IS_BIGGER):
			{
				runner = GET_CHILD(runner, LEFT);
				break;
			}

			case(SECOND_IS_BIGGER):
			{
				runner = GET_CHILD(runner, RIGHT);
				break;
			}
		}

    }

    *res_out = compare_result;

    return (founded);
}

static direction_t CheckForChildren(bst_node_t *node)
{
	boolean_t result_left = FALSE;
	boolean_t result_right = FALSE;

	assert(NULL != node);

	result_left = HAS_CHILD(node, LEFT);
	result_right = HAS_CHILD(node, RIGHT);

	if (result_left && result_right)
	{
		return (AMOUNT_OF_DIRECTIONS);
	}
	else if (result_left)
	{
		return (LEFT);
	}
	else if (result_right)
	{
		return (RIGHT);
	}
	else
	{
		return (LEAF);
	}
}

static bst_node_t *GetToTheMostNode(bst_node_t *node, direction_t direction)
{
	bst_node_t *runner = node;

	assert(NULL != node);

	while(HAS_CHILD(runner, direction))
	{
		runner = GET_CHILD(runner, direction);
	}

	return (runner);
}

static void RemoveNode(bst_node_t *node_to_remove)
{
	bst_node_t *parent = NULL;

	assert(NULL != node_to_remove);

	parent = GET_PARENT(node_to_remove);

	if (IS_CHILDS_DIRECTION(node_to_remove, LEFT))
	{
		UPDATE_CHILD(parent, LEFT, NULL);
	}
	else
	{
		UPDATE_CHILD(parent, RIGHT, NULL);
	}

	free(node_to_remove);
	node_to_remove = NULL;
}

static bst_node_t *MoveInOrder(bst_node_t *runner, direction_t direction)
{
	boolean_t res = FALSE;

	assert(NULL != runner);

	if (HAS_CHILD(runner, direction))
	{
		runner = GetToTheMostNode(GET_CHILD(runner, direction),
									LEFT == direction ? RIGHT : LEFT);
	}
	else
	{
		do
		{
			res = IS_CHILDS_DIRECTION(runner, direction);
			runner = GET_PARENT(runner);

		} while(FALSE != res);
	}

	return (runner);

}

static void CopyNode(bst_node_t *from, bst_node_t *to)
{
	bst_node_t *right_child = GET_CHILD(from, RIGHT);
	bst_node_t *left_child = GET_CHILD(from, LEFT);

	assert(NULL != from);
	assert(NULL != to);

	UPDATE_CHILD(to, LEFT, left_child);
	UPDATE_CHILD(to, RIGHT, right_child);
	UPDATE_DATA(to, GET_DATA(from));

	if (HAS_CHILD(from, LEFT))
	{
		UPDATE_PARENT(left_child, to);
	}
	if (HAS_CHILD(from, RIGHT))
	{
		UPDATE_PARENT(right_child, to);
	}
}

static bst_node_t *InitializeNode(bst_node_t *parent, bst_node_t *left,
										bst_node_t *right, void *data)
{
	bst_node_t *new_node = NULL;

	new_node = (bst_node_t *) malloc(SIZE_OF_BST_NODE_STRUCT);
	if (NULL == new_node)
	{
		return (NULL);
	}

	UPDATE_CHILD(new_node, LEFT, left);
	UPDATE_CHILD(new_node, RIGHT, right);
	UPDATE_PARENT(new_node, parent);
	UPDATE_DATA(new_node, data);

	return (new_node);
}

static bst_iter_t TranslateNodeToIter(bst_node_t *node)
{
	assert(NULL != node);

	return ((bst_iter_t) node);
}

static bst_node_t *TranslateIterToNode(bst_iter_t iter)
{
	assert(NULL != iter);

	return ((bst_node_t *) iter);
}

#ifndef NDEBUG

static bst_node_t *GetToTheDummy(bst_node_t *node)
{
	bst_node_t *runner = node;

	assert(NULL != node);

	while(HAS_PARENT(runner))
    {
    	runner = GET_PARENT(runner);
    }

    return (runner);
}

static int IsNodeIsTheBeginning(bst_node_t *node)
{
	bst_node_t *dummy = NULL;
	bst_node_t *begin = NULL;

	assert(NULL != node);

	dummy = GetToTheDummy(node);

	begin = GetToTheMostNode(dummy, LEFT);

	return (BSTIsSameIterator(TranslateNodeToIter(begin),
								TranslateNodeToIter(node)));
}


static int IsNodeIsTheDummy(bst_node_t *node)
{
	bst_node_t *dummy = NULL;

	assert(NULL != node);

	dummy = GetToTheDummy(node);

	return (BSTIsSameIterator(TranslateNodeToIter(dummy),
								TranslateNodeToIter(node)));
}

#endif