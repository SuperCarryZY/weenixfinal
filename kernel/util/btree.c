#include "globals.h"

#include "util/btree.h"

#include "mm/slab.h"
#include "mm/pframe.h"

#include "util/debug.h"
#include "util/string.h"

static slab_allocator_t *btree_node_allocator;

// Array helpers
static int btree_pop_key(btree_node_t *x, unsigned int i, void **d);
static void btree_append_key(btree_node_t *x, int k, void *d);
static void btree_insert_key(btree_node_t *x, int k, int i, void *d);
static btree_node_t *btree_pop_child(btree_node_t *x, unsigned int i);
static void btree_append_child(btree_node_t *x, btree_node_t *addr);
static void btree_insert_child(btree_node_t *x, btree_node_t *c, int i);

// Insertion helpers
static void btree_split_child(btree_node_t *root, unsigned int child_ind);
static void btree_insert_nonfull(btree_node_t *x, uint64_t k, void *data);

// Deletion helpers
static void btree_fill_node(btree_node_t *x, unsigned int i);
static void btree_merge(btree_node_t *x, unsigned int i);
static void btree_take_prev(btree_node_t *x, unsigned int i);
static void btree_take_next(btree_node_t *x, unsigned int i);
static int btree_get_predecessor(btree_node_t *x, unsigned int i, void **d);
static int btree_get_successor(btree_node_t *x, unsigned int i, void **d);
static void btree_delete_internal(btree_node_t **root, btree_node_t *x, int key, unsigned int i);
static void btree_delete_helper(btree_node_t **root, btree_node_t *x, uint64_t key);

static void print_btree_helper(btree_node_t *x, int level);

static void btree_assert_sanity(btree_node_t *root);

static int btree_pop_key(btree_node_t *x, unsigned int i, void **d)
{
    KASSERT(i < MAX_KEYS);
    KASSERT(i < x->n_keys);
    KASSERT(x->n_keys > 0);

    int ret = x->keys[i];
    if (d != NULL) 
        *d = x->data[i];

    for (int j = i; j < (MAX_KEYS - 1); j++)
    {
        x->keys[j] = x->keys[j+1];
        x->data[j] = x->data[j+1];
    }
    x->keys[x->n_keys-1] = 0;
    x->data[x->n_keys-1] = NULL;
    x->n_keys--;

    return ret;
}

static void btree_append_key(btree_node_t *x, int k, void *d)
{
    KASSERT(x->n_keys < MAX_KEYS && "Adding key to full node");
    x->keys[x->n_keys] = k;
    x->data[x->n_keys] = d;
    x->n_keys++;
}

static void btree_insert_key(btree_node_t *x, int k, int i, void *d)
{
    KASSERT(x->n_keys < MAX_KEYS && "Inserting key to full node");
    for (int j = MAX_KEYS-1; j > i; j--)
    {
        x->keys[j] = x->keys[j-1];
        x->data[j] = x->data[j-1];
    }

    x->keys[i] = k;
    x->data[i] = d;
    x->n_keys++;
}

static btree_node_t *btree_pop_child(btree_node_t *x, unsigned int i)
{
    KASSERT(i < MAX_CHILDREN);
    KASSERT(i < x->n_children);
    KASSERT(x->n_children > 0);

    btree_node_t *ret = x->children[i];
    
    for (int j = i; j < MAX_CHILDREN-1; j++)
    {
        x->children[j] = x->children[j+1];
    }

    x->children[x->n_children-1] = NULL;
    x->n_children--;

    return ret;
}

static void btree_append_child(btree_node_t *x, btree_node_t *addr)
{
    KASSERT(x->n_children < MAX_CHILDREN && "Adding child to full node");
    x->children[x->n_children] = addr;
    x->n_children++;
    x->is_leaf = 0;
}

static void btree_insert_child(btree_node_t *x, btree_node_t *c, int i)
{
    KASSERT(x->n_children < MAX_CHILDREN && "Inserting child to full node");
    for (int j = MAX_CHILDREN-1; j > i; j--)
    {
        x->children[j] = x->children[j-1];
    }

    x->children[i] = c;
    x->n_children++;
    x->is_leaf = 0;
}

static void btree_split_child(btree_node_t *root, unsigned int child_ind)
{
    KASSERT(child_ind < 2*BRANCHING_FACTOR);
    // to_split is a *full* child of root
    btree_node_t *to_split = root->children[child_ind];
    KASSERT(to_split->n_keys == 2*BRANCHING_FACTOR - 1);

    // create new child and insert into parents list of children
    btree_node_t *new_child = btree_node_create();
    new_child->is_leaf = to_split->is_leaf;
    
    btree_insert_child(root, new_child, child_ind+1);

    // insert median of child as new key in parent
    void *d;
    int k = btree_pop_key(to_split, BRANCHING_FACTOR-1, &d);
    btree_insert_key(root, k, child_ind, d);

    // give new_child the second half of to_split's keys
    for (unsigned int i = BRANCHING_FACTOR-1; i < MAX_KEYS-1; i++)
    {
        k = btree_pop_key(to_split, i, &d);
        btree_append_key(new_child, k, d);
    }


    // give new_child the second half of to_split's children (if it's not a leaf)
    if (!to_split->is_leaf)
    {
        for (unsigned int i = BRANCHING_FACTOR; i < 2*BRANCHING_FACTOR; i++)
        {
            btree_append_child(new_child, btree_pop_child(to_split, BRANCHING_FACTOR));
        }
    }
}

// DONE
static void btree_insert_nonfull(btree_node_t *x, uint64_t k, void *data)
{
    int i = x->n_keys - 1;

    if (x->is_leaf)
    {
        while (i >= 0 && k < x->keys[i])
        {
            x->keys[i+1] = x->keys[i];
            x->data[i+1] = x->data[i];
            i--;
        }

        x->keys[i+1] = k;
        x->data[i+1] = data;
        x->n_keys++;
    }
    else
    {
        while (i >= 0 && k < x->keys[i])
        {
            i -= 1;
        }
        i += 1;

        if (x->children[i]->n_keys == MAX_KEYS)
        {
            btree_split_child(x, i);
            if (k > x->keys[i])
            {
                i += 1;
            }
        }
        
        btree_insert_nonfull(x->children[i], k, data);
    }
}

//DONE
static void btree_delete_internal(btree_node_t **root, btree_node_t *x, int key, unsigned int i)
{
    KASSERT(i < x->n_keys && i < x->n_children-1);
    int k = x->keys[i];

    if (x->children[i]->n_keys >= BRANCHING_FACTOR)
    {
        void *d;
        int k = btree_get_predecessor(x, i, &d);
        x->keys[i] = k;
        x->data[i] = d;
        btree_delete(&x->children[i], k);
    }
    else if (x->children[i+1]->n_keys >= BRANCHING_FACTOR)
    {
        void *d;
        int k = btree_get_successor(x, i, &d);
        x->keys[i] = k;
        x->data[i] = d;
        btree_delete(&x->children[i+1], k);
    }
    else
    {
        btree_merge(x, i);
        btree_delete(&x->children[i], k);
    }
}

static int btree_get_predecessor(btree_node_t *x, unsigned int i, void **d)
{
    btree_node_t *cur = x->children[i];
    while (!cur->is_leaf)
        cur = cur->children[cur->n_children-1];
    
    *d = cur->data[cur->n_keys-1];
    return cur->keys[cur->n_keys-1];
}

static int btree_get_successor(btree_node_t *x, unsigned int i, void **d)
{
    btree_node_t *cur = x->children[i+1];
    while (!cur->is_leaf)
        cur = cur->children[0];
    
    *d = cur->data[0];
    return cur->keys[0];
}


/*
 * The actual delete function logic. Just used in a wrapper to handle the 
 * passing the original root down in recursion
 */
static void btree_delete_helper(btree_node_t **root, btree_node_t *x, uint64_t key)
{
    unsigned int i = 0;

    while (i < x->n_keys && key > x->keys[i]) i++;

    // Simplest deletion case. most keys in a btree are in leaves, so we hit this often
    if (x->is_leaf)
    {
        if (i < x->n_keys && x->keys[i] == key)
        {
            btree_pop_key(x, i, NULL);
        }
        return;
    }

    if (i < x->n_keys && x->keys[i] == key)
    {
        btree_delete_internal(root, x, key, i);
    }
    else
    {
        int last = i == x->n_keys;

        if (x->children[i]->n_keys < BRANCHING_FACTOR)
            btree_fill_node(x, i);

        if (last && i > x->n_keys)
            btree_delete_helper(root, x->children[i-1], key);
        else
            btree_delete_helper(root, x->children[i], key);
    }
}

static void btree_take_prev(btree_node_t *x, unsigned int i)
{
    KASSERT(i > 0);
    btree_node_t *c = x->children[i];
    btree_node_t *s = x->children[i-1];
    
    void *d;
    int k = btree_pop_key(x, i-1, &d);
    btree_insert_key(c, k, 0, d);

    if (!c->is_leaf)
        btree_insert_child(c, btree_pop_child(s, s->n_children-1), 0);

    k = btree_pop_key(s, s->n_keys-1, &d);
    btree_insert_key(x, k, i-1, d);
}

static void btree_take_next(btree_node_t *x, unsigned int i)
{
    KASSERT(i < x->n_children-1);
    btree_node_t *c = x->children[i];
    btree_node_t *s = x->children[i+1];

    void *d;
    int k = btree_pop_key(x, i, &d);
    btree_append_key(c, k, d);

    if (!c->is_leaf)
        btree_append_child(c, btree_pop_child(s, 0));
    
    k = btree_pop_key(s, 0, &d);
    btree_insert_key(x, k, i, d);
}

static void btree_fill_node(btree_node_t *x, unsigned int i)
{
    if (i != 0 && x->children[i-1]->n_keys >= BRANCHING_FACTOR)
        btree_take_prev(x, i);

    else if (i != x->n_keys && x->children[i+1]->n_keys >= BRANCHING_FACTOR)
        btree_take_next(x, i);
    else
    {
        if (i != x->n_keys)
            btree_merge(x, i);
        else
            btree_merge(x, i-1);
    }
}

static void btree_merge(btree_node_t *x, unsigned int i)
{
    KASSERT(i < x->n_children-1);
    btree_node_t *c = x->children[i];
    btree_node_t *s = x->children[i+1];

    void *d;
    int k = btree_pop_key(x, i, &d);
    btree_append_key(c, k, d);

    for (unsigned int j = 0; j < s->n_keys; j++)
        btree_append_key(c, s->keys[j], s->data[j]);
    
    if (!c->is_leaf)
    {
        for (unsigned int j = 0; j <= s->n_keys; j++)
            btree_append_child(c, s->children[j]);
    }

    btree_pop_child(x, i+1);

    btree_node_free(&s);
}

    // c->keys[j+BRANCHING_FACTOR] = s->keys[j];

    // for (int j = i+1; j <= x->n_keys; j++)
    //     x->keys[j-1] = x->keys[j];

    // for (int j = i+2; j <= x->n_keys; j++)
    //     x->children[i-1] = x->children[j];
    
    // c->n_keys += s->n_keys+1;
    // x->n_keys--;


// Initializes the allocator for btree nodes
void btree_init()
{
    btree_node_allocator = slab_allocator_create("btree_node", sizeof(btree_node_t));
}

// Insertion, searching, deletion, and other helper functions go here
// ...

btree_node_t *btree_node_create()
{
    btree_node_t *bt = slab_obj_alloc(btree_node_allocator);
    if (!bt)
    {
        return NULL;
    }

    memset(bt, 0, sizeof(btree_node_t));
    
    bt->is_leaf = 1;

    return bt;
}

void *btree_search(btree_node_t *root, uint64_t key)
{
    if (!root)
        return NULL;
    unsigned int ind = 0;
    while (ind < root->n_keys && key > root->keys[ind]) ind++;

    if (ind < root->n_keys && key == root->keys[ind]) return root->data[ind];
    // if (ind < root->n_keys && key == root->keys[ind]) return (void *)0xdeadbeef;
    else if (root->is_leaf) return NULL;

    return btree_search(root->children[ind], key);
}

void btree_insert(btree_node_t **root, uint64_t key, void *data)
{
    if (!(*root))
        *root = btree_node_create();

    if ((*root)->n_keys == MAX_KEYS)
    {
        // TODO should return the new root somehow, because if its full when user passes in
        // the root of the btree gets changed here
        btree_node_t *new_root = btree_node_create();
        btree_append_child(new_root, *root);

        btree_split_child(new_root, 0);
        btree_insert_nonfull(new_root, key, data);
        *root = new_root;
    }
    else
    {
        btree_insert_nonfull(*root, key, data);
    }

    btree_assert_sanity(*root);
}

void btree_delete(btree_node_t **root, uint64_t key)
{
    if (btree_search(*root, key) != NULL) btree_delete_helper(root, *root, key);
    else panic("attempted to delete something not in tree");

    if ((*root)->n_keys == 0)
    {
        btree_node_t *tmp = *root;
        if ((*root)->is_leaf)
            *root = NULL;
        else
            *root = (*root)->children[0];
        
        btree_node_free(&tmp);
    }

    btree_assert_sanity(*root);
}

void btree_node_free(btree_node_t **node)
{
    slab_obj_free(btree_node_allocator, *node);
    *node = NULL;
}

void btree_destroy(btree_node_t *root)
{
    if (root)
    {
        // Recursively free nodes
        // ...
        // free(tree);
    }
}

void print_btree(btree_node_t *x)
{
    print_btree_helper(x, 0);
}

static void print_btree_helper(btree_node_t *x, int level)
{
    dbg_print("Level %d ", level);
    for (unsigned int i = 0; i < x->n_keys; i++)
    {
        // KASSERT(x->keys[i] == (uint64_t)x->data[i]);
        dbg_print("%lu:%p,", x->keys[i], x->data[i]);
    }

    dbg_print("\n");

    level++;

    if (x->n_children > 0)
    {
        for (unsigned int i = 0; i < x->n_children; i++)
        {
            print_btree_helper(x->children[i], level);
        }
    }
}

void btree_assert_sanity(btree_node_t *root)
{
    if (!root) return;

    KASSERT(root->n_keys > 0);

    for (unsigned int i = 0; i < root->n_keys; i++)
    {
        KASSERT(((pframe_t *)root->data[i])->pf_pagenum == root->keys[i]);
    }

    if (root->n_children == 0 || root->is_leaf)
    {
        KASSERT((root->n_children == 0) == root->is_leaf);
        return;
    }

    for (unsigned int i = 0; i < root->n_children; i++)
        btree_assert_sanity(root->children[i]);
}