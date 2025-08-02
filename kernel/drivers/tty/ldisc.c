#include "drivers/tty/ldisc.h"
#include <drivers/keyboard.h>
#include <drivers/tty/tty.h>
#include <errno.h>
#include <util/bits.h>
#include <util/debug.h>
#include <util/string.h>

#define ldisc_to_tty(ldisc) CONTAINER_OF((ldisc), tty_t, tty_ldisc)

/**
 * Initialize the line discipline. Don't forget to wipe the buffer associated
 * with the line discipline clean.
 *
 * @param ldisc line discipline.
 */
void ldisc_init(ldisc_t *ldisc)
{
    // Initialize the circular buffer indices
    ldisc->ldisc_head = 0;
    ldisc->ldisc_tail = 0;
    ldisc->ldisc_cooked = 0;
    
    // Initialize the buffer to be empty
    ldisc->ldisc_full = 0;
    
    // Initialize the read queue for threads waiting for data
    sched_queue_init(&ldisc->ldisc_read_queue);
    
    // Clear the buffer
    memset(ldisc->ldisc_buffer, 0, LDISC_BUFFER_SIZE);
}

/**
 * While there are no new characters to be read from the line discipline's
 * buffer, you should make the current thread to sleep on the line discipline's
 * read queue. Note that this sleep can be cancelled. What conditions must be met 
 * for there to be no characters to be read?
 *
 * @param  ldisc the line discipline
 * @return       0 if there are new characters to be read or the ldisc is full.
 *               If the sleep was interrupted, return what
 *               `sched_cancellable_sleep_on` returned (i.e. -EINTR)
 */
long ldisc_wait_read(ldisc_t *ldisc)
{
    long ret = 0;
    
    // Sleep until there are new characters to be read or the ldisc is full
    while (!ret && ldisc->ldisc_tail == ldisc->ldisc_cooked && 
           !ldisc->ldisc_full)
    {
        ret = sched_cancellable_sleep_on(&ldisc->ldisc_read_queue);
        if (ret == -EINTR){
            return -EINTR;
        }   
    }
    
    return ret;
}

/**
 * Reads `count` bytes (at max) from the line discipline's buffer into the
 * provided buffer. Keep in mind the the ldisc's buffer is circular.
 *
 * If you encounter a new line symbol before you have read `count` bytes, you
 * should stop copying and return the bytes read until now.
 * 
 * If you encounter an `EOT` you should stop reading and you should NOT include 
 * the `EOT` in the count of the number of bytes read
 *
 * @param  ldisc the line discipline
 * @param  buf   the buffer to read into.
 * @param  count the maximum number of bytes to read from ldisc.
 * @return       the number of bytes read from the ldisc.
 */
size_t ldisc_read(ldisc_t *ldisc, char *buf, size_t count)
{
    // If no characters available to read, return 0
    if (ldisc->ldisc_tail == ldisc->ldisc_cooked && !ldisc->ldisc_full){
        return 0;
    }
    
    size_t read = 0;
    while (read < count){
        // Copy character from ldisc buffer to output buffer
        buf[read++] = ldisc->ldisc_buffer[ldisc->ldisc_tail];
        ldisc->ldisc_tail = MOD_POW_2(ldisc->ldisc_tail + 1, LDISC_BUFFER_SIZE);
        // if (buf[read - 1] == LF) {
        //     break;
        // }

        // Case for EOT
        if (buf[read - 1] == EOT){
            read--;
            break;
        }
        
        // Case for cooked buffer being full
        if (ldisc->ldisc_tail == ldisc->ldisc_cooked)        {
            break;
        }
    }
    
    // Update full flag
    ldisc->ldisc_full = 0;
    return read;
}

/**
 * Place the character received into the ldisc's buffer. You should also update
 * relevant fields of the struct.
 *
 * An easier way of handling new characters is making sure that you always have
 * one byte left in the line discipline. This way, if the new character you
 * received is a new line symbol (user hit enter), you can still place the new
 * line symbol into the buffer; if the new character is not a new line symbol,
 * you shouldn't place it into the buffer so that you can leave the space for
 * a new line symbol in the future. 
 * 
 * If the line discipline is full, all incoming characters should be ignored. 
 *
 * Here are some special cases to consider:
 *      1. If the character is a backspace:
 *          * if there is a character to remove you must also emit a `\b` to
 *            the vterminal.
 *      2. If the character is end of transmission (EOT) character (typing ctrl-d)
 *      3. If the character is end of text (ETX) character (typing ctrl-c)
 *      4. If your buffer is almost full and what you received is not a new line
 *      symbol
 *
 * If you did receive a new line symbol, you should wake up the thread that is
 * sleeping on the wait queue of the line discipline. You should also
 * emit a `\n` to the vterminal by using `vterminal_write`.  
 * 
 * If you encounter the `EOT` character, you should add it to the buffer, 
 * cook the buffer, and wake up the reader (but do not emit an `\n` character 
 * to the vterminal)
 * 
 * In case of `ETX` you should cause the input line to be effectively transformed
 * into a cooked blank line. You should clear uncooked portion of the line, by 
 * adjusting ldisc_head. You should also emit a "^C" to the vterminal by using
 * `vterminal_write`.
 *
 * Finally, if the none of the above cases apply you should fallback to
 * `vterminal_key_pressed`.
 *
 * Don't forget to write the corresponding characters to the virtual terminal
 * when it applies!
 * 
 * Hint: Test out how ctrl-c and ctrl-d work in your terminal!
 *
 * @param ldisc the line discipline
 * @param c     the new character
 */
void ldisc_key_pressed(ldisc_t *ldisc, char c)
{
    // Get the vterminal for this line discipline
    vterminal_t *vt = &ldisc_to_tty(ldisc)->tty_vterminal;
    
    // Handle backspace
    if (c == BS){
        if (ldisc->ldisc_cooked != ldisc->ldisc_head){
            ldisc->ldisc_head = MOD_POW_2(ldisc->ldisc_head - 1, LDISC_BUFFER_SIZE);
            vterminal_write(vt, "\b", 1);
        }
        return;
    }
    
    // Handle Ctrl-C
    if (c == ETX){
        ldisc->ldisc_head = ldisc->ldisc_cooked;
        c = LF;
        vterminal_write(vt, "^C", 2);
    }
    
    // Case for buffer being full
    if (ldisc->ldisc_full){
        return;
    }
    
    // Case for buffer being almost full
    if ((MOD_POW_2(ldisc->ldisc_head + 1, LDISC_BUFFER_SIZE) == ldisc->ldisc_tail) &&
        (c != LF && c != EOT)){
        return;
    }
    
    // Add character to buffer and update head and full flag
    ldisc->ldisc_buffer[ldisc->ldisc_head] = c;
    ldisc->ldisc_head = MOD_POW_2(ldisc->ldisc_head + 1, LDISC_BUFFER_SIZE);
    ldisc->ldisc_full = (ldisc->ldisc_head == ldisc->ldisc_tail);
    
    // Case for new line or EOT
    if (c == LF || c == EOT){
        ldisc->ldisc_cooked = ldisc->ldisc_head;
        sched_wakeup_on(&ldisc->ldisc_read_queue, NULL);
        
        if (c == LF)
        {
            vterminal_write(vt, "\n", 1);
        }
    }
    else{
        vterminal_key_pressed(vt);
    }
}

/**
 * Copy the raw part of the line discipline buffer into the buffer provided.
 *
 * @param  ldisc the line discipline
 * @param  s     the character buffer to write to
 * @return       the number of bytes copied
 */
size_t ldisc_get_current_line_raw(ldisc_t *ldisc, char *s)
{
    // Calculate the length of the raw portion
    size_t len = MOD_POW_2(ldisc->ldisc_head - ldisc->ldisc_cooked, LDISC_BUFFER_SIZE);
    
    // Case for no raw data
    if (len == 0){
        return 0;
    }
    
    // Case for raw data being contiguous
    if (ldisc->ldisc_head > ldisc->ldisc_cooked){
        memcpy(s, ldisc->ldisc_buffer + ldisc->ldisc_cooked, 
               ldisc->ldisc_head - ldisc->ldisc_cooked);
    } else{
        memcpy(s, ldisc->ldisc_buffer + ldisc->ldisc_cooked,
               LDISC_BUFFER_SIZE - ldisc->ldisc_cooked);
        memcpy(s + (LDISC_BUFFER_SIZE - ldisc->ldisc_cooked),
               ldisc->ldisc_buffer, ldisc->ldisc_head);
    }
    
    return len;
}
