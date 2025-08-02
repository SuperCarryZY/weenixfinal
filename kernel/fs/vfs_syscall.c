#include "fs/vfs_syscall.h"
#include "errno.h"
#include "fs/fcntl.h"
#include "fs/file.h"
#include "fs/lseek.h"
#include "fs/vfs.h"
#include "fs/vnode.h"
#include "globals.h"
#include "kernel.h"
#include "util/debug.h"
#include "util/string.h"
#include <limits.h>

/*
 * Read len bytes into buf from the fd's file using the file's vnode operation
 * read.
 *
 * Return the number of bytes read on success, or:
 *  - EBADF: fd is invalid or is not open for reading
 *  - EISDIR: fd refers to a directory
 *  - Propagate errors from the vnode operation read
 *
 * Hints:
 *  - Be sure to update the file's position appropriately.
 *  - Lock/unlock the file's vnode when calling its read operation.
 */
ssize_t do_read(int fd, void *buf, size_t len)
{
    // Validate file descriptor
    if (fd < 0 || fd >= NFILES) {
        return -EBADF;
    }
    
    // Retrieve file object using fget
    file_t *file_obj = fget(fd);
    if (!file_obj) {
        return -EBADF;
    }
    
    // Verify read permissions
    if (!(file_obj->f_mode & FMODE_READ)) {
        fput(&file_obj);
        return -EBADF;
    }
    
    vnode_t *target_vnode = file_obj->f_vnode;
    
    // Prevent reading from directories
    if (S_ISDIR(target_vnode->vn_mode)) {
        fput(&file_obj);
        return -EISDIR;
    }
    
    // Ensure vnode has read capability
    if (!target_vnode->vn_ops || !target_vnode->vn_ops->read) {
        fput(&file_obj);
        return -EBADF;
    }
    
    // Perform read operation with proper locking
    vlock(target_vnode);
    ssize_t result = target_vnode->vn_ops->read(target_vnode, file_obj->f_pos, buf, len);
    
    // Update position on successful read
    if (result > 0) {
        file_obj->f_pos += result;
    }
    
    vunlock(target_vnode);
    
    // Clean up file reference
    fput(&file_obj);
    
    return result;
}

/*
 * Write len bytes from buf into the fd's file using the file's vnode operation
 * write.
 *
 * Return the number of bytes written on success, or:
 *  - EBADF: fd is invalid or is not open for writing
 *  - Propagate errors from the vnode operation read
 *
 * Hints:
 *  - Check out `man 2 write` for details about how to handle the FMODE_APPEND
 *    flag.
 *  - Be sure to update the file's position appropriately.
 *  - Lock/unlock the file's vnode when calling its write operation.
 */
ssize_t do_write(int fd, const void *buf, size_t len)
{
    // Validate file descriptor
    if (fd < 0 || fd >= NFILES) {
        return -EBADF;
    }
    
    // Retrieve file object using fget
    file_t *file_obj = fget(fd);
    if (!file_obj) {
        return -EBADF;
    }
    
    // Verify write permissions
    if (!(file_obj->f_mode & FMODE_WRITE)) {
        fput(&file_obj);
        return -EBADF;
    }
    
    vnode_t *target_vnode = file_obj->f_vnode;
    
    // Ensure vnode has write capability
    if (!target_vnode->vn_ops || !target_vnode->vn_ops->write) {
        fput(&file_obj);
        return -EBADF;
    }
    
    // Acquire vnode lock for write operation
    vlock(target_vnode);
    
    size_t write_position;
    
    // Handle append mode
    if (file_obj->f_mode & FMODE_APPEND) {
        write_position = target_vnode->vn_len;
    } else {
        write_position = file_obj->f_pos;
    }
    
    // Execute write operation
    ssize_t result = target_vnode->vn_ops->write(target_vnode, write_position, buf, len);
    
    // Update position on successful write
    if (result > 0) {
        if (file_obj->f_mode & FMODE_APPEND) {
            file_obj->f_pos = write_position + result;
        } else {
            file_obj->f_pos += result;
        }
    }
    
    vunlock(target_vnode);
    
    // Clean up file reference
    fput(&file_obj);
    
    return result;
}

/*
 * Close the file descriptor fd.
 *
 * Return 0 on success, or:
 *  - EBADF: fd is invalid or not open
 * 
 * Hints: 
 * Check `proc.h` to see if there are any helpful fields in the 
 * proc_t struct for checking if the file associated with the fd is open. 
 * Consider what happens when we open a file and what counts as closing it
 */
long do_close(int fd)
{
    // Validate file descriptor
    if (fd < 0 || fd >= NFILES) {
        return -EBADF;
    }
    
    // Check if file is currently open
    file_t *file_obj = curproc->p_files[fd];
    if (!file_obj) {
        return -EBADF;
    }
    
    // Remove file from process file table
    curproc->p_files[fd] = NULL;
    
    // Release file reference
    fput(&file_obj);
    
    return 0;
}

/*
 * Duplicate the file descriptor fd.
 *
 * Return the new file descriptor on success, or:
 *  - EBADF: fd is invalid or not open
 *  - Propagate errors from get_empty_fd()
 *
 * Hint: Use get_empty_fd() to obtain an available file descriptor.
 */
long do_dup(int fd)
{
    // Validate file descriptor
    if (fd < 0 || fd >= NFILES) {
        return -EBADF;
    }
    
    // Get original file object
    file_t *original_file = curproc->p_files[fd];
    if (!original_file) {
        return -EBADF;
    }
    
    // Find available file descriptor
    int new_fd;
    long status = get_empty_fd(&new_fd);
    if (status < 0) {
        return status;
    }
    
    // Create duplicate reference
    fref(original_file);
    curproc->p_files[new_fd] = original_file;
    
    return new_fd;
}

/*
 * Duplicate the file descriptor ofd using the new file descriptor nfd. If nfd
 * was previously open, close it.
 *
 * Return nfd on success, or:
 *  - EBADF: ofd is invalid or not open, or nfd is invalid
 *
 * Hint: You don't need to do anything if ofd and nfd are the same.
 * (If supporting MTP, this action must be atomic)
 */
long do_dup2(int ofd, int nfd)
{
    // Validate original file descriptor
    if (ofd < 0 || ofd >= NFILES) {
        return -EBADF;
    }
    
    // Validate new file descriptor
    if (nfd < 0 || nfd >= NFILES) {
        return -EBADF;
    }
    
    // Get original file object
    file_t *original_file = curproc->p_files[ofd];
    if (!original_file) {
        return -EBADF;
    }
    
    // No action needed if descriptors are identical
    if (ofd == nfd) {
        return nfd;
    }
    
    // Close existing file if nfd is open
    if (curproc->p_files[nfd]) {
        do_close(nfd);
    }
    
    // Create duplicate reference
    fref(original_file);
    curproc->p_files[nfd] = original_file;
    
    return nfd;
}

/*
 * Create a file specified by mode and devid at the location specified by path.
 *
 * Return 0 on success, or:
 *  - EINVAL: Mode is not S_IFCHR, S_IFBLK, or S_IFREG
 *  - Propagate errors from namev_open()
 *
 * Hints:
 *  - Create the file by calling namev_open() with the O_CREAT flag.
 *  - Be careful about refcounts after calling namev_open(). The newly created 
 *    vnode should have no references when do_mknod returns. The underlying 
 *    filesystem is responsible for maintaining references to the inode, which 
 *    will prevent it from being destroyed, even if the corresponding vnode is 
 *    cleaned up.
 *  - You don't need to handle EEXIST (this would be handled within namev_open, 
 *    but doing so would likely cause problems elsewhere)
 */
long do_mknod(const char *path, int mode, devid_t devid)
{
    // Validate file mode
    int file_type = _S_TYPE(mode);
    if (file_type != S_IFCHR && file_type != S_IFBLK && file_type != S_IFREG) {
        return -EINVAL;
    }
    
    // Ensure current process exists
    if (!curproc) {
        return -ENOENT;
    }
    
    // Create file using namev_open
    vnode_t *new_vnode;
    long status = namev_open(curproc->p_cwd, path, O_CREAT, mode, devid, &new_vnode);
    if (status < 0) {
        return status;
    }
    
    // Release vnode reference
    vput(&new_vnode);
    
    return 0;
}

/*
 * Create a directory at the location specified by path.
 *
 * Return 0 on success, or:
 *  - ENAMETOOLONG: The last component of path is too long
 *  - ENOTDIR: The parent of the directory to be created is not a directory
 *  - EEXIST: A file located at path already exists
 *  - Propagate errors from namev_dir(), namev_lookup(), and the vnode
 *    operation mkdir
 *
 * Hints:
 * 1) Use namev_dir() to find the parent of the directory to be created.
 * 2) Use namev_lookup() to check that the directory does not already exist.
 * 3) Use the vnode operation mkdir to create the directory.
 *  - Compare against NAME_LEN to determine if the basename is too long.
 *    Check out ramfs_mkdir() to confirm that the basename will be null-
 *    terminated.
 *  - Be careful about locking and refcounts after calling namev_dir() and
 *    namev_lookup().
 */
long do_mkdir(const char *path)
{
    vnode_t *parent_directory;
    const char *directory_name;
    size_t name_length;
    long status;
    
    // Find parent directory
    status = namev_dir(curproc->p_cwd, path, &parent_directory, &directory_name, &name_length);
    if (status < 0) {
        return status;
    }
    
    // Check name length limit
    if (name_length >= NAME_LEN) {
        vput(&parent_directory);
        return -ENAMETOOLONG;
    }
    
    // Check for empty name
    if (name_length == 0) {
        vput(&parent_directory);
        return -EEXIST;
    }
    
    // Verify parent is a directory
    if (!S_ISDIR(parent_directory->vn_mode)) {
        vput(&parent_directory);
        return -ENOTDIR;
    }
    
    // Lock parent directory
    vlock(parent_directory);
    
    // Check if directory already exists
    vnode_t *existing_vnode;
    status = namev_lookup(parent_directory, directory_name, name_length, &existing_vnode);
    if (status == 0) {
        vput(&existing_vnode);
        vunlock(parent_directory);
        vput(&parent_directory);
        return -EEXIST;
    } else if (status != -ENOENT) {
        vunlock(parent_directory);
        vput(&parent_directory);
        return status;
    }
    
    // Create null-terminated name
    char null_terminated_name[NAME_LEN];
    strncpy(null_terminated_name, directory_name, name_length);
    null_terminated_name[name_length] = '\0';
    
    // Verify mkdir operation exists
    if (!parent_directory->vn_ops || !parent_directory->vn_ops->mkdir) {
        vunlock(parent_directory);
        vput(&parent_directory);
        return -ENOTDIR;
    }
    
    // Create directory
    vnode_t *new_directory;
    status = parent_directory->vn_ops->mkdir(parent_directory, null_terminated_name, name_length, &new_directory);
    
    vunlock(parent_directory);
    vput(&parent_directory);
    
    if (status < 0) {
        return status;
    }
    
    // Release reference
    vput(&new_directory);
    
    return 0;
}

/*
 * Delete a directory at path.
 *
 * Return 0 on success, or:
 *  - EINVAL: Attempting to rmdir with "." as the final component
 *  - ENOTEMPTY: Attempting to rmdir with ".." as the final component
 *  - ENOTDIR: The parent of the directory to be removed is not a directory
 *  - ENAMETOOLONG: the last component of path is too long
 *  - Propagate errors from namev_dir() and the vnode operation rmdir
 *
 * Hints:
 *  - Use namev_dir() to find the parent of the directory to be removed.
 *  - Be careful about refcounts from calling namev_dir().
 *  - Use the parent directory's rmdir operation to remove the directory.
 *  - Lock/unlock the vnode when calling its rmdir operation.
 */
long do_rmdir(const char *path)
{
    vnode_t *parent_directory;
    const char *directory_name;
    size_t name_length;
    long status;
    
    // Find parent directory
    status = namev_dir(curproc->p_cwd, path, &parent_directory, &directory_name, &name_length);
    if (status < 0) {
        return status;
    }
    
    // Check name length limit
    if (name_length >= NAME_LEN) {
        vput(&parent_directory);
        return -ENAMETOOLONG;
    }
    
    // Verify parent is a directory
    if (!S_ISDIR(parent_directory->vn_mode)) {
        vput(&parent_directory);
        return -ENOTDIR;
    }
    
    // Check special cases
    if (name_length == 1 && directory_name[0] == '.') {
        vput(&parent_directory);
        return -EINVAL;
    }
    
    if (name_length == 2 && directory_name[0] == '.' && directory_name[1] == '.') {
        vput(&parent_directory);
        return -ENOTEMPTY;
    }
    
    // Create null-terminated name
    char null_terminated_name[NAME_LEN];
    strncpy(null_terminated_name, directory_name, name_length);
    null_terminated_name[name_length] = '\0';
    
    // Lock parent directory
    vlock(parent_directory);
    
    // Verify rmdir operation exists
    if (!parent_directory->vn_ops || !parent_directory->vn_ops->rmdir) {
        vunlock(parent_directory);
        vput(&parent_directory);
        return -ENOTDIR;
    }
    
    // Remove directory
    status = parent_directory->vn_ops->rmdir(parent_directory, null_terminated_name, name_length);
    
    vunlock(parent_directory);
    vput(&parent_directory);
    
    return status;
}

/*
 * Remove the link between path and the file it refers to.
 *
 * Return 0 on success, or:
 *  - ENOTDIR: the parent of the file to be unlinked is not a directory
 *  - ENAMETOOLONG: the last component of path is too long
 *  - Propagate errors from namev_dir() and the vnode operation unlink
 *
 * Hints:
 *  - Use namev_dir() and be careful about refcounts.
 *  - Lock/unlock the parent directory when calling its unlink operation.
 */
long do_unlink(const char *path)
{
    vnode_t *parent_directory;
    const char *file_name;
    size_t name_length;
    long status;
    
    // Ensure current process exists
    if (!curproc) {
        return -ENOENT;
    }
    
    // Find parent directory
    status = namev_dir(curproc->p_cwd, path, &parent_directory, &file_name, &name_length);
    if (status < 0) {
        return status;
    }
    
    // Check name length limit
    if (name_length >= NAME_LEN) {
        vput(&parent_directory);
        return -ENAMETOOLONG;
    }
    
    // Verify parent is a directory
    if (!S_ISDIR(parent_directory->vn_mode)) {
        vput(&parent_directory);
        return -ENOTDIR;
    }
    
    // Create null-terminated name
    char null_terminated_name[NAME_LEN];
    strncpy(null_terminated_name, file_name, name_length);
    null_terminated_name[name_length] = '\0';
    
    // Lock parent directory
    vlock(parent_directory);
    
    // Verify unlink operation exists
    if (!parent_directory->vn_ops || !parent_directory->vn_ops->unlink) {
        vunlock(parent_directory);
        vput(&parent_directory);
        return -ENOTDIR;
    }
    
    // Check if target is a directory
    vnode_t *target_vnode;
    status = namev_lookup(parent_directory, file_name, name_length, &target_vnode);
    if (status == 0) {
        // Prevent unlinking directories
        if (S_ISDIR(target_vnode->vn_mode)) {
            vput(&target_vnode);
            vunlock(parent_directory);
            vput(&parent_directory);
            return -EPERM;
        }
        // Release target vnode reference
        vput(&target_vnode);
    }
    
    // Remove file
    status = parent_directory->vn_ops->unlink(parent_directory, null_terminated_name, name_length);
    
    vunlock(parent_directory);
    
    vput(&parent_directory);
    
    return status;
}

/*
 * Create a hard link newpath that refers to the same file as oldpath.
 *
 * Return 0 on success, or:
 *  - EPERM: oldpath refers to a directory
 *  - ENAMETOOLONG: The last component of newpath is too long
 *  - ENOTDIR: The parent of the file to be linked is not a directory
 *
 * Hints:
 * 1) Use namev_resolve() on oldpath to get the target vnode.
 * 2) Use namev_dir() on newpath to get the directory vnode.
 * 3) Use vlock_in_order() to lock the directory and target vnodes.
 * 4) Use the directory vnode's link operation to create a link to the target.
 * 5) Use vunlock_in_order() to unlock the vnodes.
 * 6) Make sure to clean up references added from calling namev_resolve() and
 *    namev_dir().
 */
long do_link(const char *oldpath, const char *newpath)
{
    vnode_t *target_vnode;
    vnode_t *parent_directory;
    const char *link_name;
    size_t name_length;
    long status;
    
    // Get target vnode
    status = namev_resolve(curproc->p_cwd, oldpath, &target_vnode);
    if (status < 0) {
        return status;
    }
    
    // Prevent linking directories
    if (S_ISDIR(target_vnode->vn_mode)) {
        vput(&target_vnode);
        return -EPERM;
    }
    
    // Get parent directory for new path
    status = namev_dir(curproc->p_cwd, newpath, &parent_directory, &link_name, &name_length);
    if (status < 0) {
        vput(&target_vnode);
        return status;
    }
    
    // Check name length limit
    if (name_length >= NAME_LEN) {
        vput(&target_vnode);
        vput(&parent_directory);
        return -ENAMETOOLONG;
    }
    
    // Verify parent is a directory
    if (!S_ISDIR(parent_directory->vn_mode)) {
        vput(&target_vnode);
        vput(&parent_directory);
        return -ENOTDIR;
    }
    
    // Verify link operation exists
    if (!parent_directory->vn_ops || !parent_directory->vn_ops->link) {
        vput(&target_vnode);
        vput(&parent_directory);
        return -ENOTDIR;
    }
    
    // Create null-terminated name
    char null_terminated_name[NAME_LEN];
    strncpy(null_terminated_name, link_name, name_length);
    null_terminated_name[name_length] = '\0';
    
    // Lock vnodes in proper order
    vlock_in_order(parent_directory, target_vnode);
    
    // Create link
    status = parent_directory->vn_ops->link(parent_directory, null_terminated_name, name_length, target_vnode);
    
    // Unlock vnodes
    vunlock_in_order(parent_directory, target_vnode);
    
    // Release references
    vput(&target_vnode);
    vput(&parent_directory);
    
    return status;
}

/* Rename a file or directory.
 *
 * Return 0 on success, or:
 *  - ENOTDIR: the parent of either path is not a directory
 *  - ENAMETOOLONG: the last component of either path is too long
 *  - Propagate errors from namev_dir() and the vnode operation rename
 *
 * You DO NOT need to support renaming of directories.
 * Steps:
 * 1. namev_dir oldpath --> olddir vnode
 * 2. namev_dir newpath --> newdir vnode
 * 4. Lock the olddir and newdir in ancestor-first order (see `vlock_in_order`)
 * 5. Use the `rename` vnode operation
 * 6. Unlock the olddir and newdir
 * 8. vput the olddir and newdir vnodes
 *
 * Alternatively, you can allow do_rename() to rename directories if
 * __RENAMEDIR__ is set in Config.mk. As with all extra credit
 * projects this is harder and you will get no extra credit (but you
 * will get our admiration). Please make sure the normal version works first.
 * Steps:
 * 1. namev_dir oldpath --> olddir vnode
 * 2. namev_dir newpath --> newdir vnode
 * 3. Lock the global filesystem `vnode_rename_mutex`
 * 4. Lock the olddir and newdir in ancestor-first order (see `vlock_in_order`)
 * 5. Use the `rename` vnode operation
 * 6. Unlock the olddir and newdir
 * 7. Unlock the global filesystem `vnode_rename_mutex`
 * 8. vput the olddir and newdir vnodes
 *
 * P.S. This scheme /probably/ works, but we're not 100% sure.
 */
long do_rename(const char *oldpath, const char *newpath)
{
    vnode_t *old_directory;
    vnode_t *new_directory;
    const char *old_name;
    const char *new_name;
    size_t old_name_length;
    size_t new_name_length;
    long status;
    
    // Find old directory
    status = namev_dir(curproc->p_cwd, oldpath, &old_directory, &old_name, &old_name_length);
    if (status < 0) {
        return status;
    }
    
    // Check old name length
    if (old_name_length >= NAME_LEN) {
        vput(&old_directory);
        return -ENAMETOOLONG;
    }
    
    // Verify old directory is a directory
    if (!S_ISDIR(old_directory->vn_mode)) {
        vput(&old_directory);
        return -ENOTDIR;
    }
    
    // Find new directory
    status = namev_dir(curproc->p_cwd, newpath, &new_directory, &new_name, &new_name_length);
    if (status < 0) {
        vput(&old_directory);
        return status;
    }
    
    // Check new name length
    if (new_name_length >= NAME_LEN) {
        vput(&old_directory);
        vput(&new_directory);
        return -ENAMETOOLONG;
    }
    
    // Verify new directory is a directory
    if (!S_ISDIR(new_directory->vn_mode)) {
        vput(&old_directory);
        vput(&new_directory);
        return -ENOTDIR;
    }
    
    // Verify rename operation exists
    if (!old_directory->vn_ops || !old_directory->vn_ops->rename) {
        vput(&old_directory);
        vput(&new_directory);
        return -ENOTDIR;
    }
    
    // Create null-terminated names
    char null_terminated_old_name[NAME_LEN];
    char null_terminated_new_name[NAME_LEN];
    strncpy(null_terminated_old_name, old_name, old_name_length);
    null_terminated_old_name[old_name_length] = '\0';
    strncpy(null_terminated_new_name, new_name, new_name_length);
    null_terminated_new_name[new_name_length] = '\0';
    
    // Lock directories in ancestor-first order
    vlock_in_order(old_directory, new_directory);
    
    // Perform rename operation
    status = old_directory->vn_ops->rename(old_directory, null_terminated_old_name, old_name_length,
                                         new_directory, null_terminated_new_name, new_name_length);
    
    // Unlock directories
    vunlock_in_order(old_directory, new_directory);
    
    // Release references
    vput(&old_directory);
    vput(&new_directory);
    
    return status;
}

/* Set the current working directory to the directory represented by path.
 *
 * Returns 0 on success, or:
 *  - ENOTDIR: path does not refer to a directory
 *  - Propagate errors from namev_resolve()
 *
 * Hints:
 *  - Use namev_resolve() to get the vnode corresponding to path.
 *  - Pay attention to refcounts!
 *  - Remember that p_cwd should not be locked upon return from this function.
 *  - (If doing MTP, must protect access to p_cwd)
 */
long do_chdir(const char *path)
{
    vnode_t *new_working_directory;
    long status;
    
    // Ensure current process exists
    if (!curproc) {
        return -ENOENT;
    }
    
    // Resolve path to vnode
    status = namev_resolve(curproc->p_cwd, path, &new_working_directory);
    if (status < 0) {
        return status;
    }
    
    // Verify it's a directory
    if (!S_ISDIR(new_working_directory->vn_mode)) {
        vput(&new_working_directory);
        return -ENOTDIR;
    }
    
    // Update current working directory
    vnode_t *old_working_directory = curproc->p_cwd;
    
    // Set new working directory
    curproc->p_cwd = new_working_directory;
    
    // Release old working directory reference
    vput(&old_working_directory);
    
    return 0;
}

/*
 * Read a directory entry from the file specified by fd into dirp.
 *
 * Return sizeof(dirent_t) on success, or:
 *  - EBADF: fd is invalid or is not open
 *  - ENOTDIR: fd does not refer to a directory
 *  - Propagate errors from the vnode operation readdir
 *
 * Hints:
 *  - Use the vnode operation readdir.
 *  - Be sure to update file position according to readdir's return value.
 *  - On success (readdir return value is strictly positive), return
 *    sizeof(dirent_t).
 */
ssize_t do_getdent(int fd, struct dirent *dirp)
{
    // Validate file descriptor
    if (fd < 0 || fd >= NFILES) {
        return -EBADF;
    }
    
    // Get file object
    file_t *file_obj = curproc->p_files[fd];
    if (!file_obj) {
        return -EBADF;
    }
    
    vnode_t *target_vnode = file_obj->f_vnode;
    
    // Verify it's a directory
    if (!S_ISDIR(target_vnode->vn_mode)) {
        return -ENOTDIR;
    }
    
    // Verify readdir operation exists
    if (!target_vnode->vn_ops || !target_vnode->vn_ops->readdir) {
        return -EBADF;
    }
    
    // Perform readdir operation with proper locking
    vlock(target_vnode);
    ssize_t result = target_vnode->vn_ops->readdir(target_vnode, file_obj->f_pos, dirp);
    
    // Update position on success
    if (result > 0) {
        file_obj->f_pos += result;
        result = sizeof(dirent_t);
    }
    
    vunlock(target_vnode);
    
    return result;
}

/*
 * Set the position of the file represented by fd according to offset and
 * whence.
 *
 * Return the new file position, or:
 *  - EBADF: fd is invalid or is not open
 *  - EINVAL: whence is not one of SEEK_SET, SEEK_CUR, or SEEK_END;
 *            or, the resulting file offset would be negative
 *
 * Hints:
 *  - See `man 2 lseek` for details about whence.
 *  - Be sure to protect the vnode if you have to access its vn_len.
 */
off_t do_lseek(int fd, off_t offset, int whence)
{
    // Validate file descriptor
    if (fd < 0 || fd >= NFILES) {
        return -EBADF;
    }
    
    // Get file object
    file_t *file_obj = curproc->p_files[fd];
    if (!file_obj) {
        return -EBADF;
    }
    
    vnode_t *target_vnode = file_obj->f_vnode;
    off_t new_position;
    
    // Calculate new position based on whence
    switch (whence) {
        case SEEK_SET:
            new_position = offset;
            break;
            
        case SEEK_CUR:
            new_position = (off_t)file_obj->f_pos + offset;
            break;
            
        case SEEK_END:
            vlock(target_vnode);
            new_position = (off_t)target_vnode->vn_len + offset;
            vunlock(target_vnode);
            break;
            
        default:
            return -EINVAL;
    }
    
    // Check for negative position
    if (new_position < 0) {
        return -EINVAL;
    }
    
    // Update file position
    file_obj->f_pos = (size_t)new_position;
    
    return new_position;
}

/* Use buf to return the status of the file represented by path.
 *
 * Return 0 on success, or:
 *  - Propagate errors from namev_resolve() and the vnode operation stat.
 */
long do_stat(const char *path, stat_t *buf)
{
    vnode_t *target_vnode;
    long status;
    
    // Ensure current process exists
    if (!curproc) {
        return -ENOENT;
    }
    
    // Resolve path to vnode
    status = namev_resolve(curproc->p_cwd, path, &target_vnode);
    if (status < 0) {
        return status;
    }
    
    // Verify stat operation exists
    if (!target_vnode->vn_ops || !target_vnode->vn_ops->stat) {
        vput(&target_vnode);
        return -EBADF;
    }
    
    // Perform stat operation with proper locking
    vlock(target_vnode);
    status = target_vnode->vn_ops->stat(target_vnode, buf);
    vunlock(target_vnode);
    
    // Release reference
    vput(&target_vnode);
    
    return status;
}

#ifdef __MOUNTING__
/*
 * Implementing this function is not required and strongly discouraged unless
 * you are absolutely sure your Weenix is perfect.
 *
 * This is the syscall entry point into vfs for mounting. You will need to
 * create the fs_t struct and populate its fs_dev and fs_type fields before
 * calling vfs's mountfunc(). mountfunc() will use the fields you populated
 * in order to determine which underlying filesystem's mount function should
 * be run, then it will finish setting up the fs_t struct. At this point you
 * have a fully functioning file system, however it is not mounted on the
 * virtual file system, you will need to call vfs_mount to do this.
 *
 * There are lots of things which can go wrong here. Make sure you have good
 * error handling. Remember the fs_dev and fs_type buffers have limited size
 * so you should not write arbitrary length strings to them.
 */
int do_mount(const char *source, const char *target, const char *type)
{
    NOT_YET_IMPLEMENTED("MOUNTING: do_mount");
    return -EINVAL;
}

/*
 * Implementing this function is not required and strongly discouraged unless
 * you are absolutley sure your Weenix is perfect.
 *
 * This function delegates all of the real work to vfs_umount. You should not
 * worry about freeing the fs_t struct here, that is done in vfs_umount. All
 * this function does is figure out which file system to pass to vfs_umount and
 * do good error checking.
 */
int do_umount(const char *target)
{
    NOT_YET_IMPLEMENTED("MOUNTING: do_umount");
    return -EINVAL;
}
#endif
