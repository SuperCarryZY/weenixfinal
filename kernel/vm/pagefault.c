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
    NOT_YET_IMPLEMENTED("VM: handle_pagefault");
}
