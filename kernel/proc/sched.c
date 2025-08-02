#include "api/syscall.h"
#include "errno.h"
#include "fs/vfs.h"
#include "globals.h"
#include "main/apic.h"
#include "main/inits.h"
#include "types.h"
#include "util/debug.h"
#include <util/time.h>

/*==========
 * Variables
 *=========*/

/*
 * The run queue of threads waiting to be run.
 */
static ktqueue_t kt_runq CORE_SPECIFIC_DATA;

/*
 * Helper tracking most recent thread context before a context_switch().
 */
static context_t *last_thread_context CORE_SPECIFIC_DATA;

/*===================
 * Preemption helpers
 *==================*/

inline void preemption_disable()
{
    if (curthr)
        curthr->kt_preemption_count++;
}

inline void preemption_enable()
{
    if (curthr)
    {
        KASSERT(curthr->kt_preemption_count);
        curthr->kt_preemption_count--;
    }
}

inline void preemption_reset()
{
    KASSERT(curthr);
    curthr->kt_preemption_count = 0;
}

inline long preemption_enabled()
{
    return curthr && !curthr->kt_preemption_count;
}

/*==================
 * ktqueue functions
 *=================*/

/*
 * Initializes queue.
 */
void sched_queue_init(ktqueue_t *queue)
{
    list_init(&queue->tq_list);
    queue->tq_size = 0;
}

/*
 * Adds thr to the tail of queue.
 *
 * queue must be locked
 */
static void ktqueue_enqueue(ktqueue_t *queue, kthread_t *thr)
{
    KASSERT(!thr->kt_wchan);

    list_assert_sanity(&queue->tq_list);
    /* Because of the way core-specific data is handled, we add to the front
     *  of the queue (and remove from the back). */
    list_insert_head(&queue->tq_list, &thr->kt_qlink);
    list_assert_sanity(&queue->tq_list);

    thr->kt_wchan = queue;
    queue->tq_size++;
}

/*
 * Removes and returns a thread from the head of queue.
 *
 * queue must be locked
 */
static kthread_t *ktqueue_dequeue(ktqueue_t *queue)
{
    if (sched_queue_empty(queue))
    {
        return NULL;
    }

    list_assert_sanity(&queue->tq_list);

    list_link_t *link = queue->tq_list.l_prev;
    kthread_t *thr = list_item(link, kthread_t, kt_qlink);
    list_remove(link);
    thr->kt_wchan = NULL;

    list_assert_sanity(&queue->tq_list);

    queue->tq_size--;
    return thr;
}

/*
 * Removes thr from queue
 *
 * queue must be locked
 */
static void ktqueue_remove(ktqueue_t *queue, kthread_t *thr)
{
    KASSERT(thr->kt_qlink.l_next && thr->kt_qlink.l_prev);
    list_remove(&thr->kt_qlink);
    thr->kt_wchan = NULL;
    queue->tq_size--;
    list_assert_sanity(&queue->tq_list);
}

/*
 * Returns 1 if queue is empty, 0 if's not
 *
 * If using this for branching / conditional logic on the queue, it should be
 * locked for this call to avoid a TOCTTOU bug. This is, however, up to the
 * callee and not enforced at this level.
 */
inline long sched_queue_empty(ktqueue_t *queue) { return queue->tq_size == 0; }

/*==========
 * Functions
 *=========*/

/*
 * Initializes the run queue.
 */
void sched_init(void)
{
    sched_queue_init(GET_CSD(curcore.kc_id, ktqueue_t, kt_runq));
}

/*
 * Puts curthr into the cancellable sleep state, and calls sched_switch() with 
 * the passed in arguments. Cancellable sleep means that the thread can be woken 
 * up from sleep for two reasons:
 *      1. The event it is waiting for has occurred.
 *      2. It was cancelled.
 *
 * Returns 0, or:
 *  - EINTR: If curthr is cancelled before or after the call to sched_switch()
 * 
 * Hints:
 * Do not enqueue the thread directly, let sched_switch handle this.
 */
long sched_cancellable_sleep_on(ktqueue_t *queue)
{
    // Check if the thread was cancelled before sleeping
    if (curthr->kt_cancelled)
    {
        return -EINTR;
    }

    // Set the thread state to cancellable sleep
    curthr->kt_state = KT_SLEEP_CANCELLABLE;
    
    // Handle the switch to the new queue
    sched_switch(queue);

    // Check if the thread was cancelled after sleeping
    return curthr->kt_cancelled ? -EINTR : 0;
}

/*
 * If the given thread is in a cancellable sleep, removes it from whatever queue 
 * it is sleeping on and makes the thread runnable again.
 *
 * Regardless of the thread's state, this should mark the thread as cancelled.
 */
void sched_cancel(kthread_t *thr)
{
    // Mark the thread as cancelled
    thr->kt_cancelled = 1;

    // Remove the thread from the queue and make it runnable if it is in cancellable sleep
    if (thr->kt_state == KT_SLEEP_CANCELLABLE)
    {
        KASSERT(thr->kt_wchan);
        ktqueue_remove(thr->kt_wchan, thr);
        sched_make_runnable(thr);
    }
}

/*
 * Switches into the context of the current core, which is constantly in a loop 
 * in core_switch() to choose a new runnable thread and switch into its thread
 * context.
 * 
 * We want to switch to the current core because the idle process handles the
 * actual switching of the threads. Please see section 3.3 Boot Sequence to 
 * find a more in depth explantion about the idle process and its
 * relationship with core_switch().
 *
 * Hints:
 * curthr state must NOT be KT_ON_CPU upon entry.
 * To ensure that curthr is enqueued on queue only once it is no longer executing,
 * set the kc_queue field of curcore (the current core) to the queue. See 
 * core_switch() to see how the queue is handled.
 * 
 * Protect the context switch from interrupts: Use intr_disable(), intr_setipl(), 
 * intr_enable(), and IPL_LOW. 
 * 
 * Even though we want to disable interrupts while modifying the run queue, 
 * core_switch() will actually enable interrupts before sleeping, 
 * but it doesn't modify the IPL. Because we want an interrupt of any level 
 * to wake up the idling core, IPL should be set to IPL_LOW.
 * 
 * After calling context_switch, make sure to set the IPL back to the original
 * IPL.
 * 
 * Do not directly call core_switch. The curcore's thread is stuck in a loop
 * inside core_switch, so switching to its context brings you there.
 * 
 * For debugging purposes, you may find it useful to set
 * last_thread_context to the context of the current thread here before the call
 * to context_switch.
 */
void sched_switch(ktqueue_t *queue)
{
    KASSERT(curthr->kt_state != KT_ON_CPU);
    
    // Disable interrupts and save the original IPL
    intr_disable();
    uint8_t old_ipl = intr_setipl(IPL_LOW);
    
    // Set the current core's queue to the new queue
    curcore.kc_queue = queue;
    
    // Save the current thread's context
    last_thread_context = &curthr->kt_ctx;
    
    // Switch to the core's context
    context_switch(&curthr->kt_ctx, &curcore.kc_ctx);

    KASSERT(curthr);
    
    // Restore the original IPL and enable interrupts
    intr_setipl(old_ipl);
    intr_enable();
}

/*
 * Set the state of the current thread to runnable and sched_switch() with the
 * current core's runq.
 */
void sched_yield()
{
    KASSERT(curthr->kt_state == KT_ON_CPU);
    curthr->kt_state = KT_RUNNABLE;
    sched_switch(&kt_runq);
}

/*
 * Makes the given thread runnable by setting its state and enqueuing it in the 
 * run queue (kt_runq).
 *
 * Hints:
 * Cannot be called on curthr (it is already running).
 * Because this can be called from an interrupt context, temporarily mask
 * interrupts. Use intr_setipl() and IPL_HIGH in order to avoid being interrupted
 * while modifying the queue.
 */
void sched_make_runnable(kthread_t *thr)
{
    KASSERT(thr != curthr);
    
    // Mask interrupts to protect queue operations
    uint8_t old_ipl = intr_setipl(IPL_HIGH);

    thr->kt_state = KT_RUNNABLE;
    
    ktqueue_enqueue(&kt_runq, thr);
    intr_setipl(old_ipl);
}

/*
 * Places curthr in an uninterruptible sleep on q. I.e. if the thread is cancelled
 * while sleeping, it will NOT notice until it is woken up by the event it's 
 * waiting for.
 *
 * Hints:
 * Temporarily mask interrupts using intr_setipl() and IPL_HIGH.
 * IPL should be set to IPL_HIGH because the act of changing the thread's state
 * and enqueuing the thread on the queue should not be interrupted 
 * (as sched_wakeup_on) could be called from an interrupt context. 
 * 
 * Do not enqueue the thread directly, let sched_switch handle this (pass q to sched_switch()).
 */
void sched_sleep_on(ktqueue_t *q)
{
    // Set the thread state to uninterruptible sleep
    uint8_t old_ipl = intr_setipl(IPL_HIGH);
    curthr->kt_state = KT_SLEEP;
    intr_setipl(old_ipl);
    sched_switch(q);
}

/*
 * Wakes up a thread on the given queue by taking it off the queue and 
 * making it runnable. If given an empty queue, do nothing.
 *
 * Hints:
 * Make sure to set *ktp (if it is provided--i.e. ktp is not NULL) to the 
 * dequeued thread before making it runnable. This allows the caller to get a 
 * handle to the thread that was woken up (useful, for instance, when 
 * implementing unlock() on a mutex: the mutex can wake up a sleeping thread
 * and make it the new owner).
 */
void sched_wakeup_on(ktqueue_t *q, kthread_t **ktp)
{
    // Dequeue a thread from the queue
    kthread_t *woken_thread = ktqueue_dequeue(q);
    
    // If queue was empty, do nothing
    if (!woken_thread) {
        if (ktp) {
            *ktp = NULL;
        }
        return;
    }
    
    // Set the output parameter if provided and make the thread runnable
    if (ktp) {
        *ktp = woken_thread;
    }
    sched_make_runnable(woken_thread);
}

/*
 * Wake up all the threads on the given queue by making them all runnable.
 */
void sched_broadcast_on(ktqueue_t *q)
{
    kthread_t *woken_thread;
    
    // Wake up all threads in the queue and make them runnable
    while ((woken_thread = ktqueue_dequeue(q)) != NULL) {
        sched_make_runnable(woken_thread);
    }
}

/*
 * The meat of our scheduler.
 *
 * You will want to (in this exact order):
 *  1) perform the operations on curcore.kc_queue and curcore.kc_lock
 *  2) set curproc to idleproc, and curthr to NULL
 *  3) try to get the next thread to run by dequeuing from the runqueue.
 * If there is no next thread, then the core is idle, so wait for an interrupt using
 * intr_wait(). Note that you will need to re-disable interrupts after returning
 * from intr_wait(). 4) ensure the context's PML4 for the selected thread is
 * correctly setup with curcore's core-specific data. Use kt_recent_core and
 * map_in_core_specific_data. 5) set curthr and curproc 6) context_switch out
 */
void core_switch()
{
    while (1)
    {
        KASSERT(!intr_enabled());
        KASSERT(!curthr || curthr->kt_state != KT_ON_CPU);

        if (curcore.kc_queue)
        {
            ktqueue_enqueue(curcore.kc_queue, curthr);
        }

        curproc = &idleproc;
        curthr = NULL;

        kthread_t *next_thread = NULL;
        while (1)
        {
            next_thread = ktqueue_dequeue(&kt_runq);

            if (next_thread)
                break;

            intr_wait();
            intr_disable();
        }

        KASSERT(next_thread->kt_state == KT_RUNNABLE);
        KASSERT(next_thread->kt_proc);

        // if (curcore.kc_id != next_thread->kt_recent_core)
        // {
        map_in_core_specific_data(next_thread->kt_ctx.c_pml4);
            // next_thread->kt_recent_core = curcore.kc_id;
        // }

        uintptr_t mapped_paddr = pt_virt_to_phys_helper(
            next_thread->kt_ctx.c_pml4, (uintptr_t)&next_thread);
        uintptr_t expected_paddr =
            pt_virt_to_phys_helper(pt_get(), (uintptr_t)&next_thread);
        KASSERT(mapped_paddr == expected_paddr);

        curthr = next_thread;
        curthr->kt_state = KT_ON_CPU;
        curproc = curthr->kt_proc;
        context_switch(&curcore.kc_ctx, &curthr->kt_ctx);
    }
}
