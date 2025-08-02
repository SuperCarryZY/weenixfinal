#include "config.h"
#include "globals.h"
#include "mm/slab.h"
#include "util/debug.h"
#include "util/string.h"

/*==========
 * Variables
 *=========*/

/*
 * Global variable maintaining the current thread on the cpu
 */
kthread_t *curthr CORE_SPECIFIC_DATA;

/*
 * Private slab for kthread structs
 */
static slab_allocator_t *kthread_allocator = NULL;

/*=================
 * Helper functions
 *================*/

/*
 * Allocates a new kernel stack. Returns null when not enough memory.
 */
static char *alloc_stack() { return page_alloc_n(DEFAULT_STACK_SIZE_PAGES); }

/*
 * Frees an existing kernel stack.
 */
static void free_stack(char *stack)
{
    page_free_n(stack, DEFAULT_STACK_SIZE_PAGES);
}

/*==========
 * Functions
 *=========*/

/*
 * Initializes the kthread_allocator.
 */
void kthread_init()
{
    KASSERT(__builtin_popcount(DEFAULT_STACK_SIZE_PAGES) == 1 &&
            "stack size should be a power of 2 pages to reduce fragmentation");
    kthread_allocator = slab_allocator_create("kthread", sizeof(kthread_t));
    KASSERT(kthread_allocator);
}

/*
 * Creates and initializes a thread.
 * Returns a new kthread, or NULL on failure.
 *
 * Hints:
 * Use kthread_allocator to allocate a kthread
 * Use alloc_stack() to allocate a kernel stack
 * Use context_setup() to set up the thread's context - 
 *  also use DEFAULT_STACK_SIZE and the process's pagetable (p_pml4)
 * Remember to initialize all the thread's fields
 * Remember to add the thread to proc's threads list
 * Initialize the thread's kt_state to KT_NO_STATE
 * Initialize the thread's kt_recent_core to ~0UL (unsigned -1)
 */
kthread_t *kthread_create(proc_t *proc, kthread_func_t func, long arg1, void *arg2)
{
    kthread_t *thr = slab_obj_alloc(kthread_allocator);
    if (!thr) {
        return NULL;
    }

    char *stack = alloc_stack();
    if (!stack) {
        slab_obj_free(kthread_allocator, thr);
        return NULL;
    }

    context_setup(&thr->kt_ctx, func, arg1, arg2, stack, DEFAULT_STACK_SIZE, proc->p_pml4);

    thr->kt_kstack = stack;
    thr->kt_retval = NULL;
    thr->kt_errno = 0;
    thr->kt_proc = proc;
    thr->kt_cancelled = 0;
    thr->kt_wchan = NULL;
    thr->kt_state = KT_NO_STATE;
    thr->kt_preemption_count = 0;

    list_link_init(&thr->kt_plink);
    list_link_init(&thr->kt_qlink);
    list_init(&thr->kt_mutexes);
    
    list_insert_tail(&proc->p_threads, &thr->kt_plink);

    return thr;
}

/*
 * Creates and initializes a thread that is a clone of thr.
 * Returns a new kthread, or null on failure.
 * 
 * P.S. Note that you do not need to implement this function until VM.
 *
 * Hints:
 * The only parts of the context that must be initialized are c_kstack and
 * c_kstacksz. The thread's process should be set outside of this function. Copy
 * over thr's retval, errno, and cancelled; other fields should be freshly
 * initialized. See kthread_create() for more hints.
 */
kthread_t *kthread_clone(kthread_t *thr)
{
    NOT_YET_IMPLEMENTED("VM: kthread_clone");
    return NULL;
}

/*
 * Free the thread's stack, remove it from its process's list of threads, and
 * free the kthread_t struct itself. Protect access to the kthread using its
 * kt_lock.
 *
 * You cannot destroy curthr.
 */
void kthread_destroy(kthread_t *thr)
{
    KASSERT(thr != curthr);
    KASSERT(thr && thr->kt_kstack);
    if (thr->kt_state != KT_EXITED)
        panic("destroying thread in state %d\n", thr->kt_state);
    free_stack(thr->kt_kstack);
    if (list_link_is_linked(&thr->kt_plink))
        list_remove(&thr->kt_plink);

    slab_obj_free(kthread_allocator, thr);
}

/*
 * Sets the thread's return value and cancels the thread.
 *
 * Note: Check out the use of check_curthr_cancelled() in syscall_handler()
 * to see how a thread eventually notices it is cancelled and handles exiting
 * itself.
 *
 * Hints:
 * This should not be called on curthr.
 * Use sched_cancel() to actually mark the thread as cancelled. This way you
 * can take care of all cancellation cases. 
 */
void kthread_cancel(kthread_t *thr, void *retval)
{
    KASSERT(thr != curthr);
    thr->kt_retval = retval;
    sched_cancel(thr);
}

/*
 * Wrapper around proc_thread_exiting().
 */
void kthread_exit(void *retval)
{
    proc_thread_exiting(retval);
}
