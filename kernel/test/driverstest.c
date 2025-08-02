#include "errno.h"
#include "globals.h"

#include "test/usertest.h"
#include "test/proctest.h"

#include "util/debug.h"
#include "util/printf.h"
#include "util/string.h"

#include "proc/proc.h"
#include "proc/kthread.h"
#include "proc/sched.h"

#include "drivers/tty/tty.h"
#include "drivers/dev.h"
#include "drivers/blockdev.h"
#include "drivers/keyboard.h"

#define TEST_STR_1 "hello\n"
#define TEST_STR_2 "different string\n"
#define TEST_STR_3 "test"
#define TEST_BUF_SZ 10
#define NUM_PROCS 3
#define BLOCK_NUM 1

// Keep LDISC_BUFFER_SIZE at 128 for these tests

// TODO: need to change to using the MOD macro 

void* kthread_write(long arg1, void* arg2) {
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, arg1));
    tty_t* tty = cd_to_tty(cd); 

    int count = 0;
    while (count < 2) {
        if (count == 0) {
            for (size_t i = 0; i < strlen(TEST_STR_1); i++) {
                ldisc_key_pressed(&tty->tty_ldisc, TEST_STR_1[i]); 
            }
        } else {
            for (size_t i = 0; i < strlen(TEST_STR_2); i++) {
                ldisc_key_pressed(&tty->tty_ldisc, TEST_STR_2[i]); 
            } 
        }
        count++;
    }
    return NULL; 
}

void* kthread_read1(long arg1, void* arg2) {
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, arg1));
    char buf[32]; 
    memset(buf, 0, 32);
    size_t num_bytes = cd->cd_ops->read(cd, 0, buf, strlen(TEST_STR_1)); 
    test_assert(num_bytes == strlen(TEST_STR_1), "number of bytes is incorrect"); 
    test_assert(!strncmp(buf, TEST_STR_1, strlen(TEST_STR_1)), "resulting strings are not equal"); 

    return NULL; 
}

void* kthread_read2(long arg1, void* arg2) {
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, arg1));

    char buf[32]; 
    memset(buf, 0, 32);
    size_t num_bytes = cd->cd_ops->read(cd, 0, buf, strlen(TEST_STR_2)); 
    test_assert(num_bytes == strlen(TEST_STR_2), "number of bytes is incorrect"); 
    test_assert(!strncmp(buf, TEST_STR_2, strlen(TEST_STR_2)), "resulting strings are not equal");  

    return NULL; 
}

long test_concurrent_reads() {
    proc_t* proc_write = proc_create("process_write");
    kthread_t* kt_write = kthread_create(proc_write, kthread_write, 0, NULL); 

    proc_t* proc_1 = proc_create("process_1_read"); 
    kthread_t* kthread_1 = kthread_create(proc_1, kthread_read1, 0, NULL); 

    proc_t* proc_2 = proc_create("process_2_read"); 
    kthread_t* kthread_2 = kthread_create(proc_2, kthread_read2, 0, NULL); 

    sched_make_runnable(kthread_1); 
    sched_make_runnable(kthread_2); 
    sched_make_runnable(kt_write); 

    while (do_waitpid(-1, NULL, 0) != -ECHILD)
        ;

    return 0; 
}

/**
 * Function for each kthread to write the order in which they were spawned 
 * to the character device. 
*/
void* kthread_concurrent_write(long arg1, void* arg2) {
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, 0));
    char buf[32];
    memset(buf, 0, 32); 
    snprintf(buf, 32, "thread_%d\n", (int)arg1); 
    size_t num_bytes = cd->cd_ops->write(cd, 0, buf, strlen(buf)); 
    test_assert(num_bytes == strlen(buf), "number of bytes written is not correct"); 
    return NULL; 
} 

long test_concurrent_writes() {
    char proc_name[32];
    for (int i = 0; i < NUM_PROCS; i++) {
        memset(proc_name, 0, 32); 
        snprintf(proc_name, 32, "process_concurrent_write_%d", i); 
        proc_t* proc_write = proc_create(proc_name); 
        kthread_t* kt_write = kthread_create(proc_write, kthread_concurrent_write, i, NULL);
        sched_make_runnable(kt_write); 
    }

    while (do_waitpid(-1, NULL, 0) != -ECHILD)
        ;
        
    return 0;
}

void* kthread_write_disk(long arg1, void* arg2) {
    // write to disk here 
    void* page_of_data = page_alloc(); 
    // memset it to be some random character
    memset(page_of_data, 'F', BLOCK_SIZE); 
    blockdev_t* bd = blockdev_lookup(MKDEVID(DISK_MAJOR, 0));
    long ret = bd->bd_ops->write_block(bd, (char*)page_of_data, arg1, 1); 
    test_assert(ret == 0, "the write operation failed");

    return NULL;  
}

void* kthread_read_disk(long arg1, void* arg2) {
    // read that same block of data here 
    // not going to memset it because we are reading that amount
    void* page_of_data_to_read = page_alloc_n(2); 
    void* data_expected = page_alloc_n(2); 
    memset(data_expected, 'F', BLOCK_SIZE); 
    blockdev_t* bd = blockdev_lookup(MKDEVID(DISK_MAJOR, 0));
    test_assert(!PAGE_ALIGNED((char*)page_of_data_to_read+1), "not page aligned"); 
    long ret = bd->bd_ops->read_block(bd, (char*)page_of_data_to_read+1, arg1, 1); 
    test_assert(ret == 0, "the read operation failed"); 
    test_assert(0 == memcmp((char*)page_of_data_to_read+1, data_expected, BLOCK_SIZE), "bytes are not equal");
    page_free_n(page_of_data_to_read, 2);
    page_free_n(data_expected, 2);
    return NULL; 
}

/*
    First write to disk and then attempt to read from disk 
*/
long test_disk_write_and_read() {
    proc_t* proc_write = proc_create("process_write");
    kthread_t* kt_write = kthread_create(proc_write, kthread_write_disk, BLOCK_NUM, NULL); 

    proc_t* proc_read = proc_create("process_read"); 
    kthread_t* kt_read = kthread_create(proc_read, kthread_read_disk, BLOCK_NUM, NULL); 

    sched_make_runnable(kt_write);
    sched_make_runnable(kt_read); 

    while (do_waitpid(-1, NULL, 0) != -ECHILD)
    ; 

    return 0; 
}

/*
    Tests inputting a character and a newline character 
*/
long test_basic_line_discipline() { 
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, 0)); 
    tty_t* tty = cd_to_tty(cd); 
    ldisc_t* ldisc = &tty->tty_ldisc; 
    ldisc_key_pressed(ldisc, 't'); 

    test_assert(ldisc->ldisc_buffer[ldisc->ldisc_tail] == 't', "character not inputted into buffer correctly"); 
    test_assert(ldisc->ldisc_head != ldisc->ldisc_cooked && ldisc->ldisc_tail != ldisc->ldisc_head, "pointers are updated correctly");

    size_t previous_head_val = ldisc->ldisc_head; 
    ldisc_key_pressed(ldisc, '\n'); 
    test_assert(ldisc->ldisc_head == previous_head_val + 1, "ldisc_head should have been incremented past newline character");
    test_assert(ldisc->ldisc_cooked == ldisc->ldisc_head, "ldisc_cooked should be equal to ldisc_head"); 

    // reset line discipline for other tests before returning 
    ldisc->ldisc_head = ldisc->ldisc_cooked = ldisc->ldisc_tail = 0; 
    return 0; 
}

/*
    Tests removing a character 
*/
long test_backspace() {
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, 0)); 
    tty_t* tty = cd_to_tty(cd); 
    ldisc_t* ldisc = &tty->tty_ldisc; 
    size_t previous_head_val = ldisc->ldisc_head; 
    ldisc_key_pressed(ldisc, 't'); 
    ldisc_key_pressed(ldisc, '\b'); 
    test_assert(ldisc->ldisc_head == previous_head_val, "Backspace should move the ldisc_head back by 1"); 

    // testing there should be no characters to remove 
    ldisc_key_pressed(ldisc, '\b');
    test_assert(ldisc->ldisc_head == previous_head_val, "This backspace should result in a no-op"); 

    // reset line discipline for other tests before returning 
    ldisc->ldisc_head = ldisc->ldisc_cooked = ldisc->ldisc_tail = 0; 
    return 0;  
}

void* kthread_wait_for_eot(long arg1, void* arg2) {
    chardev_t* cd = (chardev_t*)arg2;
    char buf[32]; 
    memset(buf, 0, 32);
    size_t num_bytes = cd->cd_ops->read(cd, 0, buf, TEST_BUF_SZ); 
    test_assert(num_bytes == strlen(TEST_STR_3), "number of bytes is incorrect"); 
    test_assert(!strncmp(buf, TEST_STR_3, strlen(TEST_STR_3)), "resulting strings are not equal"); 
    return NULL; 
}

/*
    Tests the behavior for EOT 
*/
long test_eot() {
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, 0)); 
    tty_t* tty = cd_to_tty(cd); 
    ldisc_t* ldisc = &tty->tty_ldisc; 

    proc_t* proc_read = proc_create("process_read");
    kthread_t* kt_read = kthread_create(proc_read, kthread_wait_for_eot, 0, cd);  
    sched_make_runnable(kt_read);
    // allow the other process to run first so it can block before typing 
    sched_yield(); 
    
    size_t prev_tail_value = ldisc->ldisc_tail; 
    for (size_t i = 0; i < strlen(TEST_STR_3); i++) {
        ldisc_key_pressed(ldisc, TEST_STR_3[i]); 
    }
    ldisc_key_pressed(ldisc, EOT); 
    test_assert(ldisc->ldisc_head == ldisc->ldisc_cooked, "ldisc_head should be equal to ldisc_cooked"); 

    // allow the other thread to read 
    while (do_waitpid(-1, NULL, 0) != -ECHILD)
    ;  
    test_assert(ldisc->ldisc_tail == prev_tail_value + strlen(TEST_STR_3) + 1, "ldisc_tail should be past the EOT char"); 
    ldisc->ldisc_head = ldisc->ldisc_tail = ldisc->ldisc_cooked = 0; 
    return 0;  
}

/*
    Tests the behavior for ETX 
*/
long test_etx() {
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, 0)); 
    tty_t* tty = cd_to_tty(cd); 
    ldisc_t* ldisc = &tty->tty_ldisc; 
    size_t previous_head_value = ldisc->ldisc_head; 

    // "press" two characters 
    ldisc_key_pressed(ldisc, 't'); 
    ldisc_key_pressed(ldisc, 'e'); 
    ldisc_key_pressed(ldisc, ETX); 

    test_assert(previous_head_value + 1 == ldisc->ldisc_head, "ldisc_head should only be one past where it used to be");
    test_assert(ldisc->ldisc_head == ldisc->ldisc_cooked, "ldisc should be a cooked blank line"); 

    // reset line discipline for other tests before returning 
    ldisc->ldisc_head = ldisc->ldisc_cooked = ldisc->ldisc_tail = 0; 
    return 0; 
}

/*
    Tests the behavior for a full line discipline
*/
long test_full_line_discipline() {
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, 0)); 
    tty_t* tty = cd_to_tty(cd); 
    ldisc_t* ldisc = &tty->tty_ldisc;
    for (int i = 0; i < LDISC_BUFFER_SIZE; i++) {
        ldisc_key_pressed(ldisc, 't'); 
    }

    test_assert(ldisc->ldisc_head == LDISC_BUFFER_SIZE - 1, "ldisc should leave keep one byte left for new line character"); 

    ldisc_key_pressed(ldisc, '\n'); 
    test_assert(ldisc->ldisc_head == 0, "ldisc_head should wrap back to 0");
    test_assert(ldisc->ldisc_cooked == ldisc->ldisc_head, "ldisc_cooked should be equal to ldisc_head"); 

    // reset line discipline for other tests before returning 
    ldisc->ldisc_head = ldisc->ldisc_cooked = ldisc->ldisc_tail = ldisc->ldisc_full = 0; 
    return 0; 
}

/*
    Tests the behavior for when the line discipline wraps
*/
long test_line_discipline_wrap() {
    chardev_t* cd = chardev_lookup(MKDEVID(TTY_MAJOR, 0)); 
    tty_t* tty = cd_to_tty(cd); 
    ldisc_t* ldisc = &tty->tty_ldisc;
    for (int i = 0; i < LDISC_BUFFER_SIZE/2; i++) {
        ldisc_key_pressed(ldisc, 't'); 
    }

    test_assert(ldisc->ldisc_head == LDISC_BUFFER_SIZE/2, "ldisc_head should be incremented to half the ldisc size"); 
    size_t previous_head_val = ldisc->ldisc_head; 
    ldisc_key_pressed(ldisc, '\n'); 
    test_assert(ldisc->ldisc_head == previous_head_val + 1, "ldisc_head should have been incremented past newline character");
    test_assert(ldisc->ldisc_cooked == ldisc->ldisc_head, "ldisc_cooked should be equal to ldisc_head");

    ldisc->ldisc_tail = ldisc->ldisc_cooked; // Emulate reading the cooked portion of the buffer

    for (int i = 0; i < LDISC_BUFFER_SIZE/2 - 1; i++) {
        ldisc_key_pressed(ldisc, 'z'); 
    }
    ldisc_key_pressed(ldisc, '\n'); 

    test_assert(ldisc->ldisc_head == 1, "ldisc_head should wrap around"); 

    // reset line discipline for other tests before returning 
    ldisc->ldisc_head = ldisc->ldisc_cooked = ldisc->ldisc_tail = 0; 
    return 0; 
}

long driverstest_main(long arg1, void* arg2)
{
    dbg(DBG_TEST, "\nStarting Drivers tests\n");
    test_init();

    test_basic_line_discipline();
    test_backspace();  
    test_eot();
    test_etx(); 
    test_disk_write_and_read();
    test_full_line_discipline();
    test_line_discipline_wrap();
    test_concurrent_reads(); 
    test_concurrent_writes(); 

    test_fini();
    return 0;
}