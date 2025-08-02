#include "errno.h"
#include "globals.h"

#include "util/debug.h"
#include "util/string.h"

#include "mm/kmalloc.h"
#include "mm/mobj.h"

#include "drivers/chardev.h"

#include "vm/anon.h"

#include "fs/vnode.h"

static ssize_t null_read(chardev_t *dev, size_t pos, void *buf, size_t count);

static ssize_t null_write(chardev_t *dev, size_t pos, const void *buf,
                          size_t count);

static ssize_t zero_read(chardev_t *dev, size_t pos, void *buf, size_t count);

static long zero_mmap(vnode_t *file, mobj_t **ret);

chardev_ops_t null_dev_ops = {.read = null_read,
                              .write = null_write,
                              .mmap = NULL,
                              .fill_pframe = NULL,
                              .flush_pframe = NULL};

chardev_ops_t zero_dev_ops = {.read = zero_read,
                              .write = null_write,
                              .mmap = zero_mmap,
                              .fill_pframe = NULL,
                              .flush_pframe = NULL};

/**
 * The char device code needs to know about these mem devices, so create
 * chardev_t's for null and zero, fill them in, and register them.
 *
 * Use kmalloc, MEM_NULL_DEVID, MEM_ZERO_DEVID, and chardev_register.
 * See dev.h for more details on how device identifiers are assigned.
 */
void memdevs_init()
{
    // Create and initialize null device
    chardev_t *null_dev = kmalloc(sizeof(chardev_t));
    null_dev->cd_id = MEM_NULL_DEVID;
    null_dev->cd_ops = &null_dev_ops;
    list_link_init(&null_dev->cd_link);
    chardev_register(null_dev);
    
    // Create and initialize zero device
    chardev_t *zero_dev = kmalloc(sizeof(chardev_t));
    zero_dev->cd_id = MEM_ZERO_DEVID;
    zero_dev->cd_ops = &zero_dev_ops;
    list_link_init(&zero_dev->cd_link);
    chardev_register(zero_dev);
}

/**
 * Reads a given number of bytes from the null device into a
 * buffer. Any read performed on the null device should read 0 bytes.
 *
 * @param  dev   the null device
 * @param  pos   the offset to read from; should be ignored
 * @param  buf   the buffer to read into
 * @param  count the maximum number of bytes to read
 * @return       the number of bytes read, which should be 0
 */
static ssize_t null_read(chardev_t *dev, size_t pos, void *buf, size_t count)
{
    // Reading from null device always returns 0 bytes (EOF)
    return 0;
}

/**
 * Writes a given number of bytes to the null device from a
 * buffer. Writing to the null device should _ALWAYS_ be successful
 * and write the maximum number of bytes.
 *
 * @param  dev   the null device
 * @param  pos   offset the offset to write to; should be ignored
 * @param  buf   buffer to read from
 * @param  count the maximum number of bytes to write
 * @return       the number of bytes written, which should be `count`
 */
static ssize_t null_write(chardev_t *dev, size_t pos, const void *buf,
                          size_t count)
{
    // Writing to null device always succeeds and discards all data
    return count;
}

/**
 * Reads a given number of bytes from the zero device into a
 * buffer. Any read from the zero device should be a series of zeros.
 *
 * @param  dev   the zero device
 * @param  pos   the offset to start reading from; should be ignored
 * @param  buf   the buffer to write to
 * @param  count the maximum number of bytes to read
 * @return       the number of bytes read. Hint: should always read the maximum
 *               number of bytes
 */
static ssize_t zero_read(chardev_t *dev, size_t pos, void *buf, size_t count)
{
    // Fill buffer with zeros
    memset(buf, 0, count);
    return count;
}

/**
 * Unlike in s5fs_mmap(), you can't necessarily use the file's underlying mobj.
 * Instead, you should simply provide an anonymous object to ret. Keep the
 * anonymous object locked when this function returns.
 */
static long zero_mmap(vnode_t *file, mobj_t **ret)
{
    // Create an anonymous memory object for /dev/zero mmap
    mobj_t *anon_obj = anon_create();
    if (!anon_obj){
        return -ENOMEM;
    }
    
    // Keep the anonymous object locked when returning
    mobj_lock(anon_obj);
    *ret = anon_obj;
    return 0;
}
