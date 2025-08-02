#include "mm/mobj.h"
#include "mm/page.h"
#include "mm/pframe.h"
#include "mm/slab.h"

#include "util/debug.h"
#include "util/string.h"

/* for debugging/verification purposes */
int anon_count = 0; 

static slab_allocator_t *anon_allocator;

static long anon_fill_pframe(mobj_t *o, pframe_t *pf);

static long anon_flush_pframe(mobj_t *o, pframe_t *pf);

static void anon_destructor(mobj_t *o);

static mobj_ops_t anon_mobj_ops = {.get_pframe = NULL,
                                   .fill_pframe = anon_fill_pframe,
                                   .flush_pframe = anon_flush_pframe,
                                   .destructor = anon_destructor};

/*
 * Initialize anon_allocator using the slab allocator.
 */
void anon_init()
{
    NOT_YET_IMPLEMENTED("VM: anon_init");
}

/*
 * Creates and initializes an anonymous object (mobj).
 * Returns a new anonymous object, or NULL on failure.
 * 
 * There isn't an "anonymous object" type, so use a generic mobj.
 * The mobj should be locked upon successful return. 
 * Use mobj_init and mobj_lock. 
 */
mobj_t *anon_create()
{
    NOT_YET_IMPLEMENTED("VM: anon_create");
    return NULL;
}

/* 
 * This function is not complicated -- think about what the pframe should look
 * like for an anonymous object 
 */
static long anon_fill_pframe(mobj_t *o, pframe_t *pf)
{
    NOT_YET_IMPLEMENTED("VM: anon_fill_pframe");
    return 0;
}

static long anon_flush_pframe(mobj_t *o, pframe_t *pf) { return 0; }

/*
 * Release all resources associated with an anonymous object.
 *
 * Hints:
 *  1) Call mobj_default_destructor() to free pframes
 *  2) Free the mobj
 */
static void anon_destructor(mobj_t *o)
{
    NOT_YET_IMPLEMENTED("VM: anon_destructor");
}
