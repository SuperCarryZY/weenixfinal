#include "vm/pagefault.h"
#include "errno.h"
#include "globals.h"
#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/mobj.h"
#include "mm/pframe.h"
#include "mm/tlb.h"
#include "types.h"
#include "util/debug.h"

/*
 * Respond to a user mode pagefault by setting up the desired page.
 *
 *  vaddr - The virtual address that the user pagefaulted on
 *  cause - A combination of FAULT_ flags indicating the type of operation that
 *  caused the fault (see pagefault.h)
 *
 * Implementation details:
 *  1) Find the vmarea that contains vaddr, if it exists.
 *  2) Check the vmarea's protections (see the vmarea_t struct) against the 'cause' of
 *     the pagefault. For example, error out if the fault has cause write and we don't
 *     have write permission in the area. Keep in mind:
 *     a) You can assume that FAULT_USER is always specified.
 *     b) If neither FAULT_WRITE nor FAULT_EXEC is specified, you may assume the
 *     fault was due to an attempted read.
 *  3) Obtain the corresponding pframe from the vmarea's mobj. Be careful about
 *     locking and error checking!
 *  4) Finally, set up a call to pt_map to insert a new mapping into the
 *     appropriate pagetable:
 *     a) Use pt_virt_to_phys() to obtain the physical address of the actual
 *        data.
 *     b) You should not assume that vaddr is page-aligned, but you should
 *        provide a page-aligned address to the mapping.
 *     c) For pdflags, use PT_PRESENT | PT_WRITE | PT_USER.
 *     d) For ptflags, start with PT_PRESENT | PT_USER. Also supply PT_WRITE if
 *        the user can and wants to write to the page.
 *  5) Flush the TLB.
 *
 * Tips:
 * 1) This gets called by _pt_fault_handler() in mm/pagetable.c, which
 *    importantly checks that the fault did not occur in kernel mode. Think
 *    about why a kernel mode page fault would be bad in Weenix. Explore
 *    _pt_fault_handler() to get a sense of what's going on.
 * 2) If you run into any errors, you should segfault by calling
 *    do_exit(EFAULT).
 */
void handle_pagefault(uintptr_t vaddr, uintptr_t cause)
{
    dbg(DBG_VM, "vaddr = 0x%p (0x%p), cause = %lu\n", (void *)vaddr,
        PAGE_ALIGN_DOWN(vaddr), cause);
    
    KASSERT(curproc && curproc->p_vmmap);
    
    // Convert virtual address to page number
    size_t vfn = ADDR_TO_PN(vaddr);
    
    // Find the vmarea that contains this virtual address
    vmarea_t *vma = vmmap_lookup(curproc->p_vmmap, vfn);
    if (!vma) {
        // No vmarea found for this address
        do_exit(EFAULT);
        return;
    }
    
    // Check permissions
    int required_prot = 0;
    if (cause & FAULT_WRITE) {
        required_prot |= PROT_WRITE;
    } else if (cause & FAULT_EXEC) {
        required_prot |= PROT_EXEC;
    } else {
        // Assume read access
        required_prot |= PROT_READ;
    }
    
    // Check if the vmarea allows the required access
    if ((vma->vma_prot & required_prot) != required_prot) {
        do_exit(EFAULT);
        return;
    }
    
    // Calculate the offset into the memory object
    size_t obj_offset = vfn - vma->vma_start + vma->vma_off;
    
    // Determine if this is a write operation
    long forwrite = (cause & FAULT_WRITE) ? 1 : 0;
    
    // Get the pframe from the memory object
    pframe_t *pf = mobj_get_pframe(vma->vma_obj, obj_offset, forwrite, 0);
    if (!pf) {
        do_exit(EFAULT);
        return;
    }
    
    // Get the physical address
    uintptr_t paddr = pt_virt_to_phys((uintptr_t)pf->pf_addr);
    
    // Set up page table flags
    int pdflags = PT_PRESENT | PT_WRITE | PT_USER;
    int ptflags = PT_PRESENT | PT_USER;
    
    // Add write permission if the vmarea allows it and this is a write fault
    if ((vma->vma_prot & PROT_WRITE) && (cause & FAULT_WRITE)) {
        ptflags |= PT_WRITE;
    }
    
    // Map the page
    pt_map(curproc->p_pml4, paddr, (uintptr_t)PAGE_ALIGN_DOWN(vaddr), pdflags, ptflags);
    
    // Flush the TLB
    tlb_flush_range((uintptr_t)PAGE_ALIGN_DOWN(vaddr), PAGE_SIZE);
}
