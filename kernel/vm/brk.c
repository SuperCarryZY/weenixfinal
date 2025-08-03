#include "errno.h"
#include "globals.h"
#include "mm/mm.h"
#include "util/debug.h"

#include "mm/mman.h"

/*
 * This function implements the brk(2) system call.
 *
 * This routine manages the calling process's "break" -- the ending address
 * of the process's dynamic region (heap)
 *
 * Some important details on the range of values 'p_brk' can take:
 * 1) 'p_brk' should not be set to a value lower than 'p_start_brk', since this
 *    could overrite data in another memory region. But, 'p_brk' can be equal to
 *    'p_start_brk', which would mean that there is no heap yet/is empty.
 * 2) Growth of the 'p_brk' cannot overlap with/expand into an existing
 *    mapping. Use vmmap_is_range_empty() to help with this.
 * 3) 'p_brk' cannot go beyond the region of the address space allocated for use by
 *    userland (USER_MEM_HIGH)
 *
 * Before setting 'p_brk' to 'addr', you must account for all scenarios by comparing
 * the page numbers of addr, 'p_brk' and 'p_start_brk' as the vmarea that represents the heap
 * has page granularity. Think about the following sub-cases (note that the heap 
 * should always be represented by at most one vmarea):
 * 1) The heap needs to be created. What permissions and attributes does a process
 *    expect the heap to have?
 * 2) The heap already exists, so you need to modify its end appropriately.
 * 3) The heap needs to shrink.
 *
 * Beware of page alignment!:
 * 1) The starting break is not necessarily page aligned. Since the loader sets
 *    'p_start_brk' to be the end of the bss section, 'p_start_brk' should always be
 *    aligned up to start the dynamic region at the first page after bss_end.
 * 2) vmareas only have page granularity, so you will need to take this
 *    into account when deciding how to set the mappings if p_brk or p_start_brk
 *    is not page aligned. The caller of do_brk() would be very disappointed if
 *    you give them less than they asked for!
 *
 * Some additional details:
 * 1) You are guaranteed that the process data/bss region is non-empty.
 *    That is, if the starting brk is not page-aligned, its page has
 *    read/write permissions.
 * 2) If 'addr' is NULL, you should return the current break. We use this to
 *    implement sbrk(0) without writing a separate syscall. Look in
 *    user/libc/syscall.c if you're curious.
 * 3) Return 0 on success, -errno on failure. The 'ret' argument should be used to 
 *    return the updated 'p_brk' on success.
 *
 * Error cases do_brk is responsible for generating:
 *  - ENOMEM: attempting to set p_brk beyond its valid range
 */
long do_brk(void *addr, void **ret)
{
    KASSERT(curproc);
    
    // If addr is NULL, return current break
    if (!addr) {
        // If p_brk is NULL, initialize it to USER_MEM_LOW
        if (!curproc->p_brk) {
            curproc->p_start_brk = USER_MEM_LOW;
            curproc->p_brk = USER_MEM_LOW;
        }
        *ret = (void *)curproc->p_brk;
        return 0;
    }
    
    uintptr_t new_brk = (uintptr_t)addr;
    uintptr_t start_brk = curproc->p_start_brk;
    uintptr_t current_brk = curproc->p_brk;
    
    // If this is the first brk call, set initial values
    if (!start_brk) {
        start_brk = USER_MEM_LOW;
        curproc->p_start_brk = start_brk;
    }
    if (!current_brk) {
        current_brk = start_brk;
        curproc->p_brk = current_brk;
    }
    
    // Check if new break is valid
    if (new_brk < start_brk || new_brk > USER_MEM_HIGH) {
        return -ENOMEM;
    }
    
    // Calculate page-aligned boundaries
    uintptr_t start_page = ADDR_TO_PN(PAGE_ALIGN_UP(start_brk));
    uintptr_t current_page = ADDR_TO_PN(PAGE_ALIGN_UP(current_brk));
    uintptr_t new_page = ADDR_TO_PN(PAGE_ALIGN_UP(new_brk));
    
    // Find existing heap vmarea
    vmarea_t *heap_vma = NULL;
    vmarea_t *vma;
    list_iterate(&curproc->p_vmmap->vmm_list, vma, vmarea_t, vma_plink) {
        if (vma->vma_start == start_page) {
            heap_vma = vma;
            break;
        }
    }
    
    if (new_brk == current_brk) {
        // No change needed
        *ret = (void *)new_brk;
        return 0;
    }
    
    if (new_brk > current_brk) {
        // Expanding heap
        uintptr_t expand_start = PAGE_ALIGN_UP(current_brk);
        uintptr_t expand_end = PAGE_ALIGN_UP(new_brk);
        size_t expand_pages = ADDR_TO_PN(expand_end - expand_start);
        
        if (expand_pages > 0) {
            // Check if the expansion range is empty
            if (vmmap_is_range_empty(curproc->p_vmmap, ADDR_TO_PN(expand_start), expand_pages) != 0) {
                return -ENOMEM;
            }
            
            if (heap_vma) {
                // Extend existing heap vmarea
                heap_vma->vma_end = new_page;
            } else {
                // Create new heap vmarea
                vmarea_t *new_vma = vmarea_alloc();
                if (!new_vma) {
                    return -ENOMEM;
                }
                
                new_vma->vma_start = start_page;
                new_vma->vma_end = new_page;
                new_vma->vma_off = 0;
                new_vma->vma_prot = PROT_READ | PROT_WRITE;
                new_vma->vma_flags = MAP_PRIVATE | MAP_ANON;
                new_vma->vma_vmmap = curproc->p_vmmap;
                new_vma->vma_obj = anon_create();
                
                if (!new_vma->vma_obj) {
                    vmarea_free(new_vma);
                    return -ENOMEM;
                }
                
                vmmap_insert(curproc->p_vmmap, new_vma);
            }
        }
        // If expand_pages is 0, just update the break without creating vmarea
    } else {
        // Shrinking heap
        if (heap_vma) {
            uintptr_t shrink_start = PAGE_ALIGN_UP(new_brk);
            uintptr_t shrink_end = PAGE_ALIGN_UP(current_brk);
            size_t shrink_pages = ADDR_TO_PN(shrink_end - shrink_start);
            
            if (shrink_pages > 0) {
                // Remove the shrunk portion
                vmmap_remove(curproc->p_vmmap, ADDR_TO_PN(shrink_start), shrink_pages);
                
                // Update heap vmarea end
                heap_vma->vma_end = new_page;
            }
        }
    }
    
    // Update the process break
    curproc->p_brk = new_brk;
    *ret = (void *)new_brk;
    
    return 0;
}
