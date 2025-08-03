#include "errno.h"
#include "globals.h"
#include "types.h"
#include <api/exec.h>
#include <drivers/screen.h>
#include <drivers/tty/tty.h>
#include <drivers/tty/vterminal.h>
#include <main/io.h>
#include <mm/mm.h>
#include <mm/slab.h>
#include <test/kshell/kshell.h>
#include <test/proctest.h>
#include <util/time.h>
#include <vm/anon.h>
#include <vm/shadow.h>

#include "util/debug.h"
#include "util/gdb.h"
#include "util/printf.h"
#include "util/string.h"

#include "main/acpi.h"
#include "main/apic.h"
#include "main/inits.h"

#include "drivers/dev.h"
#include "drivers/pcie.h"

#include "api/syscall.h"

#include "fs/fcntl.h"
#include "fs/vfs.h"
#include "fs/vfs_syscall.h"
#include "fs/vnode.h"

#include "test/driverstest.h"

#include "util/btree.h"

// Forward declaration for vmtest
long vmtest_main(long arg1, void* arg2);

GDB_DEFINE_HOOK(boot)

GDB_DEFINE_HOOK(initialized)

GDB_DEFINE_HOOK(shutdown)

static void initproc_start();

typedef void (*init_func_t)();
static init_func_t init_funcs[] = {
    dbg_init,
    intr_init,
    page_init,
    pt_init,
    acpi_init,
    apic_init,
    core_init,
    slab_init,
    pframe_init,
    pci_init,
    vga_init,
#ifdef __VM__
    anon_init,
    shadow_init,
#endif
    vmmap_init,
    proc_init,
    kthread_init,
#ifdef __DRIVERS__
    chardev_init,
    blockdev_init,
#endif
    kshell_init,
    file_init,
    pipe_init,
    syscall_init,
    elf64_init,

    proc_idleproc_init,
    btree_init,
};

/*
 * Call the init functions (in order!), then run the init process
 * (initproc_start)
 */
void kmain()
{
    GDB_CALL_HOOK(boot);

    for (size_t i = 0; i < sizeof(init_funcs) / sizeof(init_funcs[0]); i++)
        init_funcs[i]();

    initproc_start();
    panic("\nReturned to kmain()\n");
}

/*
 * Make:
 * 1) /dev/null
 * 2) /dev/zero
 * 3) /dev/ttyX for 0 <= X < __NTERMS__
 * 4) /dev/hdaX for 0 <= X < __NDISKS__
 */
static void make_devices()
{
    long status = do_mkdir("/dev");
    KASSERT(!status || status == -EEXIST);

    status = do_mknod("/dev/null", S_IFCHR, MEM_NULL_DEVID);
    KASSERT(!status || status == -EEXIST);
    status = do_mknod("/dev/zero", S_IFCHR, MEM_ZERO_DEVID);
    KASSERT(!status || status == -EEXIST);

    char path[32] = {0};
    for (long i = 0; i < __NTERMS__; i++)
    {
        snprintf(path, sizeof(path), "/dev/tty%ld", i);
        dbg(DBG_INIT, "Creating tty mknod with path %s\n", path);
        status = do_mknod(path, S_IFCHR, MKDEVID(TTY_MAJOR, i));
        KASSERT(!status || status == -EEXIST);
    }

    for (long i = 0; i < __NDISKS__; i++)
    {
        snprintf(path, sizeof(path), "/dev/hda%ld", i);
        dbg(DBG_INIT, "Creating disk mknod with path %s\n", path);
        status = do_mknod(path, S_IFBLK, MKDEVID(DISK_MAJOR, i));
        KASSERT(!status || status == -EEXIST);
    }
}

/*
 * The function executed by the init process. Finish up all initialization now 
 * that we have a proper thread context.
 * 
 * This function will require edits over the course of the project:
 *
 * - Before finishing drivers, this is where your tests lie. You can, however, 
 *  have them in a separate test function which can even be in a separate file 
 *  (see handout).
 * 
 * - After finishing drivers but before starting VM, you should start __NTERMS__
 *  processes running kshells (see kernel/test/kshell/kshell.c, specifically
 *  kshell_proc_run). Testing here amounts to defining a new kshell command 
 *  that runs your tests. 
 * 
 * - During and after VM, you should use kernel_execve when starting, you
 *  will probably want to kernel_execve the program you wish to test directly.
 *  Eventually, you will want to kernel_execve "/sbin/init" and run your
 *  tests from the userland shell (by typing in test commands)
 * 
 * Note: The init process should wait on all of its children to finish before 
 * returning from this function (at which point the system will shut down).
 */
static void *initproc_run(long arg1, void *arg2)
{
#ifdef __VFS__
    dbg(DBG_INIT, "Initializing VFS...\n");
    vfs_init();
    make_devices();
#endif

    // Run VM tests first
    dbg(DBG_INIT, "Running VM tests...\n");
    long vmtest_result = vmtest_main(0, NULL);
    
    if (vmtest_result == 0) {
        dbg(DBG_INIT, "VM tests PASSED!\n");
    } else {
        dbg(DBG_INIT, "VM tests FAILED with result: %ld\n", vmtest_result);
    }
    
    // Run the process/scheduler tests
    dbg(DBG_INIT, "Init process started successfully!\n");
    dbg(DBG_INIT, "Running process and scheduler tests...\n");
    
    // Call the main test function from proctest.c
    long test_result = proctest_main(0, NULL);
    
    if (test_result == 0) {
        dbg(DBG_INIT, "All tests PASSED!\n");
    } else {
        dbg(DBG_INIT, "Some tests FAILED with result: %ld\n", test_result);
    }
    

    char *argv[] = {"/sbin/init", NULL};
    char *envp[] = {NULL};
    
    kernel_execve("/sbin/init", argv, envp);
    
    dbg(DBG_PRINT, "kernel_execve returned, falling back to kshell...\n");
    
    char *hello_argv[] = {"/usr/bin/hello", NULL};
    kernel_execve("/usr/bin/hello", hello_argv, envp);
    
    char *segfault_argv[] = {"/usr/bin/segfault", NULL};
    kernel_execve("/usr/bin/segfault", segfault_argv, envp);
    
    char *memtest_argv[] = {"/usr/bin/memtest", NULL};
    kernel_execve("/usr/bin/memtest", memtest_argv, envp);
    
    char *args_argv[] = {"/usr/bin/args", "test", "arguments", "here", NULL};
    kernel_execve("/usr/bin/args", args_argv, envp);
    
    char *forktest_argv[] = {"/usr/bin/forktest", NULL};
    kernel_execve("/usr/bin/forktest", forktest_argv, envp);
    
    char *uname_argv[] = {"/bin/uname", NULL};
    kernel_execve("/bin/uname", uname_argv, envp);
    
    char *stat_argv[] = {"/bin/stat", "/etc/passwd", NULL};
    kernel_execve("/bin/stat", stat_argv, envp);
    
    char *kshell_argv[] = {"/usr/bin/kshell", NULL};
    kernel_execve("/usr/bin/kshell", kshell_argv, envp);
    
    char *ls_argv[] = {"/bin/ls", "/", NULL};
    kernel_execve("/bin/ls", ls_argv, envp);
    
    char *wc_argv[] = {"/usr/bin/wc", "/etc/passwd", NULL};
    kernel_execve("/usr/bin/wc", wc_argv, envp);
    
    char *hd_argv[] = {"/bin/hd", "/etc/passwd", NULL};
    kernel_execve("/bin/hd", hd_argv, envp);
    
    char *sh_argv[] = {"/bin/sh", NULL};
    kernel_execve("/bin/sh", sh_argv, envp);
    
    char *vfstest_argv[] = {"/usr/bin/vfstest", NULL};
    kernel_execve("/usr/bin/vfstest", vfstest_argv, envp);
    
    char *eatinodes_argv[] = {"/usr/bin/eatinodes", NULL};
    kernel_execve("/usr/bin/eatinodes", eatinodes_argv, envp);
    
    char *eatmem_argv[] = {"/usr/bin/eatmem", NULL};
    kernel_execve("/usr/bin/eatmem", eatmem_argv, envp);
    
    char *ed_argv[] = {"/bin/ed", NULL};
    kernel_execve("/bin/ed", ed_argv, envp);
    
    /* To create a kshell on each terminal */
#ifdef __DRIVERS__
    char name[32] = {0};
    for (long i = 0; i < __NTERMS__; i++)
    {
        snprintf(name, sizeof(name), "kshell%ld", i);
        proc_t *proc = proc_create("ksh");
        kthread_t *thread = kthread_create(proc, kshell_proc_run, i, NULL);
        sched_make_runnable(thread);
    }
#endif

    // Wait for all children to finish before exiting
    int status;
    while (do_waitpid(-1, &status, 0) != -ECHILD) {
        dbg(DBG_INIT, "Init process: child exited with status %d\n", status);
    }
    
    dbg(DBG_INIT, "Init process: all children have exited, shutting down\n");
    return NULL;
}

/*
 * Sets up the initial process and prepares it to run.
 *
 * Hints:
 * Use proc_create() to create the initial process.
 * Use kthread_create() to create the initial process's only thread.
 * Make sure the thread is set up to start running initproc_run() (values for
 *  arg1 and arg2 do not matter, they can be 0 and NULL).
 * Use sched_make_runnable() to make the thread runnable.
 * Use context_make_active() with the context of the current core (curcore) 
 * to start the scheduler.
 */
void initproc_start()
{
    proc_t *init_proc = proc_create("init");
    KASSERT(init_proc && "Failed to create init process");
    

    kthread_t *init_thread = kthread_create(init_proc, initproc_run, 0, NULL);
    KASSERT(init_thread && "Failed to create init thread");
    
    sched_make_runnable(init_thread);
    context_make_active(&curcore.kc_ctx);
    
    panic("initproc_start: returned from context_make_active");
}

void initproc_finish()
{
#ifdef __VFS__
    if (vfs_shutdown())
        panic("vfs shutdown FAILED!!\n");

#endif

#ifdef __DRIVERS__
    screen_print_shutdown();
#endif

    /* sleep forever */
    while (1)
    {
        __asm__ volatile("cli; hlt;");
    }

    panic("should not get here");
}
