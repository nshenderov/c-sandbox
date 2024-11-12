/*******************************************************************************
*
* FILENAME : avl.c
*
* DESCRIPTION : AVL tree implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 05.06.2023
* 
*******************************************************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "avl.h"

#define SIZE_OF_AVL_STRUCT (sizeof(struct avl))
#define SIZE_OF_AVL_NODE_STRUCT (sizeof(struct avl_node))

#define BASE_HEIGHT (1)
#define BALANCE_THRESHOLD (1)

#define MAX(A,B) (((A)>(B))?(A):(B))
#define HAS_CHILD(NODE, DIRECTION) (NULL != NODE->children[DIRECTION])
#define GET_HEIGHT(NODE) (NULL == NODE ? 0 : NODE->height)

#define COMPUTE_HEIGHT(NODE) \
(BASE_HEIGHT + MAX(GET_HEIGHT(NODE->children[LEFT]), \
GET_HEIGHT(NODE->children[RIGHT])))

#define COMPUTE_BALANCE(NODE) \
((int) GET_HEIGHT(NODE->children[LEFT]) - \
(int) GET_HEIGHT(NODE->children[RIGHT]))

typedef enum {SUCCESS, FAILURE} status_t;

typedef enum {
	SECOND_IS_BIGGER = -1,
	EQUALS,
	FIRST_IS_BIGGER
} comparison_res_t;

typedef enum direction
{
	LEAF = -1,
	LEFT,
	RIGHT,
	AMOUNT_OF_DIRECTIONS
} direction_t;

typedef struct avl_node
{
    struct avl_node *children[AMOUNT_OF_DIRECTIONS];
    void *data;
    size_t height;
    
} avl_node_t;

struct avl
{
    avl_node_t *root;
    avl_compare_t cmp;
    void *params;
};

static avl_node_t *InitializeNewNode(void *data);
static avl_node_t *RebalanceNodes(avl_node_t *node);
static avl_node_t *Rotate(avl_node_t *node, direction_t direction);
static avl_node_t *GetSuccessor(avl_node_t *node);
static int CountNodes(void *data, void *param);
static direction_t CheckForChildren(avl_node_t *node);
static avl_node_t *BinaryInsert(avl_compare_t compare, avl_node_t *node,
												void *data, void *params);
static avl_node_t *BinaryRemove(avl_node_t *node, avl_compare_t Compare,
												void *data, void *params);
static void *BinaryFind(avl_node_t *node, avl_compare_t Compare, void *data,
																void *params);
static void AVLDestroyRecursion(avl_node_t *node);
static void AVLForEachRecursion(avl_node_t *node, avl_action_t action,
										void *param, status_t *status);

avl_t *AVLCreate(avl_compare_t compare, void *params)
{
	avl_t *new_avl = NULL;

	assert(NULL != compare);

	new_avl = (avl_t *) malloc(SIZE_OF_AVL_STRUCT);

	new_avl->root = NULL;
	new_avl->cmp = compare;
	new_avl->params = params;

	return (new_avl);
}

void AVLDestroy(avl_t *avl)
{
	assert(NULL != avl);

	AVLDestroyRecursion(avl->root);

	free(avl);
	avl = NULL;
}

static avl_node_t *BinaryInsert(avl_compare_t compare, avl_node_t *node,
													void *data, void *params)
{
    direction_t direction = 0;

    if(NULL == node)
    {
        return (InitializeNewNode(data));
    }

    direction = (0 < compare(node->data, data, params)) ? RIGHT : LEFT;

    node->children[direction] = BinaryInsert(compare, node->children[direction],
    															data, params);

    node->height = COMPUTE_HEIGHT(node);

    return (RebalanceNodes(node));
}

int AVLInsert(avl_t *avl, void *data)
{
	avl_node_t *root = NULL;
	avl_node_t *new_node = NULL;

	assert(NULL != avl);

	root = avl->root;

	if (NULL == root)
	{
		new_node = InitializeNewNode(data);
		avl->root = new_node;
		return (SUCCESS);
	}

	avl->root = BinaryInsert(avl->cmp, root, data, avl->params);

	return (NULL == avl->root ? FAILURE : SUCCESS);
}

static avl_node_t *BinaryRemove(avl_node_t *node, avl_compare_t Compare,
												void *data, void *params)
{
	avl_node_t *temp = NULL;
	direction_t direction = LEFT;

	assert(NULL != Compare);

	if(NULL == node)
    {
        return (node);
    }

	if (EQUALS == Compare(node->data, data, params))
	{
		switch(CheckForChildren(node))
			{
				case(LEAF):
				{
	                free(node);
	                node = NULL;
					return (NULL);
				}
				case(LEFT):
				{
					temp = node->children[LEFT];
					free(node);
					return (temp);
				}
				case(RIGHT):
				{
					temp = node->children[RIGHT];
					free(node);
					return (temp);
				}
				case (AMOUNT_OF_DIRECTIONS):
				{
					temp = GetSuccessor(node->children[RIGHT]);
					node->data = temp->data;
					node->children[RIGHT] =BinaryRemove(node->children[RIGHT],
												Compare, temp->data, params);
					break;
				}	
			}
	}
	else
	{
		direction = 0 < Compare(node->data, data, params) ? RIGHT : LEFT;
		node->children[direction] = BinaryRemove(node->children[direction],
													Compare, data, params);
	}

	node->height = COMPUTE_HEIGHT(node);

	return (RebalanceNodes(node));

}

void AVLRemove(avl_t *avl, void *data)
{
	assert(NULL != avl);

	if(AVLIsEmpty(avl))
    {
        return;
    }

	avl->root = BinaryRemove(avl->root, avl->cmp, data, avl->params);
}

static void *BinaryFind(avl_node_t *node, avl_compare_t Compare, void *data,
																void *params)
{
    direction_t direction = LEFT;

    if(NULL == node || EQUALS == Compare(node->data, data, params))
    {
        return (node);
    }

    direction = 0 < Compare(node->data, data, params) ? RIGHT : LEFT;

    return(BinaryFind(node->children[direction], Compare, data, params));

}
void *AVLFind(const avl_t *avl, void *data)
{
	avl_node_t *node = NULL;

	assert(NULL != avl);
	assert(NULL != data);

    node = BinaryFind(avl->root, avl->cmp, data, avl->params);
    if(NULL == node)
    {
        return (NULL);
    }

    return(node->data);
}

int AVLForEach(avl_t *avl, avl_action_t action, void *param)
{
	avl_node_t *root = NULL;

	status_t status = 0;

	assert(NULL != avl);
	assert(NULL != action);

	root = avl->root;

	AVLForEachRecursion(root, action, param, &status);

	return (status);
}

int AVLIsEmpty(const avl_t *avl)
{
	assert(NULL != avl);

	return (NULL == avl->root);
}

size_t AVLSize(const avl_t *avl)
{
	size_t counter = 0;

	assert(NULL != avl);

	AVLForEach((avl_t *) avl, CountNodes, &counter);

	return(counter);
}

size_t AVLHeight(const avl_t *avl)
{
	assert(NULL != avl);

	return (GET_HEIGHT(avl->root));
}

static avl_node_t *InitializeNewNode(void *data)
{
    avl_node_t *new_node = (avl_node_t *) malloc(SIZE_OF_AVL_NODE_STRUCT);
    if (NULL == new_node)
    {
    	return (NULL);
    }

    new_node->height = BASE_HEIGHT;
    new_node->data = data;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;

    return(new_node);
}

static avl_node_t *RebalanceNodes(avl_node_t *node)
{
	int balance = 0;

	assert(NULL != node);

	balance = COMPUTE_BALANCE(node);

	if (balance < -BALANCE_THRESHOLD)
	{
		if (0 < COMPUTE_BALANCE(node->children[RIGHT]) )
		{
			node->children[RIGHT] = Rotate(node->children[RIGHT], RIGHT);
		}
        	
        return Rotate(node, LEFT);
	}
	else if (balance > BALANCE_THRESHOLD)
	{
		if (0 > COMPUTE_BALANCE(node->children[LEFT]))
		{
			node->children[LEFT] = Rotate(node->children[LEFT], LEFT);
		}

		return Rotate(node, RIGHT);
	}

	return (node);
}

static avl_node_t *Rotate(avl_node_t *node, direction_t direction)
{
	avl_node_t *node_to_rotate_child = NULL;
	avl_node_t *child_of_the_child = NULL;

	assert(NULL != node);

	node_to_rotate_child = node->children[!direction];
	child_of_the_child = node_to_rotate_child->children[direction];

	node_to_rotate_child->children[direction] = node;
	node->children[!direction] = child_of_the_child;

	node->height =  COMPUTE_HEIGHT(node);
	node_to_rotate_child->height =  COMPUTE_HEIGHT(node_to_rotate_child);

	return (node_to_rotate_child);
}

static avl_node_t *GetSuccessor(avl_node_t *node)
{
	assert(NULL != node);

	if (NULL == node->children[LEFT])
	{
		return (node);
	}

	return (GetSuccessor(node->children[LEFT]));
}


static void AVLDestroyRecursion(avl_node_t *node)
{
	if (node == NULL)
	{
       	return;
	}

	AVLDestroyRecursion(node->children[LEFT]);
	AVLDestroyRecursion(node->children[RIGHT]);

	free(node);
	node = NULL;
}

static void AVLForEachRecursion(avl_node_t *node, avl_action_t action,
												void *param, status_t *status)
{
	if (node == NULL)
	{
       	return;
	}

	AVLForEachRecursion(node->children[LEFT], action, param, status);
	AVLForEachRecursion(node->children[RIGHT], action, param, status);

	if (0 == *status)
	{
		*status = action(node->data, param);
	}
}

static int CountNodes(void *data, void *param)
{
	*(size_t *) param += 1;

	return (0);
	(void) data;
}

static direction_t CheckForChildren(avl_node_t *node)
{
	int result_left = 0;
	int result_right = 0;

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