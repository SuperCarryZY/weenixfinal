#pragma once

#include "proc/kmutex.h"
#include "util/atomic.h"
#include "util/list.h"
#include "util/btree.h"
#include "mm/pframe.h"

struct pframe;

struct mobj;

typedef enum
{
    MOBJ_VNODE = 1,
    MOBJ_SHADOW,
    MOBJ_ANON,
    MOBJ_FS,
} mobj_type_t;

typedef struct mobj_ops
{
    long (*get_pframe)(struct mobj *o, uint64_t pagenum, long forwrite,
                       struct pframe **pfp);

    long (*fill_pframe)(struct mobj *o, struct pframe *pf);

    long (*flush_pframe)(struct mobj *o, struct pframe *pf);

    void (*destructor)(struct mobj *o);
} mobj_ops_t;

typedef struct mobj
{
    long mo_type;
    struct mobj_ops mo_ops;
    atomic_t mo_refcount;
    list_t mo_pframes;
    kmutex_t mo_mutex;
    btree_node_t *mo_btree;
} mobj_t;

void mobj_init(mobj_t *o, long type, mobj_ops_t *ops);

void mobj_lock(mobj_t *o);

void mobj_unlock(mobj_t *o);

void mobj_ref(mobj_t *o);

void mobj_put(mobj_t **op);

void mobj_put_locked(mobj_t **op);

long mobj_get_pframe(mobj_t *o, uint64_t pagenum, long forwrite,
                     struct pframe **pfp);

void mobj_find_pframe(mobj_t *o, uint64_t pagenum, struct pframe **pfp);

long mobj_flush_pframe(mobj_t *o, struct pframe *pf);

long mobj_flush(mobj_t *o);

long mobj_free_pframe(mobj_t *o, struct pframe **pfp);

void mobj_delete_pframe(mobj_t *o, size_t pagenum);

long mobj_default_get_pframe(mobj_t *o, uint64_t pagenum, long forwrite,
                             struct pframe **pfp);

void mobj_default_destructor(mobj_t *o);

void mobj_create_pframe(mobj_t *o, uint64_t pagenum, uint64_t loc, pframe_t **pfp);
