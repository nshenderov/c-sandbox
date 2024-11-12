/*******************************************************************************
*
* FILENAME : dhcp_test.c
*
* DESCRIPTION : DHCP implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 17.06.2023
* 
*******************************************************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* atol */
#include <stdio.h> /* sprintf */

#include "dhcp.h"

#define IP_ADDR_SIZE (32)

#define NETWORK_ADDR ((ip_t) 0x0)
#define SERVER_ADDR ((ip_t) 0xFFFFFFFE)
#define BROADCAST_ADDR ((ip_t) 0xFFFFFFFF)

typedef enum default_netids
{
    NETWORK,
    SERVER,
    BROADCAST,
    AMOUNT_OF_DEF_NETIDS
} default_netids_t;

typedef enum children
{
    LEAF = -1,
    LEFT,
    RIGHT,
    AMOUNT_OF_CHILDREN
} children_t;

enum {SUCCESS, FAILURE};
enum {NOT_FULL, FULL};

typedef struct trie trie_t;
typedef struct trie_node trie_node_t;

struct dhcp
{
    trie_t *trie;
    ip_t network_id;
    size_t network_num_bits;
};

struct trie
{
    trie_node_t *root;
    size_t max_depth;
};

struct trie_node
{
    trie_node_t *children[AMOUNT_OF_CHILDREN];
    int is_full;
};

/* trie */
/* -------------------------------------------------------------------------- */
static trie_node_t *TrieCreateNode(void);
static trie_t *TrieCreate(size_t max_depth);
static void TrieDestroyNodes(trie_node_t *node);
static void TrieDestroy(trie_t *trie);
static void TrieCountTraversal(trie_node_t *node, size_t depth, size_t *counter);
static size_t TrieCount(trie_t *trie);
static int TrieInsert(trie_t *trie, trie_node_t *node, ip_t *host, size_t depth);
static int TrieFree(trie_t *trie, trie_node_t *node, ip_t host, size_t depth);
static void TrieTryToSetFull(trie_node_t *node);
static int TrieIsHostReachedMax(trie_t *trie,ip_t host);
/* -------------------------------------------------------------------------- */

/* helpers */
/* -------------------------------------------------------------------------- */
static int FillTrieDefault(trie_t *trie);
static unsigned int CheckBit(ip_t number, size_t position);
static unsigned int GetMask(size_t bits_shift_right);
static size_t PowOfTwo(size_t power);
/* -------------------------------------------------------------------------- */


/* main api */
/* -------------------------------------------------------------------------- */
dhcp_t *DHCPCreate(ip_t network_id, size_t network_num_bits)
{
    dhcp_t *new_dhcp = NULL;
    trie_t *new_trie = NULL;

    assert(0 != network_num_bits);
    assert(0 != network_id);

    new_dhcp = (dhcp_t *) malloc(sizeof(dhcp_t));
    if (NULL == new_dhcp)
    {
        return (NULL);
    }

    new_trie = TrieCreate(IP_ADDR_SIZE - network_num_bits);
    if (NULL == new_trie)
    {
        free(new_dhcp);
        new_dhcp = NULL;

        return (NULL);
    }

    if (FillTrieDefault(new_trie))
    {
        free(new_trie);
        new_trie = NULL;

        free(new_dhcp);
        new_dhcp = NULL;

        return (NULL);
    }
    
    new_dhcp->trie = new_trie;
    new_dhcp->network_id = network_id & (~GetMask(network_num_bits));
    new_dhcp->network_num_bits = network_num_bits;

    return (new_dhcp);
}

void DHCPDestroy(dhcp_t *dhcp)
{
    assert(NULL != dhcp);

    TrieDestroy(dhcp->trie);
    dhcp->trie = NULL;

    free(dhcp);
    dhcp = NULL;
}

int DHCPAllocIP(dhcp_t *dhcp, ip_t requested_ip, ip_t *result_ip)
{
    int status = 0;
    ip_t allocated_ip = 0;
    trie_t *trie = dhcp->trie;
    trie_node_t *root = trie->root;

    assert(NULL != dhcp);
    assert(NULL != result_ip);

    allocated_ip = requested_ip & GetMask(dhcp->network_num_bits);

    status = TrieInsert(trie, root, &allocated_ip, trie->max_depth);

    *result_ip = dhcp->network_id + allocated_ip;

    return (status);  
}

void DHCPFreeIP(dhcp_t *dhcp, ip_t ip)
{
    trie_t *trie = dhcp->trie;
    trie_node_t *root = trie->root;
    assert(NULL != dhcp);

    ip &= GetMask(dhcp->network_num_bits);

    TrieFree(trie, root, ip, trie->max_depth);
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
    assert(NULL != dhcp);

    return (GetMask(dhcp->network_num_bits) - TrieCount(dhcp->trie));
}


char *DHCPIPToStr(ip_t ip, char *buffer)
{
    unsigned char *bytes = (unsigned char *) &ip;

    assert(NULL != buffer);

    sprintf(buffer, "%d.%d.%d.%d", bytes[3], bytes[2], bytes[1], bytes[0]);

    return (buffer);
}

ip_t DHCPStrToIP(const char *string)
{
    ip_t ip = 0;

    char *runner = NULL;

    assert(NULL != string);

    runner = (char *) string;

    do
    {
        ip <<= 8;
        ip |= atol(runner);
        runner = strchr(runner, '.');
    } while (NULL != runner++);
    
    return (ip);
}


/* trie */
/* -------------------------------------------------------------------------- */
static trie_node_t *TrieCreateNode(void)
{
    trie_node_t *new_node = (trie_node_t *) malloc(sizeof(trie_node_t));
    if (NULL == new_node)
    {
        return (NULL);
    }

    new_node->is_full = NOT_FULL;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;

    return (new_node);
}

static trie_t *TrieCreate(size_t max_depth)
{
    trie_t *new_trie = NULL;
    trie_node_t *root_node = NULL;

    assert(0 != max_depth);

    new_trie = (trie_t *) malloc(sizeof(trie_t));
    if (NULL == new_trie)
    {
        return (NULL);
    }

    root_node = TrieCreateNode();
    if (NULL == root_node)
    {
        free(new_trie);
        new_trie = NULL;

        return (NULL);
    }

    new_trie->root = root_node;
    new_trie->max_depth = max_depth;

    return (new_trie);
}

static int TrieInsert(trie_t *trie, trie_node_t *node, ip_t *host, size_t depth)
{
    int direction = CheckBit(*host, depth - 1);
    trie_node_t *child = NULL;

    assert(NULL != trie);
    assert(NULL != node);

    if (TrieIsHostReachedMax(trie, *host))
    {
        return (IP_SUBSEQUENT_IS_OCCUPIED);
    }

    if (depth == 0)
    {
        node->is_full = FULL;

        return (IP_SUCCESS);
    }

    child = node->children[direction];

    if (NULL == child)
    {
        child = TrieCreateNode();
        if (NULL == child)
        {
            return (IP_ALLOC_FAIL);
        }

        node->children[direction] = child;
    }    

    TrieTryToSetFull(node);

    if (child->is_full)
    {
        *host += 1;
        return (TrieInsert(trie, trie->root, host, trie->max_depth));
    }

    return (TrieInsert(trie, child, host, depth - 1));
}

static void TrieDestroyNodes(trie_node_t *node)
{
    if (NULL == node)
    {
        return;
    }

    TrieDestroyNodes(node->children[LEFT]);
    TrieDestroyNodes(node->children[RIGHT]);

    free(node);
    node = NULL;
}

static void TrieDestroy(trie_t *trie)
{
    assert(NULL != trie);

    TrieDestroyNodes(trie->root);

    free(trie);
    trie = NULL;
}

static int TrieFree(trie_t *trie, trie_node_t *node, ip_t host, size_t depth)
{
    int direction = CheckBit(host, depth - 1);
    int res = 0;
    trie_node_t *child = NULL;

    assert(NULL != trie);
    assert(NULL != node);

    if (depth == 0)
    {
        node->is_full = NOT_FULL;

        return (SUCCESS);
    }

    child = node->children[direction];

    if (NULL == child)
    {
        return (FAILURE);
    }

    res = TrieFree(trie, child, host, depth - 1);

    if (FAILURE == res)
    {
        return (FAILURE);
    }

    node->is_full = NOT_FULL;


    return (SUCCESS);
}

static void TrieCountTraversal(trie_node_t *node, size_t depth, size_t *counter)
{
	if (NULL == node)
	{
		return;
	}

	if (node->is_full)
	{
		*counter += PowOfTwo(depth);
		return;
	}

	TrieCountTraversal(node->children[LEFT], depth - 1, counter);
	TrieCountTraversal(node->children[RIGHT], depth - 1, counter);
}

static size_t TrieCount(trie_t *trie)
{
    size_t counter = 0;

    assert(NULL != trie);

    TrieCountTraversal(trie->root, trie->max_depth, &counter);

    return (counter);
}

static int TrieIsHostReachedMax(trie_t *trie,ip_t host)
{
    return (0 == (GetMask(IP_ADDR_SIZE - trie->max_depth) - 1) - host);
}

static void TrieTryToSetFull(trie_node_t *node)
{
    assert(NULL != node);

    if (NULL != node->children[LEFT] && NULL != node->children[RIGHT]
        && node->children[LEFT]->is_full && node->children[RIGHT]->is_full)
    {
        node->is_full = FULL;
    }
}


/* helpers */
/* -------------------------------------------------------------------------- */

static int FillTrieDefault(trie_t *trie)
{

    size_t i = 0;
    ip_t ips[AMOUNT_OF_DEF_NETIDS] = {0};
    int status = 0;

    assert(NULL != trie);

    ips[NETWORK] = NETWORK_ADDR;
    ips[SERVER] = SERVER_ADDR;
    ips[BROADCAST] = BROADCAST_ADDR;

    for (; 3 > i && 1 != status; ++i)
    {
        ips[i] &= GetMask(IP_ADDR_SIZE - trie->max_depth);

        status = TrieInsert(trie, trie->root, &ips[i], trie->max_depth);
    }

    return (status);
}

static unsigned int CheckBit(ip_t number, size_t position)
{
    return (0 != (number & (0x1 << (position))));
}

static unsigned int GetMask(size_t bits_shift_right)
{
    return (0xFFFFFFFF >> bits_shift_right);
}

static size_t PowOfTwo(size_t power)
{
	return (0x1 << power);
}
