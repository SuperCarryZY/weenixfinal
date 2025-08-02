#pragma once

#include "kernel.h"

/*
 * Standard btree implementation based on the following and Python implementation:
 * Introduction to Algorithms (Cormen, Leiserson, Rivest, Stein)
 * https://github.com/msambol/dsa/blob/master/trees/b_tree.py
 */

/* 
 * Branching factor determines certain bounds on the size of our tree as follows:
 * Let t be our branching factor.
 * Lower bound: each node has at least t-1 keys, at least t children
 * Upper bound: each node has at most 2t-1 keys, at most 2t children
 * Height of the tree is bound by log_t(n + 1) / 2 where n is the number of nodes
 * 
 * Change it as needed for performance
 */

#define BRANCHING_FACTOR 2
#define MAX_KEYS (2*BRANCHING_FACTOR) - 1
#define MAX_CHILDREN 2*BRANCHING_FACTOR

/*
 * btree_node is the core of our rough btree implementation here
 * - n_keys keeps track of the number of keys (out of the max 2t-1) we have on this node
 * - keys is the array of keys
 * - data is the array of addresses corresdoning to keys
 * - children is the array of children for this node
 * 
 * Note that keys and data are indexed the same. For the pageframe example, keys[i] is 
 * the pagenum of the i'th pageframe, and data[i] is the address of the i'th pageframe
 * where i is the index of the pageframe stored in this node. In this example, i has 
 * absoutely nothing to do with the pageframe's index in the mo_pframes linkedlist
*/

typedef struct btree_node
{
    unsigned int n_keys;
    unsigned int n_children;
    int is_leaf;
    uint64_t keys[MAX_KEYS];
    void *data[MAX_KEYS];
    struct btree_node *children[MAX_CHILDREN];
} btree_node_t;


void btree_init();

btree_node_t *btree_node_create();

void btree_insert(btree_node_t **root, uint64_t key, void *data);

void *btree_search(btree_node_t *root, uint64_t key);

void btree_delete(btree_node_t **root, uint64_t key);

void btree_node_free(btree_node_t **node);

void btree_destroy(btree_node_t *root);

void print_btree(btree_node_t *x);