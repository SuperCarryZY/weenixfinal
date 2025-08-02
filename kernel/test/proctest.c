#include "errno.h"
#include "globals.h"

#include "test/proctest.h"
#include "test/usertest.h"

#include "util/debug.h"
#include "util/printf.h"
#include "util/string.h"

#include "proc/kthread.h"
#include "proc/proc.h"
#include "proc/sched.h"

/*
 * Set up a testing function for the process to execute. 
*/
void *test_func(long arg1, void *arg2)
{
    proc_t *proc_as_arg = (proc_t *)arg2;
    test_assert(arg1 == proc_as_arg->p_pid, "Arguments are not set up correctly");
    test_assert(proc_as_arg->p_state == PROC_RUNNING, "Process state is not running");
    test_assert(list_empty(&proc_as_arg->p_children), "There should be no child processes");
    return NULL;
}

void test_termination()
{
    int num_procs_created = 0;
    proc_t *new_proc1 = proc_create("proc test 1");
    kthread_t *new_kthread1 = kthread_create(new_proc1, test_func, 2, new_proc1);
    num_procs_created++;
    sched_make_runnable(new_kthread1);

    int count = 0;
    int status;
    while (do_waitpid(-1, &status, 0) != -ECHILD)
    {
        test_assert(status == 0, "Returned status not set correctly");
        count++;
    }
    test_assert(count == num_procs_created,
                "Expected: %d, Actual: %d number of processes have been cleaned up\n", num_procs_created, count);
}

// Custom test functions written to test the sleep, wakeup, cancellable sleep, and broadcast functionality
static ktqueue_t test_queue;
static volatile int wakeup_test_result = 0;

// Test function for sleep
void *sleep_test_func(long arg1, void *arg2)
{
    dbg(DBG_TEST, "Thread %ld going to sleep on test queue\n", arg1);
    sched_sleep_on(&test_queue);
    dbg(DBG_TEST, "Thread %ld woke up from sleep\n", arg1);
    wakeup_test_result = (int)arg1;
    return NULL;
}

// Test function for sleep and wakeup
void test_sleep_wakeup()
{
    dbg(DBG_TEST, "Testing sleep and wakeup functionality\n");
    
    sched_queue_init(&test_queue);
    test_assert(sched_queue_empty(&test_queue), "Test queue should be empty initially");
    
    proc_t *sleep_proc = proc_create("sleep_test");
    kthread_t *sleep_thread = kthread_create(sleep_proc, sleep_test_func, 42, NULL);
    sched_make_runnable(sleep_thread);
    
    sched_yield();
    
    test_assert(!sched_queue_empty(&test_queue), "Test queue should have sleeping thread");
    
    sched_wakeup_on(&test_queue, NULL);
    
    int status;
    do_waitpid(sleep_proc->p_pid, &status, 0);
    
    test_assert(wakeup_test_result == 42, "Wakeup test should return correct value");
    test_assert(sched_queue_empty(&test_queue), "Test queue should be empty after wakeup");
}

// Test function for cancellable sleep
static volatile int cancel_test_result = 0;

void *cancellable_sleep_func(long arg1, void *arg2)
{
    dbg(DBG_TEST, "Thread %ld going to cancellable sleep\n", arg1);
    long result = sched_cancellable_sleep_on(&test_queue);
    cancel_test_result = (result == -EINTR) ? 1 : 0;
    dbg(DBG_TEST, "Thread %ld cancellable sleep returned %ld\n", arg1, result);
    return NULL;
}

void test_cancellable_sleep()
{
    dbg(DBG_TEST, "Testing cancellable sleep functionality\n");
    
    sched_queue_init(&test_queue);
    
    proc_t *cancel_proc = proc_create("cancel_test");
    kthread_t *cancel_thread = kthread_create(cancel_proc, cancellable_sleep_func, 99, NULL);
    sched_make_runnable(cancel_thread);
    
    sched_yield();
    
    sched_cancel(cancel_thread);
    
    int status;
    do_waitpid(cancel_proc->p_pid, &status, 0);
    
    test_assert(cancel_test_result == 1, "Cancelled thread should return -EINTR");
}

// Test function for broadcast wakeup
static volatile int broadcast_count = 0;

void *broadcast_test_func(long arg1, void *arg2)
{
    dbg(DBG_TEST, "Thread %ld going to sleep for broadcast test\n", arg1);
    sched_sleep_on(&test_queue);
    broadcast_count++;
    dbg(DBG_TEST, "Thread %ld woke up from broadcast\n", arg1);
    return NULL;
}

void test_broadcast()
{
    dbg(DBG_TEST, "Testing broadcast wakeup functionality\n");
    
    sched_queue_init(&test_queue);
    broadcast_count = 0;

    const int num_threads = 3;
    proc_t *procs[num_threads];
    kthread_t *threads[num_threads];
    
    for (int i = 0; i < num_threads; i++) {
        char name[32];
        snprintf(name, sizeof(name), "broadcast_test_%d", i);
        procs[i] = proc_create(name);
        threads[i] = kthread_create(procs[i], broadcast_test_func, i, NULL);
        sched_make_runnable(threads[i]);
    }
    
    sched_yield();
    
    sched_broadcast_on(&test_queue);
    
    for (int i = 0; i < num_threads; i++) {
        int status;
        do_waitpid(procs[i]->p_pid, &status, 0);
    }
    
    test_assert(broadcast_count == num_threads, 
                "All threads should wake up from broadcast (expected %d, got %d)", 
                num_threads, broadcast_count);
    test_assert(sched_queue_empty(&test_queue), "Test queue should be empty after broadcast");
}

// Test function for multiple process creation
void test_multiple_processes()
{
    dbg(DBG_TEST, "Testing multiple process creation and cleanup\n");
    
    const int num_procs = 5;
    for (int i = 0; i < num_procs; i++) {
        char name[32];
        snprintf(name, sizeof(name), "multi_test_%d", i);
        proc_t *proc = proc_create(name);
        test_assert(proc != NULL, "Process creation should succeed");
        test_assert(proc->p_pid > 0, "Process should have valid PID");
        
        kthread_t *thread = kthread_create(proc, test_func, proc->p_pid, proc);
        test_assert(thread != NULL, "Thread creation should succeed");
        sched_make_runnable(thread);
    }
    
    int count = 0;
    int status;
    while (do_waitpid(-1, &status, 0) != -ECHILD) {
        count++;
    }
    
    test_assert(count == num_procs, 
                "All processes should complete (expected %d, got %d)", 
                num_procs, count);
}

long proctest_main(long arg1, void *arg2)
{
    dbg(DBG_TEST, "\n=== Starting Process and Scheduler Tests ===\n");
    test_init();
    
    // Run existing test
    test_termination();
    
    // Run new comprehensive tests
    test_sleep_wakeup();
    test_cancellable_sleep();
    test_broadcast();
    test_multiple_processes();
    
    dbg(DBG_TEST, "=== Process and Scheduler Tests Complete ===\n");
    test_fini();
    return 0;
}