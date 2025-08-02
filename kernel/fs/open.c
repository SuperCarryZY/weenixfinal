#include "errno.h"
#include "fs/fcntl.h"
#include "fs/file.h"
#include "fs/vfs.h"
#include "fs/vfs_syscall.h"
#include "fs/vnode.h"
#include "globals.h"
#include "util/debug.h"
#include "drivers/chardev.h"
#include "drivers/blockdev.h"

// NOTE: IF DOING MULTI-THREADED PROCS, NEED TO SYNCHRONIZE ACCESS TO FILE
// DESCRIPTORS, AND, MORE GENERALLY SPEAKING, p_files, IN PARTICULAR IN THIS
// FUNCTION AND ITS CALLERS.
/*
 * Go through curproc->p_files and find the first null entry.
 * If one exists, set fd to that index and return 0.
 *
 * Error cases get_empty_fd is responsible for generating:
 *  - EMFILE: no empty file descriptor
 */
long get_empty_fd(int *fd)
{
    for (*fd = 0; *fd < NFILES; (*fd)++)
    {
        if (!curproc->p_files[*fd])
        {
            return 0;
        }
    }
    *fd = -1;
    return -EMFILE;
}

/*
 * Open the file at the provided path with the specified flags.
 *
 * Returns the file descriptor on success, or error cases:
 *  - EINVAL: Invalid oflags
 *  - EISDIR: Trying to open a directory with write access
 *  - ENXIO: Blockdev or chardev vnode does not have an actual underlying device
 *  - ENOMEM: Not enough kernel memory (if fcreate() fails)
 *
 * Hints:
 * 1) Use get_empty_fd() to get an available fd.
 * 2) Use namev_open() with oflags, mode S_IFREG, and devid 0.
 * 3) Check for EISDIR and ENXIO errors.
 * 4) Convert oflags (O_RDONLY, O_WRONLY, O_RDWR, O_APPEND) into corresponding
 *    file access flags (FMODE_READ, FMODE_WRITE, FMODE_APPEND). 
 * 5) Use fcreate() to create and initialize the corresponding file descriptor
 *    with the vnode from 2) and the mode from 4).
 *
 * When checking oflags, you only need to check that the read and write
 * permissions are consistent. However, because O_RDONLY is 0 and O_RDWR is 2,
 * there's no way to tell if both were specified. So, you really only need
 * to check if O_WRONLY and O_RDWR were specified. 
 * 
 * If O_TRUNC specified and the vnode represents a regular file, make sure to call the
 * the vnode's truncate routine (to reduce the size of the file to 0).
 *
 * If a vnode represents a chardev or blockdev, then the appropriate field of
 * the vnode->vn_dev union will point to the device. Otherwise, the union will be NULL.
 * To check for the ENXIO error, if the vnode is a block device or char device but the
 * device pointed to by the union is NULL then return ENXIO.
 * 
 * Keep in mind that O_RDONLY is 0 and FMODE_READ is 1 to avoid confusion.
 */
long do_open(const char *filename, int oflags)
{
    int fd;
    long ret;
    vnode_t *vnode;
    file_t *file;
    
    // Check for invalid oflags
    if ((oflags & O_WRONLY) && (oflags & O_RDWR)) {
        return -EINVAL;
    }
    
    // Get an empty file descriptor
    ret = get_empty_fd(&fd);
    if (ret < 0) {
        return ret;
    }
    
    // Check if curproc is valid
    if (!curproc) {
        return -ENOENT;
    }
    
    // Use namev_open to get the vnode
    ret = namev_open(curproc->p_cwd, filename, oflags, S_IFREG, 0, &vnode);
    if (ret < 0) {
        return ret;
    }
    
    // Check for DIR & CHR & BLK errors
    if (S_ISDIR(vnode->vn_mode) && (oflags & (O_WRONLY | O_RDWR))) {
        vput(&vnode);
        return -EISDIR;
    }
    
    if (S_ISCHR(vnode->vn_mode)) {
        if (!vnode->vn_dev.chardev) {
            vput(&vnode);
            return -ENXIO;
        }
    } else if (S_ISBLK(vnode->vn_mode)) {
        if (!vnode->vn_dev.blockdev) {
            vput(&vnode);
            return -ENXIO;
        }
    }
    
    // Convert oflags to file mode flags
    unsigned int fmode = 0;
    if (oflags & O_RDWR) {
        fmode |= FMODE_READ | FMODE_WRITE;
    } else if (oflags & O_WRONLY) {
        fmode |= FMODE_WRITE;
    } else { /* O_RDONLY is 0 */
        fmode |= FMODE_READ;
    }
    
    if (oflags & O_APPEND) {
        fmode |= FMODE_APPEND;
    }
    
    // Handle case for regular files
    if ((oflags & O_TRUNC) && S_ISREG(vnode->vn_mode) && 
        (fmode & FMODE_WRITE) && vnode->vn_ops && vnode->vn_ops->truncate_file) {
        vlock(vnode);
        vnode->vn_ops->truncate_file(vnode);
        vunlock(vnode);
    }
    
    // Create the file descriptor
    file = fcreate(fd, vnode, fmode);
    if (!file) {
        vput(&vnode);
        return -ENOMEM;
    }
    
    // Need to release the reference from namev_open
    vput(&vnode);
    
    return fd;
}
