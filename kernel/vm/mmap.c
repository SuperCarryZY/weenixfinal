#include "vm/mmap.h"
#include "errno.h"
#include "fs/file.h"
#include "fs/vfs.h"
#include "fs/vnode.h"
#include "globals.h"
#include "mm/mm.h"
#include "mm/mman.h"
#include "mm/tlb.h"
#include "util/debug.h"

/*
 * This function implements the mmap(2) syscall: Add a mapping to the current
 * process's address space. Supports the following flags: MAP_SHARED,
 * MAP_PRIVATE, MAP_FIXED, and MAP_ANON.
 *
 *  ret - If provided, on success, *ret must point to the start of the mapped area
 *
 * Return 0 on success, or:
 *  - EACCES: 
 *     - a file mapping was requested, but fd is not open for reading. 
 *     - MAP_SHARED was requested and PROT_WRITE is set, but fd is
 *       not open in read/write (O_RDWR) mode.
 *     - PROT_WRITE is set, but the file has FMODE_APPEND specified.
 *  - EBADF:
 *     - fd is not a valid file descriptor and MAP_ANON was
 *       not set
 *  - EINVAL:
 *     - addr is not page aligned and MAP_FIXED is specified 
 *     - addr is out of range of the user address space and MAP_FIXED is specified
 *     - off is not page aligned
 *     - len is <= 0 or off < 0
 *     - flags do not contain MAP_PRIVATE or MAP_SHARED
 *  - ENODEV:
 *     - The underlying filesystem of the specified file does not
 *       support memory mapping or in other words, the file's vnode's mmap
 *       operation doesn't exist
 *  - Propagate errors from vmmap_map()
 * 
 *  See the errors section of the mmap(2) man page for more details
 * 
 * Hints:
 *  1) A lot of error checking.
 *  2) Call vmmap_map() to create the mapping.
 *     a) Use VMMAP_DIR_HILO as default, which will make other stencil code in
 *        Weenix happy.
 *  3) Call tlb_flush_range() on the newly-mapped region. This is because the
 *     newly-mapped region could have been used by someone else, and you don't
 *     want to get stale mappings.
 *  4) Don't forget to set ret if it was provided.
 * 
 *  If you are mapping less than a page, make sure that you are still allocating 
 *  a full page.
 */
long do_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off,
             void **ret)
{
    NOT_YET_IMPLEMENTED("VM: do_mmap");
    return -1;
}

/*
 * This function implements the munmap(2) syscall.
 *
 * Return 0 on success, or:
 *  - EINVAL:
 *     - addr is not aligned on a page boundary
 *     - the region to unmap is out of range of the user address space
 *     - len is 0
 *  - Propagate errors from vmmap_remove()
 * 
 *  See the errors section of the munmap(2) man page for more details
 *
 * Hints:
 *  - Similar to do_mmap():
 *  1) Perform error checking.
 *  2) Call vmmap_remove().
 */
long do_munmap(void *addr, size_t len)
{
    NOT_YET_IMPLEMENTED("VM: do_munmap");
    return -1;
}