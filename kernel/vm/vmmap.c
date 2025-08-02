#include "globals.h"
#include "kernel.h"
#include <errno.h>

#include "vm/anon.h"
#include "vm/shadow.h"

#include "util/debug.h"
#include "util/printf.h"
#include "util/string.h"

#include "fs/file.h"
#include "fs/vfs_syscall.h"
#include "fs/vnode.h"

#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/slab.h"

static slab_allocator_t *vmmap_allocator;
static slab_allocator_t *vmarea_allocator;

void vmmap_init(void)
{
    vmmap_allocator = slab_allocator_create("vmmap", sizeof(vmmap_t));
    vmarea_allocator = slab_allocator_create("vmarea", sizeof(vmarea_t));
    KASSERT(vmmap_allocator && vmarea_allocator);
}

/*
 * Allocate and initialize a new vmarea using vmarea_allocator.
 */
vmarea_t *vmarea_alloc(void)
{
    NOT_YET_IMPLEMENTED("VM: vmarea_alloc");
    return NULL;
}

/*
 * Free the vmarea by removing it from any lists it may be on, putting its
 * vma_obj if it exists, and freeing the vmarea_t.
 */
void vmarea_free(vmarea_t *vma)
{
    NOT_YET_IMPLEMENTED("VM: vmarea_free");
}

/*
 * Create and initialize a new vmmap. Initialize all the fields of vmmap_t.
 */
vmmap_t *vmmap_create(void)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_create");
    return NULL;
}

/*
 * Destroy the map pointed to by mapp and set *mapp = NULL.
 * Remember to free each vma in the maps list.
 */
void vmmap_destroy(vmmap_t **mapp)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_destroy");
}

/*
 * Add a vmarea to an address space. Assumes (i.e. asserts to some extent) the
 * vmarea is valid. Iterate through the list of vmareas, and add it 
 * accordingly. 
 * 
 * Hint: when thinking about what constitutes a "valid" vmarea, think about
 * the range that it covers. Can the starting page be lower than USER_MEM_LOW?
 * Can the ending page be higher than USER_MEM_HIGH? Can the start > end?
 * You don't need to explicitly handle these cases, but it may help to 
 * use KASSERTs to catch these aforementioned errors.
 */
void vmmap_insert(vmmap_t *map, vmarea_t *new_vma)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_insert");
}

/*
 * Find a contiguous range of free virtual pages of length npages in the given
 * address space. Returns starting page number for the range, without altering the map.
 * Return -1 if no such range exists.
 *
 * Your algorithm should be first fit. 
 * You should assert that dir is VMMAP_DIR_LOHI OR VMMAP_DIR_HILO.
 * If dir is:
 *    - VMMAP_DIR_HILO: find a gap as high in the address space as possible, 
 *                      starting from USER_MEM_HIGH.
 *    - VMMAP_DIR_LOHI: find a gap as low in the address space as possible, 
 *                      starting from USER_MEM_LOW.
 * 
 * Make sure you are converting between page numbers and addresses correctly! 
 */
ssize_t vmmap_find_range(vmmap_t *map, size_t npages, int dir)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_find_range");
    return -1;
}

/*
 * Return the vm_area that vfn (a page number) lies in. Scan the address space looking
 * for a vma whose range covers vfn. If the page is unmapped, return NULL.
 */
vmarea_t *vmmap_lookup(vmmap_t *map, size_t vfn)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_lookup");
    return NULL;
}

/*
 * For each vmarea in the map, if it is a shadow object, call shadow_collapse.
 */
void vmmap_collapse(vmmap_t *map)
{
    list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink)
    {
        if (vma->vma_obj->mo_type == MOBJ_SHADOW)
        {
            mobj_lock(vma->vma_obj);
            shadow_collapse(vma->vma_obj);
            mobj_unlock(vma->vma_obj);
        }
    }
}

/*
 * This is where the magic of fork's copy-on-write gets set up. 
 * 
 * Upon successful return, the new vmmap should be a clone of map with all 
 * shadow objects properly set up.
 *
 * For each vmarea, clone its members. You should also use vmmap_collapse() 
 * somewhere in this function. If you're unsure why, look at the "Shadow Objects"
 * portion of the VM handout.
 *  1) vmarea is share-mapped, you don't need to do anything special. 
 *  2) vmarea is not share-mapped, time for shadow objects: 
 *     a) Create two shadow objects, one for map and one for the new vmmap you
 *        are constructing, both of which shadow the current vma_obj the vmarea
 *        being cloned. 
 *     b) After creating the shadow objects, put the original vma_obj
 *     c) and insert the shadow objects into their respective vma's.
 *
 * Be sure to clean up in any error case, manage the reference counts correctly,
 * and to lock/unlock properly. When you ref a mobj, make sure the mobj is locked.
 */
vmmap_t *vmmap_clone(vmmap_t *map)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_clone");
    return NULL;
}

/*
 *
 * Insert a mapping into the map starting at lopage for npages pages.
 * 
 *  file    - If provided, the vnode of the file to be mapped in
 *  lopage  - If provided, the desired start range of the mapping
 *  prot    - See mman.h for possible values
 *  flags   - See do_mmap()'s comments for possible values
 *  off     - Offset in the file to start mapping at, in bytes
 *  dir     - VMMAP_DIR_LOHI or VMMAP_DIR_HILO
 *  new_vma - If provided, on success, must point to the new vmarea_t
 * 
 *  Return 0 on success, or:
 *  - ENOMEM: On vmarea_alloc, anon_create, shadow_create or 
 *    vmmap_find_range failure 
 *  - Propagate errors from file->vn_ops->mmap and vmmap_remove
 * 
 * Hints:
 *  - You can assume/assert that all input is valid. It may help to write
 *    this function and do_mmap() somewhat in tandem.
 *  - If file is NULL, create an anon object.
 *  - If file is non-NULL, use the vnode's mmap operation to get the mobj.
 *    Do not assume it is file->vn_obj (mostly relevant for special devices).
 *  - If lopage is 0, use vmmap_find_range() to get a valid range
 *  - If lopage is not 0, the direction flag (dir) is ignored.
 *  - If lopage is nonzero and MAP_FIXED is specified and 
 *    the given range overlaps with any preexisting mappings, 
 *    remove the preexisting mappings.
 *  - If MAP_PRIVATE is specified, set up a shadow object. Be careful with
 *    refcounts!
 *  - Be careful: off is in bytes (albeit should be page-aligned), but
 *    vma->vma_off is in pages.
 *  - Be careful with the order of operations. Hold off on any irreversible
 *    work until there is no more chance of failure.
 */
long vmmap_map(vmmap_t *map, vnode_t *file, size_t lopage, size_t npages,
               int prot, int flags, off_t off, int dir, vmarea_t **new_vma)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_map");
    return -1;
}

/*
 * Iterate over the mapping's vmm_list and make sure that the specified range
 * is completely empty. You will have to handle the following cases:
 *
 * Key:     [             ] = existing vmarea_t
 *              *******     = region to be unmapped
 *
 * Case 1:  [   *******   ]
 * The region to be unmapped lies completely inside the vmarea. We need to
 * split the old vmarea into two vmareas. Be sure to increment the refcount of
 * the object associated with the vmarea.
 *
 * Case 2:  [      *******]**
 * The region overlaps the end of the vmarea. Just shorten the length of
 * the mapping.
 *
 * Case 3: *[*****        ]
 * The region overlaps the beginning of the vmarea. Move the beginning of
 * the mapping (remember to update vma_off), and shorten its length.
 *
 * Case 4: *[*************]**
 * The region completely contains the vmarea. Remove the vmarea from the
 * list.
 * 
 * Return 0 on success, or:
 *  - ENOMEM: Failed to allocate a new vmarea when splitting a vmarea (case 1).
 * 
 * Hints:
 *  - Whenever you shorten/remove any mappings, be sure to call pt_unmap_range()
 *    tlb_flush_range() to clean your pagetables and TLB.
 *  - If you ref a mobj, make sure that the mobj is locked
 */
long vmmap_remove(vmmap_t *map, size_t lopage, size_t npages)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_remove");
    return -1;
}

/*
 * Returns 1 if the given address space has no mappings for the given range,
 * 0 otherwise.
 */
long vmmap_is_range_empty(vmmap_t *map, size_t startvfn, size_t npages)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_is_range_empty");
    return 0;
}

/*
 * Read into 'buf' from the virtual address space of 'map'. Start at 'vaddr'
 * for size 'count'. 'vaddr' is not necessarily page-aligned. count is in bytes.
 * 
 * Hints:
 *  1) Find the vmareas that correspond to the region to read from.
 *  2) Find the pframes within those vmareas corresponding to the virtual 
 *     addresses you want to read.
 *  3) Read from those page frames and copy it into `buf`.
 *  4) You will not need to check the permissisons of the area.
 *  5) You may assume/assert that all areas exist.
 * 
 * Return 0 on success, -errno on error (propagate from the routines called).
 * This routine will be used within copy_from_user(). 
 */
long vmmap_read(vmmap_t *map, const void *vaddr, void *buf, size_t count)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_read");
    return 0;
}

/*
 * Write from 'buf' into the virtual address space of 'map' starting at
 * 'vaddr' for size 'count'.
 * 
 * Hints:
 *  1) Find the vmareas to write to.
 *  2) Find the correct pframes within those areas that contain the virtual addresses
 *     that you want to write data to.
 *  3) Write to the pframes, copying data from buf.
 *  4) You do not need check permissions of the areas you use.
 *  5) Assume/assert that all areas exist.
 *  6) Remember to dirty the pages that you write to. 
 * 
 * Returns 0 on success, -errno on error (propagate from the routines called).
 * This routine will be used within copy_to_user(). 
 */
long vmmap_write(vmmap_t *map, void *vaddr, const void *buf, size_t count)
{
    NOT_YET_IMPLEMENTED("VM: vmmap_write");
    return 0;
}

size_t vmmap_mapping_info(const void *vmmap, char *buf, size_t osize)
{
    return vmmap_mapping_info_helper(vmmap, buf, osize, "");
}

size_t vmmap_mapping_info_helper(const void *vmmap, char *buf, size_t osize,
                                 char *prompt)
{
    KASSERT(0 < osize);
    KASSERT(NULL != buf);
    KASSERT(NULL != vmmap);

    vmmap_t *map = (vmmap_t *)vmmap;
    ssize_t size = (ssize_t)osize;

    int len =
        snprintf(buf, (size_t)size, "%s%37s %5s %7s %18s %11s %23s\n", prompt,
                 "VADDR RANGE", "PROT", "FLAGS", "MOBJ", "OFFSET", "VFN RANGE");

    list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink)
    {
        size -= len;
        buf += len;
        if (0 >= size)
        {
            goto end;
        }

        len =
            snprintf(buf, (size_t)size,
                     "%s0x%p-0x%p  %c%c%c  %7s 0x%p %#.9lx %#.9lx-%#.9lx\n",
                     prompt, (void *)(vma->vma_start << PAGE_SHIFT),
                     (void *)(vma->vma_end << PAGE_SHIFT),
                     (vma->vma_prot & PROT_READ ? 'r' : '-'),
                     (vma->vma_prot & PROT_WRITE ? 'w' : '-'),
                     (vma->vma_prot & PROT_EXEC ? 'x' : '-'),
                     (vma->vma_flags & MAP_SHARED ? " SHARED" : "PRIVATE"),
                     vma->vma_obj, vma->vma_off, vma->vma_start, vma->vma_end);
    }

end:
    if (size <= 0)
    {
        size = osize;
        buf[osize - 1] = '\0';
    }
    return osize - size;
}
