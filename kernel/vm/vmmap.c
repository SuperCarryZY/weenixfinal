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
#include "util/list.h"
#include "mm/pframe.h"

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
    vmarea_t *vma = slab_obj_alloc(vmarea_allocator);
    if (!vma) {
        return NULL;
    }
    
    // Initialize all fields to zero
    memset(vma, 0, sizeof(vmarea_t));
    
    // Initialize the list link
    list_link_init(&vma->vma_plink);
    
    return vma;
}

/*
 * Free the vmarea by removing it from any lists it may be on, putting its
 * vma_obj if it exists, and freeing the vmarea_t.
 */
void vmarea_free(vmarea_t *vma)
{
    if (!vma) {
        return;
    }
    
    // Remove from any lists it may be on
    if (list_link_is_linked(&vma->vma_plink)) {
        list_remove(&vma->vma_plink);
    }
    
    // Put the memory object if it exists
    if (vma->vma_obj) {
        mobj_put(&vma->vma_obj);
    }
    
    // Free the vmarea
    slab_obj_free(vmarea_allocator, vma);
}

/*
 * Create and initialize a new vmmap. Initialize all the fields of vmmap_t.
 */
vmmap_t *vmmap_create(void)
{
    vmmap_t *map = slab_obj_alloc(vmmap_allocator);
    if (!map) {
        return NULL;
    }
    
    // Initialize all fields to zero to avoid dirty data from slab allocator
    memset(map, 0, sizeof(vmmap_t));
    
    // Initialize the list of virtual memory areas
    list_init(&map->vmm_list);
    
    // Initialize the process pointer to NULL
    map->vmm_proc = NULL;
    
    return map;
}

/*
 * Destroy the map pointed to by mapp and set *mapp = NULL.
 * Remember to free each vma in the maps list.
 */
void vmmap_destroy(vmmap_t **mapp)
{
    if (!mapp || !*mapp) {
        return;
    }
    
    vmmap_t *map = *mapp;
    
    // Free each vmarea in the list
    vmarea_t *vma, *next_vma;
    list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink) {
        vmarea_free(vma);
    }
    
    // Free the vmmap itself
    slab_obj_free(vmmap_allocator, map);
    
    // Set the pointer to NULL
    *mapp = NULL;
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
    dbg(DBG_VM, "[vmmap_insert] pid=%d, vma_start=%lu, vma_end=%lu\n", curproc ? curproc->p_pid : -1, (unsigned long)new_vma->vma_start, (unsigned long)new_vma->vma_end);
    KASSERT(map && new_vma);
    KASSERT(new_vma->vma_start < new_vma->vma_end);
    KASSERT(new_vma->vma_start >= ADDR_TO_PN(USER_MEM_LOW));
    KASSERT(new_vma->vma_end <= ADDR_TO_PN(USER_MEM_HIGH));
    
    // Set the vmmap pointer
    new_vma->vma_vmmap = map;
    
    // If the list is empty, just add it
    if (list_empty(&map->vmm_list)) {
        list_insert_head(&map->vmm_list, &new_vma->vma_plink);
        return;
    }
    
    // Find the correct position to insert (keep sorted by start address)
    vmarea_t *vma;
    list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink) {
        if (new_vma->vma_start < vma->vma_start) {
            // Insert before this vma
            list_insert_before(&vma->vma_plink, &new_vma->vma_plink);
            return;
        }
    }
    
    // If we get here, insert at the end
    list_insert_tail(&map->vmm_list, &new_vma->vma_plink);
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
    KASSERT(map);
    KASSERT(dir == VMMAP_DIR_LOHI || dir == VMMAP_DIR_HILO);
    
    if (npages == 0) {
        return -1;
    }
    
    // If no vmareas exist, return the appropriate starting address
    if (list_empty(&map->vmm_list)) {
        if (dir == VMMAP_DIR_HILO) {
            // Start from high address
            size_t start = ADDR_TO_PN(USER_MEM_HIGH) - npages;
            return start;
        } else {
            // Start from low address
            size_t start = ADDR_TO_PN(USER_MEM_LOW);
            return start;
        }
    }
    
    // Find gaps between existing vmareas
    vmarea_t *vma;
    size_t gap_start, gap_end;
    
    if (dir == VMMAP_DIR_HILO) {
        // Check gap before the first vma (at high address)
        vma = list_head(&map->vmm_list, vmarea_t, vma_plink);
        size_t high_start = ADDR_TO_PN(USER_MEM_HIGH) - npages;
        
        // If the first vma starts after our desired high address, we can use the high address
        if (vma->vma_start > high_start) {
            return high_start;
        }
        
        // Check if there's a gap before the first vma that's big enough
        if (vma->vma_start >= npages) {
            size_t candidate_start = vma->vma_start - npages;
            return candidate_start;
        }
        
        // Check gaps between vmareas
        vmarea_t *prev_vma = NULL;
        list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink) {
            if (prev_vma) {
                gap_start = prev_vma->vma_end;
                gap_end = vma->vma_start;
                
                if (gap_end - gap_start >= npages) {
                    // Found a gap big enough
                    size_t candidate_start = gap_end - npages;
                    return candidate_start;
                }
            }
            prev_vma = vma;
        }
        
        // Check gap after the last vma
        if (prev_vma) {
            gap_start = prev_vma->vma_end;
            gap_end = ADDR_TO_PN(USER_MEM_HIGH);
            
            if (gap_end - gap_start >= npages) {
                size_t candidate_start = gap_end - npages;
                return candidate_start;
            }
        }
    } else {
        // VMMAP_DIR_LOHI - start from low address
        // Check gap before the first vma
        vma = list_head(&map->vmm_list, vmarea_t, vma_plink);
        size_t low_start = ADDR_TO_PN(USER_MEM_LOW);
        if (vma->vma_start >= low_start + npages) {
            return low_start;
        }
        
        // Check gaps between vmareas
        vmarea_t *prev_vma = NULL;
        list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink) {
            if (prev_vma) {
                gap_start = prev_vma->vma_end;
                gap_end = vma->vma_start;
                
                if (gap_end - gap_start >= npages) {
                    // Found a gap big enough
                    return gap_start;
                }
            }
            prev_vma = vma;
        }
        
        // Check gap after the last vma
        if (prev_vma) {
            gap_start = prev_vma->vma_end;
            gap_end = ADDR_TO_PN(USER_MEM_HIGH);
            
            if (gap_end - gap_start >= npages) {
                return gap_start;
            }
        }
    }
    
    return -1;
}

/*
 * Look up the vmarea that contains the given virtual frame number.
 * Returns NULL if no such vmarea exists.
 */
vmarea_t *vmmap_lookup(vmmap_t *map, size_t vfn)
{
    KASSERT(map);
    
    vmarea_t *vma;
    list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink) {
        if (vfn >= vma->vma_start && vfn < vma->vma_end) {
            return vma;
        }
    }
    
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
 * Clone a vmmap by creating a new vmmap and copying all vmareas from the original.
 * This is used in fork() to create a copy of the parent's address space.
 */
vmmap_t *vmmap_clone(vmmap_t *map)
{
    KASSERT(map);
    
    vmmap_t *new_map = vmmap_create();
    if (!new_map) {
        return NULL;
    }
    
    // Copy each vmarea from the original map
    vmarea_t *vma;
    list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink) {
        vmarea_t *new_vma = vmarea_alloc();
        if (!new_vma) {
            vmmap_destroy(&new_map);
            return NULL;
        }
        
        // Copy the vmarea structure
        *new_vma = *vma;
        new_vma->vma_vmmap = new_map;
        list_link_init(&new_vma->vma_plink);
        
        // Increment reference count on the memory object
        if (new_vma->vma_obj) {
            mobj_ref(new_vma->vma_obj);
        }
        
        // Insert into the new map
        vmmap_insert(new_map, new_vma);
    }
    
    return new_map;
}

/*
 * Map a file or anonymous memory into the address space.
 * This is the core function used by mmap() system call.
 */
long vmmap_map(vmmap_t *map, vnode_t *file, size_t lopage, size_t npages,
               int prot, int flags, off_t off, int dir, vmarea_t **new_vma)
{
    KASSERT(map);
    
    if (npages == 0) {
        return -EINVAL;
    }
    
    // Find a range for the mapping
    ssize_t start_vfn = vmmap_find_range(map, npages, dir);
    if (start_vfn < 0) {
        return -ENOMEM;
    }
    
    // Create a new vmarea
    vmarea_t *vma = vmarea_alloc();
    if (!vma) {
        return -ENOMEM;
    }
    
    // Initialize the vmarea
    vma->vma_start = start_vfn;
    vma->vma_end = start_vfn + npages;
    vma->vma_off = ADDR_TO_PN(off);
    vma->vma_prot = prot;
    vma->vma_flags = flags;
    vma->vma_vmmap = map;
    
    // Create the appropriate memory object
    if (flags & MAP_ANON) {
        // Anonymous mapping
        vma->vma_obj = anon_create();
        if (!vma->vma_obj) {
            vmarea_free(vma);
            return -ENOMEM;
        }
    } else if (file) {
        // File mapping
        if (!file->vn_ops->mmap) {
            vmarea_free(vma);
            return -ENODEV;
        }
        
        // Call the filesystem's mmap function
        long result = file->vn_ops->mmap(file, &vma->vma_obj);
        if (result < 0) {
            vmarea_free(vma);
            return result;
        }
    } else {
        // Should not happen
        vmarea_free(vma);
        return -EINVAL;
    }
    
    // Insert the vmarea into the map
    vmmap_insert(map, vma);
    
    if (new_vma) {
        *new_vma = vma;
    }
    
    return 0;
}

/*
 * Remove the virtual memory areas that overlap with the range [lopage, lopage + npages).
 * This may require splitting or truncating existing vmareas.
 */
long vmmap_remove(vmmap_t *map, size_t lopage, size_t npages)
{
    KASSERT(map);
    
    if (npages == 0) {
        return 0;
    }
    
    size_t hipage = lopage + npages;
    
    vmarea_t *vma, *next_vma;
    list_iterate(&map->vmm_list, vma, vmarea_t, vma_plink) {
        // Check if this vma overlaps with the range to remove
        if (vma->vma_end <= lopage || vma->vma_start >= hipage) {
            // No overlap, skip this vma
            continue;
        }
        
        // There is overlap, need to handle it
        if (vma->vma_start < lopage && vma->vma_end > hipage) {
            // The vma completely contains the range to remove
            // Split into two vmareas
            
            // Create new vma for the high part
            vmarea_t *high_vma = vmarea_alloc();
            KASSERT(high_vma);
            
            // Copy properties to high vma
            *high_vma = *vma;
            high_vma->vma_start = hipage;
            high_vma->vma_off = vma->vma_off + (hipage - vma->vma_start);
            list_link_init(&high_vma->vma_plink);
            
            // Truncate the original vma
            vma->vma_end = lopage;
            
            // Insert the high vma after the original
            list_insert_before(&vma->vma_plink, &high_vma->vma_plink);
            
        } else if (vma->vma_start < lopage) {
            // The vma overlaps with the low end of the range
            // Truncate the vma
            vma->vma_end = lopage;
            
        } else if (vma->vma_end > hipage) {
            // The vma overlaps with the high end of the range
            // Adjust the vma
            vma->vma_off += (lopage - vma->vma_start);
            vma->vma_start = lopage;
            vma->vma_end = hipage;
            
        } else {
            // The vma is completely contained in the range to remove
            // Remove it entirely
            list_remove(&vma->vma_plink);
            vmarea_free(vma);
        }
    }
    
    return 0;
}

/*
 * Check if the range [startvfn, startvfn + npages) is empty (i.e., no vmarea
 * overlaps with this range). Returns 0 if the range is empty, -1 if it overlaps
 * with any existing vmarea.
 */
long vmmap_is_range_empty(vmmap_t *map, size_t startvfn, size_t npages)
{
    KASSERT(map);

    if (npages == 0) {
        return 0;
    }

    size_t endvfn = startvfn + npages;

    list_link_t *link;
    list_t *list = &map->vmm_list;
    for (link = list->l_next; link != list; link = link->l_next) {
        vmarea_t *vma = list_item(link, vmarea_t, vma_plink);
        if (!(endvfn <= vma->vma_start || startvfn >= vma->vma_end)) {
            return -1;
        }
    }

    return 0;
}

/*
 * Read data from a virtual memory address range into a kernel buffer.
 * This function handles page faults by accessing the memory through the
 * corresponding memory objects for each vmarea.
 */
long vmmap_read(vmmap_t *map, const void *vaddr, void *buf, size_t count)
{
    KASSERT(map && buf);
    
    if (count == 0) {
        return 0;
    }
    
    uintptr_t start_addr = (uintptr_t)vaddr;
    uintptr_t end_addr = start_addr + count;
    size_t bytes_read = 0;
    
    // Read data page by page
    uintptr_t current_addr = start_addr;
    while (current_addr < end_addr) {
        // Find the vmarea for this address
        size_t vfn = ADDR_TO_PN(current_addr);
        vmarea_t *vma = vmmap_lookup(map, vfn);
        
        if (!vma) {
            return -EFAULT;
        }
        
        // Calculate how much to read from this page
        uintptr_t page_start = PAGE_ALIGN_DOWN(current_addr);
        uintptr_t page_end = PAGE_ALIGN_UP(current_addr + 1);
        size_t page_offset = current_addr - page_start;
        size_t bytes_in_page = page_end - current_addr;
        
        if (bytes_in_page > (end_addr - current_addr)) {
            bytes_in_page = end_addr - current_addr;
        }
        
        // Calculate offset into the memory object
        size_t obj_offset = vfn - vma->vma_start + vma->vma_off;
        
        // Get the pframe from the memory object
        pframe_t *pf = mobj_get_pframe(vma->vma_obj, obj_offset, 0, 0);
        if (!pf) {
            return -EFAULT;
        }
        
        // Copy data from the pframe to the buffer
        memcpy((char *)buf + bytes_read, (char *)pf->pf_addr + page_offset, bytes_in_page);
        
        // Move to next page
        current_addr += bytes_in_page;
        bytes_read += bytes_in_page;
    }
    
    return bytes_read;
}

/*
 * Write data from a kernel buffer to a virtual memory address range.
 * This function handles page faults by accessing the memory through the
 * corresponding memory objects for each vmarea.
 */
long vmmap_write(vmmap_t *map, void *vaddr, const void *buf, size_t count)
{
    KASSERT(map && buf);
    
    if (count == 0) {
        return 0;
    }
    
    uintptr_t start_addr = (uintptr_t)vaddr;
    uintptr_t end_addr = start_addr + count;
    size_t bytes_written = 0;
    
    // Write data page by page
    uintptr_t current_addr = start_addr;
    while (current_addr < end_addr) {
        // Find the vmarea for this address
        size_t vfn = ADDR_TO_PN(current_addr);
        vmarea_t *vma = vmmap_lookup(map, vfn);
        
        if (!vma) {
            return -EFAULT;
        }
        
        // Calculate how much to write to this page
        uintptr_t page_start = PAGE_ALIGN_DOWN(current_addr);
        uintptr_t page_end = PAGE_ALIGN_UP(current_addr + 1);
        size_t page_offset = current_addr - page_start;
        size_t bytes_in_page = page_end - current_addr;
        
        if (bytes_in_page > (end_addr - current_addr)) {
            bytes_in_page = end_addr - current_addr;
        }
        
        // Calculate offset into the memory object
        size_t obj_offset = vfn - vma->vma_start + vma->vma_off;
        
        // Get the pframe from the memory object
        pframe_t *pf = mobj_get_pframe(vma->vma_obj, obj_offset, 0, 0);
        if (!pf) {
            return -EFAULT;
        }
        
        // Copy data from the buffer to the pframe
        memcpy((char *)pf->pf_addr + page_offset, (char *)buf + bytes_written, bytes_in_page);
        
        // Mark the pframe as dirty
        pf->pf_dirty = 1;
        
        // Move to next page
        current_addr += bytes_in_page;
        bytes_written += bytes_in_page;
    }
    
    return bytes_written;
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
