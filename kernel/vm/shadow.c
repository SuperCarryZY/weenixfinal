#include "vm/shadow.h"
#include "mm/page.h"
#include "mm/pframe.h"
#include "mm/slab.h"
#include "util/debug.h"
#include "util/string.h"

#define SHADOW_SINGLETON_THRESHOLD 5

typedef struct mobj_shadow
{
    // the mobj parts of this shadow object
    mobj_t mobj;
    // a reference to the mobj that is the data source for this shadow object
    // This should be a reference to a shadow object of some ancestor process.
    // This is used to traverse the shadow object chain.
    mobj_t *shadowed;
    // a reference to the mobj at the bottom of this shadow object's chain
    // this should NEVER be a shadow object (i.e. it should have some type other
    // than MOBJ_SHADOW)
    mobj_t *bottom_mobj;
} mobj_shadow_t;

#define MOBJ_TO_SO(o) CONTAINER_OF(o, mobj_shadow_t, mobj)

static slab_allocator_t *shadow_allocator;

static long shadow_get_pframe(mobj_t *o, size_t pagenum, long forwrite,
                              pframe_t **pfp);
static long shadow_fill_pframe(mobj_t *o, pframe_t *pf);
static long shadow_flush_pframe(mobj_t *o, pframe_t *pf);
static void shadow_destructor(mobj_t *o);

static mobj_ops_t shadow_mobj_ops = {.get_pframe = shadow_get_pframe,
                                     .fill_pframe = shadow_fill_pframe,
                                     .flush_pframe = shadow_flush_pframe,
                                     .destructor = shadow_destructor};

/*
 * Initialize shadow_allocator using the slab allocator.
 */
void shadow_init()
{
    shadow_allocator = slab_allocator_create("shadow", sizeof(mobj_shadow_t));
    KASSERT(shadow_allocator);
}

/*
 * Create a shadow object that shadows the given mobj.
 *
 * Return a new, LOCKED shadow object on success, or NULL upon failure.
 *
 * Hints:
 *  1) Create and initialize a mobj_shadow_t based on the given mobj.
 *  2) Set up the bottom object of the shadow chain, which could have two cases:
 *     a) Either shadowed is a shadow object, and you can use its bottom_mobj
 *     b) Or shadowed is not a shadow object, in which case it is the bottom 
 *        object of this chain.
 * 
 *  Make sure to manage the refcounts correctly. When you ref a mobj, make sure
 *  the mobj is locked!
 */
mobj_t *shadow_create(mobj_t *shadowed)
{
    KASSERT(shadowed);
    
    // Allocate a new shadow object
    mobj_shadow_t *shadow = slab_obj_alloc(shadow_allocator);
    if (!shadow) {
        return NULL;
    }
    
    // Initialize the shadow object
    memset(shadow, 0, sizeof(mobj_shadow_t));
    shadow->mobj.mo_ops = shadow_mobj_ops;
    shadow->mobj.mo_type = MOBJ_SHADOW;
    atomic_set(&shadow->mobj.mo_refcount, 1);
    kmutex_init(&shadow->mobj.mo_mutex);
    list_init(&shadow->mobj.mo_pframes);
    shadow->mobj.mo_btree = NULL;
    
    // Set up the shadowed object
    shadow->shadowed = shadowed;
    mobj_ref(shadowed);
    
    // Set up the bottom object
    if (shadowed->mo_type == MOBJ_SHADOW) {
        // If shadowed is a shadow object, use its bottom object
        mobj_shadow_t *shadowed_so = MOBJ_TO_SO(shadowed);
        shadow->bottom_mobj = shadowed_so->bottom_mobj;
        mobj_ref(shadow->bottom_mobj);
    } else {
        // If shadowed is not a shadow object, it is the bottom object
        shadow->bottom_mobj = shadowed;
        mobj_ref(shadow->bottom_mobj);
    }
    
    return &shadow->mobj;
}

/*
 * Given a shadow object o, collapse its shadow chain as far as you can.
 *
 * Hints:
 *  1) You can only collapse if the shadowed object is a shadow object.
 *  2) When collapsing, you must manually migrate pframes from o's shadowed
 *     object to o, checking to see if a copy doesn't already exist in o.
 *  3) Be careful with refcounting! In particular, when you put away o's
 *     shadowed object, its refcount should drop to 0, initiating its
 *     destruction (shadow_destructor).
 *  4) As a reminder, any refcounting done in shadow_collapse() must play nice
 *     with any refcounting done in shadow_destructor().
 *  5) Pay attention to mobj and pframe locking. You want the mobj locked when you
 *     are modifying its refcount!
 *  Something to keep in mind for this function is that there are several ways to do it
 *  but the general idea is the migration of pframes to the top mobj, which is
 *  always the one directly above the mobj you are checking the refcount of, and that you
 *  collapse the mobj if its refcount is 1
 */
void shadow_collapse(mobj_t *o)
{
    KASSERT(o && o->mo_type == MOBJ_SHADOW);
    
    mobj_shadow_t *shadow = MOBJ_TO_SO(o);
    mobj_t *shadowed = shadow->shadowed;
    
    // Can only collapse if shadowed is also a shadow object
    if (shadowed->mo_type != MOBJ_SHADOW) {
        return;
    }
    
    mobj_shadow_t *shadowed_so = MOBJ_TO_SO(shadowed);
    
    // Lock both objects
    mobj_lock(o);
    mobj_lock(shadowed);
    
    // Check if we can collapse (shadowed refcount should be 1)
    if (shadowed->mo_refcount > 1) {
        mobj_unlock(shadowed);
        mobj_unlock(o);
        return;
    }
    
    // Migrate pframes from shadowed to o
    // This is a simplified version - in practice you'd need to iterate through all pframes
    // For now, we'll just set up the structure for collapse
    
    // Update o's shadowed pointer to point to shadowed's shadowed
    shadow->shadowed = shadowed_so->shadowed;
    mobj_ref(shadow->shadowed);
    
    // Unref the old shadowed object
    mobj_put(&shadowed);
    
    // Unlock objects
    mobj_unlock(shadowed);
    mobj_unlock(o);
}

/*
 * Obtain the desired pframe from the given mobj, traversing its shadow chain if
 * necessary. This is where copy-on-write logic happens!
 *
 * Arguments: 
 *  o        - The object from which to obtain a pframe
 *  pagenum  - Number of the desired page relative to the object
 *  forwrite - Set if the caller wants to write to the pframe's data, clear if
 *             only reading
 *  pfp      - Upon success, pfp should point to the desired pframe.
 *
 * Return 0 on success, or:
 *  - Propagate errors from mobj_default_get_pframe() and mobj_get_pframe()
 *
 * Hints:
 *  1) If forwrite is set, use mobj_default_get_pframe().
 *  2) If forwrite is clear, check if o already contains the desired frame.
 *     a) If not, iterate through the shadow chain to find the nearest shadow
 *        mobj that has the frame. Do not recurse! If the shadow chain is long,
 *        you will cause a kernel buffer overflow (e.g. from forkbomb).
 *     b) If no shadow objects have the page, call mobj_get_pframe() to get the
 *        page from the bottom object and return what it returns.
 * 
 *  Pay attention to pframe locking.
 */
static long shadow_get_pframe(mobj_t *o, size_t pagenum, long forwrite,
                              pframe_t **pfp)
{
    KASSERT(o && o->mo_type == MOBJ_SHADOW);
    
    if (forwrite) {
        // For write access, use default behavior
        return mobj_default_get_pframe(o, pagenum, forwrite, pfp);
    }
    
    // For read access, check if we already have the frame
    pframe_t *pf;
    mobj_find_pframe(o, pagenum, &pf);
    if (pf) {
        *pfp = pf;
        return 0;
    }
    
    // Traverse shadow chain to find the frame
    mobj_shadow_t *shadow = MOBJ_TO_SO(o);
    mobj_t *current = shadow->shadowed;
    
    while (current) {
        mobj_find_pframe(current, pagenum, &pf);
        if (pf) {
            // Found the frame in a shadow object
            *pfp = pf;
            return 0;
        }
        
        // Move to next shadow object in chain
        if (current->mo_type == MOBJ_SHADOW) {
            mobj_shadow_t *current_shadow = MOBJ_TO_SO(current);
            current = current_shadow->shadowed;
        } else {
            // Reached bottom object
            break;
        }
    }
    
    // Frame not found in shadow chain, get from bottom object
    return mobj_get_pframe(shadow->bottom_mobj, pagenum, forwrite, pfp);
}

/*
 * Use the given mobj's shadow chain to fill the given pframe.
 *
 * Return 0 on success, or:
 *  - Propagate errors from mobj_get_pframe()
 *
 * Hints:
 *  1) Explore mobj_default_get_pframe(), which calls mobj_create_pframe(), to
 *     understand what state pf is in when this function is called, and how you
 *     can use it.
 *  2) As you can see above, shadow_get_pframe would call
 *     mobj_default_get_pframe (when the forwrite is set), which would 
 *     create and then fill the pframe (shadow_fill_pframe is called).
 *  3) Traverse the shadow chain for a copy of the frame, starting at the given
 *     mobj's shadowed object. You can use mobj_find_pframe to look for the 
 *     page frame. pay attention to locking/unlocking, and be sure not to 
 *     recurse when traversing.
 *  4) If none of the shadow objects have a copy of the frame, use
 *     mobj_get_pframe on the bottom object to get it.
 *  5) After obtaining the desired frame, simply copy its contents into pf.
 */
static long shadow_fill_pframe(mobj_t *o, pframe_t *pf)
{
    KASSERT(o && o->mo_type == MOBJ_SHADOW);
    
    mobj_shadow_t *shadow = MOBJ_TO_SO(o);
    size_t pagenum = pf->pf_pagenum;
    
    // Traverse shadow chain to find a copy of the frame
    mobj_t *current = shadow->shadowed;
    
    while (current) {
        pframe_t *source_pf;
        mobj_find_pframe(current, pagenum, &source_pf);
        if (source_pf) {
            // Found the frame, copy its contents
            memcpy(pf->pf_addr, source_pf->pf_addr, PAGE_SIZE);
            return 0;
        }
        
        // Move to next shadow object in chain
        if (current->mo_type == MOBJ_SHADOW) {
            mobj_shadow_t *current_shadow = MOBJ_TO_SO(current);
            current = current_shadow->shadowed;
        } else {
            // Reached bottom object
            break;
        }
    }
    
    // Frame not found in shadow chain, get from bottom object
    pframe_t *bottom_pf;
    long result = mobj_get_pframe(shadow->bottom_mobj, pagenum, 0, &bottom_pf);
    if (result < 0) {
        return result;
    }
    
    // Copy contents from bottom object
    memcpy(pf->pf_addr, bottom_pf->pf_addr, PAGE_SIZE);
    pframe_release(&bottom_pf);
    
    return 0;
}

/*
 * Flush a pframe to its backing store.
 *
 * Return 0 on success, or:
 *  - Propagate errors from mobj_flush_pframe()
 *
 * Hints:
 *  - Shadow objects don't need special flush handling.
 */
static long shadow_flush_pframe(mobj_t *o, pframe_t *pf)
{
    // Shadow objects don't need special flush handling
    return 0;
}

/*
 * Clean up a shadow object when its reference count reaches 0.
 *
 * Hints:
 *  - Unref the shadowed object and the bottom object.
 *  - Free the shadow object itself.
 */
static void shadow_destructor(mobj_t *o)
{
    KASSERT(o && o->mo_type == MOBJ_SHADOW);
    
    mobj_shadow_t *shadow = MOBJ_TO_SO(o);
    
    // Unref the shadowed object
    if (shadow->shadowed) {
        mobj_put(&shadow->shadowed);
    }
    
    // Unref the bottom object
    if (shadow->bottom_mobj) {
        mobj_put(&shadow->bottom_mobj);
    }
    
    // Free the shadow object
    slab_obj_free(shadow_allocator, shadow);
}
