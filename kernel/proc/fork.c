#include "errno.h"
#include "globals.h"
#include "types.h"

#include "util/debug.h"
#include "util/string.h"

#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/pframe.h"
#include "mm/tlb.h"

#include "fs/vnode.h"

#include "vm/shadow.h"

#include "api/exec.h"

/* Pushes the appropriate things onto the kernel stack of a newly forked thread
 * so that it can begin execution in userland_entry.
 * regs: registers the new thread should have on execution
 * kstack: location of the new thread's kernel stack
 * Returns the new stack pointer on success. */
static uintptr_t fork_setup_stack(const regs_t *regs, void *kstack)
{
    /* Pointer argument and dummy return address, and userland dummy return
     * address */
    uint64_t rsp =
        ((uint64_t)kstack) + DEFAULT_STACK_SIZE - (sizeof(regs_t) + 16);
    memcpy((void *)(rsp + 8), regs, sizeof(regs_t)); /* Copy over struct */
    return rsp;
}

/*
 * This function implements the fork(2) system call.
 *
 * TODO:
 * 1) Use proc_create() and kthread_clone() to set up a new process and thread. If
 *    either fails, perform any appropriate cleanup.
 * 2) Finish any initialization work for the new process and thread.
 * 3) Fix the values of the registers and the rest of the kthread's ctx. 
 *    Some registers can be accessed from the cloned kthread's context (see the context_t 
 *    and kthread_t structs for more details):
 *    a) We want the child process to also enter userland execution. 
 *       For this, the instruction pointer should point to userland_entry (see exec.c).
 *    b) Remember that the only difference between the parent and child processes 
 *       is the return value of fork(). This value is returned in the RAX register, 
 *       and the return value should be 0 for the child. The parent's return value would 
 *       be the process id of the newly created child process. 
 *    c) Before the process begins execution in userland_entry, 
 *       we need to push all registers onto the kernel stack of the kthread. 
 *       Use fork_setup_stack to do this, and set RSP accordingly. 
 *    d) Use pt_unmap_range and tlb_flush_all on the parent in advance of
 *       copy-on-write.
 * 5) Prepare the child process to be run on the CPU.
 * 6) Return the child's process id to the parent.
 */
long do_fork(struct regs *regs)
{
    // Create a new process
    proc_t *child_proc = proc_create("forked");
    if (!child_proc) {
        return -ENOMEM;
    }
    
    // Clone the current thread
    kthread_t *child_thread = kthread_clone(curthr);
    if (!child_thread) {
        proc_destroy(child_proc);
        return -ENOMEM;
    }
    
    // Set up the child process - PID is already set by proc_create
    child_thread->kt_proc = child_proc;
    list_insert_tail(&child_proc->p_threads, &child_thread->kt_plink);
    
    // Clone the virtual memory map
    vmmap_t *child_vmmap = vmmap_clone(curproc->p_vmmap);
    if (!child_vmmap) {
        kthread_destroy(child_thread);
        proc_destroy(child_proc);
        return -ENOMEM;
    }
    child_proc->p_vmmap = child_vmmap;
    
    // Set up copy-on-write for shared memory objects
    vmarea_t *vma;
    list_iterate(&child_vmmap->vmm_list, vma, vmarea_t, vma_plink) {
        if (!(vma->vma_flags & MAP_SHARED)) {
            // Create shadow objects for private mappings
            mobj_t *shadow_obj = shadow_create(vma->vma_obj);
            if (!shadow_obj) {
                vmmap_destroy(&child_vmmap);
                kthread_destroy(child_thread);
                proc_destroy(child_proc);
                return -ENOMEM;
            }
            
            // Replace the original object with shadow object
            mobj_ref(shadow_obj);
            vma->vma_obj = shadow_obj;
        }
    }
    
    // Set up the child's registers
    regs_t child_regs = *regs;
    child_regs.r_rax = 0;  // Child returns 0
    
    // Set up the child's stack
    uintptr_t child_rsp = fork_setup_stack(&child_regs, child_thread->kt_kstack);
    child_thread->kt_ctx.c_rsp = child_rsp;
    child_thread->kt_ctx.c_rip = (uintptr_t)userland_entry;
    
    // Unmap the parent's pages to trigger copy-on-write
    vmarea_t *parent_vma;
    list_iterate(&curproc->p_vmmap->vmm_list, parent_vma, vmarea_t, vma_plink) {
        if (!(parent_vma->vma_flags & MAP_SHARED)) {
            pt_unmap_range(curproc->p_pml4, 
                          (uintptr_t)PN_TO_ADDR(parent_vma->vma_start),
                          (uintptr_t)PN_TO_ADDR(parent_vma->vma_end - parent_vma->vma_start));
        }
    }
    tlb_flush_all();
    
    // Add the child thread to the scheduler
    sched_make_runnable(child_thread);
    
    return child_proc->p_pid;
}
