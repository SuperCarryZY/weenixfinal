#include "errno.h"
#include "globals.h"
#include "kernel.h"
#include <mm/slab.h>

#include "util/debug.h"
#include "util/printf.h"
#include "util/string.h"

#include "proc/kmutex.h"

#include "fs/dirent.h"
#include "fs/file.h"
#include "fs/s5fs/s5fs.h"
#include "fs/s5fs/s5fs_subr.h"
#include "fs/stat.h"

#include "mm/kmalloc.h"

static long s5_check_super(s5_super_t *super);

static long s5fs_check_refcounts(fs_t *fs);

static void s5fs_read_vnode(fs_t *fs, vnode_t *vn);

static void s5fs_delete_vnode(fs_t *fs, vnode_t *vn);

static long s5fs_umount(fs_t *fs);

static void s5fs_sync(fs_t *fs);

static ssize_t s5fs_read(vnode_t *vnode, size_t pos, void *buf, size_t len);

static ssize_t s5fs_write(vnode_t *vnode, size_t pos, const void *buf,
                          size_t len);

static long s5fs_mmap(vnode_t *file, mobj_t **ret);

static long s5fs_mknod(struct vnode *dir, const char *name, size_t namelen,
                       int mode, devid_t devid, struct vnode **out);

static long s5fs_lookup(vnode_t *dir, const char *name, size_t namelen,
                        vnode_t **out);

static long s5fs_link(vnode_t *dir, const char *name, size_t namelen,
                      vnode_t *child);

static long s5fs_unlink(vnode_t *dir, const char *name, size_t namelen);

static long s5fs_rename(vnode_t *olddir, const char *oldname, size_t oldnamelen,
                        vnode_t *newdir, const char *newname,
                        size_t newnamelen);

static long s5fs_mkdir(vnode_t *dir, const char *name, size_t namelen,
                       struct vnode **out);

static long s5fs_rmdir(vnode_t *parent, const char *name, size_t namelen);

static long s5fs_readdir(vnode_t *vnode, size_t pos, struct dirent *d);

static long s5fs_stat(vnode_t *vnode, stat_t *ss);

static void s5fs_truncate_file(vnode_t *vnode);

static long s5fs_release(vnode_t *vnode, file_t *file);

static long s5fs_get_pframe(vnode_t *vnode, size_t pagenum, long forwrite,
                            pframe_t **pfp);

static long s5fs_fill_pframe(vnode_t *vnode, pframe_t *pf);

static long s5fs_flush_pframe(vnode_t *vnode, pframe_t *pf);

fs_ops_t s5fs_fsops = {.read_vnode = s5fs_read_vnode,
                       .delete_vnode = s5fs_delete_vnode,
                       .umount = s5fs_umount,
                       .sync = s5fs_sync};

static vnode_ops_t s5fs_dir_vops = {.read = NULL,
                                    .write = NULL,
                                    .mmap = NULL,
                                    .mknod = s5fs_mknod,
                                    .lookup = s5fs_lookup,
                                    .link = s5fs_link,
                                    .unlink = s5fs_unlink,
                                    .rename = s5fs_rename,
                                    .mkdir = s5fs_mkdir,
                                    .rmdir = s5fs_rmdir,
                                    .readdir = s5fs_readdir,
                                    .stat = s5fs_stat,
                                    .acquire = NULL,
                                    .release = NULL,
                                    .get_pframe = s5fs_get_pframe,
                                    .fill_pframe = s5fs_fill_pframe,
                                    .flush_pframe = s5fs_flush_pframe,
                                    .truncate_file = NULL};

static vnode_ops_t s5fs_file_vops = {.read = s5fs_read,
                                     .write = s5fs_write,
                                     .mmap = s5fs_mmap,
                                     .mknod = NULL,
                                     .lookup = NULL,
                                     .link = NULL,
                                     .unlink = NULL,
                                     .mkdir = NULL,
                                     .rmdir = NULL,
                                     .readdir = NULL,
                                     .stat = s5fs_stat,
                                     .acquire = NULL,
                                     .release = NULL,
                                     .get_pframe = s5fs_get_pframe,
                                     .fill_pframe = s5fs_fill_pframe,
                                     .flush_pframe = s5fs_flush_pframe,
                                     .truncate_file = s5fs_truncate_file};


static mobj_ops_t s5fs_mobj_ops = {.get_pframe = NULL,
                                   .fill_pframe = blockdev_fill_pframe,
                                   .flush_pframe = blockdev_flush_pframe,
                                   .destructor = NULL};

/*
 * Initialize the passed-in fs_t. The only members of fs_t that are initialized
 * before the call to s5fs_mount are fs_dev and fs_type ("s5fs"). You must
 * initialize everything else: fs_vnode_allocator, fs_i, fs_ops, fs_root.
 *
 * Initialize the block device for the s5fs_t that is created, and copy
 * the super block from disk into memory.
 */
long s5fs_mount(fs_t *fs)
{
    int num;

    KASSERT(fs);

    if (sscanf(fs->fs_dev, "disk%d", &num) != 1)
    {
        return -EINVAL;
    }

    blockdev_t *dev = blockdev_lookup(MKDEVID(DISK_MAJOR, num));
    if (!dev)
        return -EINVAL;

    slab_allocator_t *allocator =
        slab_allocator_create("s5_node", sizeof(s5_node_t));
    fs->fs_vnode_allocator = allocator;

    s5fs_t *s5fs = (s5fs_t *)kmalloc(sizeof(s5fs_t));

    if (!s5fs)
    {
        slab_allocator_destroy(fs->fs_vnode_allocator);
        fs->fs_vnode_allocator = NULL;
        return -ENOMEM;
    }

    mobj_init(&s5fs->s5f_mobj, MOBJ_FS, &s5fs_mobj_ops);
    s5fs->s5f_bdev = dev;

    pframe_t *pf;
    s5_get_meta_disk_block(s5fs, S5_SUPER_BLOCK, 0, &pf);
    memcpy(&s5fs->s5f_super, pf->pf_addr, sizeof(s5_super_t));
    s5_release_disk_block(&pf);

    if (s5_check_super(&s5fs->s5f_super))
    {
        kfree(s5fs);
        slab_allocator_destroy(fs->fs_vnode_allocator);
        fs->fs_vnode_allocator = NULL;
        return -EINVAL;
    }

    kmutex_init(&s5fs->s5f_mutex);

    s5fs->s5f_fs = fs;

    fs->fs_i = s5fs;
    fs->fs_ops = &s5fs_fsops;
    fs->fs_root = vget(fs, s5fs->s5f_super.s5s_root_inode);

    return 0;
}

/*
 * See umount in vfs.h
 *
 * Check reference counts and the super block.
 * Put the fs_root.
 * Write the super block out to disk.
 * Flush the underlying memory object.
 */
static long s5fs_umount(fs_t *fs)
{
    s5fs_t *s5fs = FS_TO_S5FS(fs);
    blockdev_t *bd = s5fs->s5f_bdev;

    if (s5fs_check_refcounts(fs))
    {
        panic(
            "s5fs_umount: WARNING: linkcount corruption "
            "discovered in fs on block device with major %d "
            "and minor %d!!\n",
            MAJOR(bd->bd_id), MINOR(bd->bd_id));
    }
    if (s5_check_super(&s5fs->s5f_super))
    {
        panic(
            "s5fs_umount: WARNING: corrupted superblock "
            "discovered on fs on block device with major %d "
            "and minor %d!!\n",
            MAJOR(bd->bd_id), MINOR(bd->bd_id));
    }

    vput(&fs->fs_root);

    s5fs_sync(fs);
    kfree(s5fs);
    return 0;
}

static void s5fs_sync(fs_t *fs)
{
    s5fs_t *s5fs = FS_TO_S5FS(fs);
    mobj_t *mobj = &s5fs->s5f_mobj;

    
    pframe_t *pf;
    s5_get_meta_disk_block(s5fs, S5_SUPER_BLOCK, 1, &pf);
    memcpy(pf->pf_addr, &s5fs->s5f_super, sizeof(s5_super_t));
    s5_release_disk_block(&pf);

    mobj_lock(&s5fs->s5f_mobj);
    mobj_flush(mobj);
    mobj_unlock(&s5fs->s5f_mobj);
}

/* Initialize a vnode and inode by reading its corresponding inode info from
 * disk.
 *
 * Hints:
 *  - To read the inode from disk, you will need to use the following:
 *     - VNODE_TO_S5NODE to obtain the s5_node_t with the inode corresponding
 *       to the provided vnode
 *     - FS_TO_S5FS to obtain the s5fs object
 *     - S5_INODE_BLOCK(vn->v_vno) to determine the block number of the block that
 *       contains the inode info
 *     - s5_get_disk_block and s5_release_disk_block to handle the disk block
 *     - S5_INODE_OFFSET to find the desired inode within the disk block
 *       containing it (returns the offset that the inode is stored within the block)
 *  - You should initialize the s5_node_t's inode field by reading directly from
 *    the inode on disk by using the page frame returned from s5_get_disk_block. Also 
 *    make sure to initialize the dirtied_inode field.
 *  - Using the inode info, you need to initialize the following vnode fields:
 *    vn_len, vn_mode, and vn_ops using the fields found in the s5_inode struct.
 *  - See stat.h for vn_mode values.
 *  - For character and block devices:
 *    1) Initialize vn_devid by reading the inode's s5_indirect_block field.
 *    2) Set vn_ops to NULL.
 */
static void s5fs_read_vnode(fs_t *fs, vnode_t *vn)
{
    s5_node_t *sn = VNODE_TO_S5NODE(vn);
    s5fs_t *s5fs = FS_TO_S5FS(fs);
    
    pframe_t *pf;
    s5_get_meta_disk_block(s5fs, S5_INODE_BLOCK(vn->vn_vno), 0, &pf);
    
    s5_inode_t *disk_inode = (s5_inode_t *)pf->pf_addr + S5_INODE_OFFSET(vn->vn_vno);
    
    memcpy(&sn->inode, disk_inode, sizeof(s5_inode_t));
    
    // Initialize dirtied_inode field
    sn->dirtied_inode = 0;
    
    // Release the disk block
    s5_release_disk_block(&pf);
    
    // Initialize vnode fields based on inode info
    s5_inode_t *inode = &sn->inode;
    
    // Set file length  
    if (inode->s5_type == S5_TYPE_DATA || inode->s5_type == S5_TYPE_DIR) {
        vn->vn_len = inode->s5_un.s5_size;
    } else {
        vn->vn_len = 0;
    }
    
    // Set vnode mode based on inode type
    switch (inode->s5_type) {
        case S5_TYPE_DATA:
            vn->vn_mode = S_IFREG;
            vn->vn_ops = &s5fs_file_vops;
            break;
        case S5_TYPE_DIR:
            vn->vn_mode = S_IFDIR;
            vn->vn_ops = &s5fs_dir_vops;
            break;
        case S5_TYPE_CHR:
            vn->vn_mode = S_IFCHR;
            vn->vn_ops = NULL;
            vn->vn_devid = inode->s5_indirect_block; // Device ID stored in indirect block
            break;
        case S5_TYPE_BLK:
            vn->vn_mode = S_IFBLK;
            vn->vn_ops = NULL;
            vn->vn_devid = inode->s5_indirect_block; // Device ID stored in indirect block
            break;
        default:
            panic("s5fs_read_vnode: invalid inode type %d\n", inode->s5_type);
    }
}

/* Clean up the inode corresponding to the given vnode.
 *
 * Hints:
 *  - This function is called in the following way: 
 *          mobj_put -> vnode_destructor -> s5fs_delete_vnode.
 *  - Cases to consider:
 *    1) The inode is no longer in use (linkcount == 0), so free it using
 *       s5_free_inode.
 *    2) The inode is dirty, so write it back to disk.
 *    3) The inode is unchanged, so do nothing.
 */
static void s5fs_delete_vnode(fs_t *fs, vnode_t *vn)
{
    s5_node_t *sn = VNODE_TO_S5NODE(vn);
    s5_inode_t *inode = &sn->inode;
    s5fs_t *s5fs = FS_TO_S5FS(fs);
    
    // The case where the inode is no longer in use
    if (inode->s5_linkcount == 0) {
        s5_free_inode(s5fs, vn->vn_vno);
        return;
    }
    
    // The case where the inode is dirty
    if (sn->dirtied_inode) {
        pframe_t *pf;
        s5_inode_t *disk_inode;
        
        s5_get_meta_disk_block(s5fs, S5_INODE_BLOCK(vn->vn_vno), 1, &pf);
        
        disk_inode = (s5_inode_t *)pf->pf_addr + S5_INODE_OFFSET(vn->vn_vno);
        
        memcpy(disk_inode, inode, sizeof(s5_inode_t));
        
        s5_release_disk_block(&pf);
        
        sn->dirtied_inode = 0;
        return;
    }
    
}

/* Wrapper around s5_read_file. */
static ssize_t s5fs_read(vnode_t *vnode, size_t pos, void *buf, size_t len)
{
    KASSERT(!S_ISDIR(vnode->vn_mode) && "should be handled at the VFS level");
    
    s5_node_t *sn = VNODE_TO_S5NODE(vnode);
    return s5_read_file(sn, pos, (char *)buf, len);
}

/* Wrapper around s5_write_file. */
static ssize_t s5fs_write(vnode_t *vnode, size_t pos, const void *buf,
                          size_t len)
{
    KASSERT(!S_ISDIR(vnode->vn_mode) && "should be handled at the VFS level");
    
    s5_node_t *sn = VNODE_TO_S5NODE(vnode);
    return s5_write_file(sn, pos, (const char *)buf, len);
}

/*
 * Any error handling should have been done before this function was called.
 * Simply add a reference to the underlying mobj and return it through ret.
 */
static long s5fs_mmap(vnode_t *file, mobj_t **ret)
{
    NOT_YET_IMPLEMENTED("VM: s5fs_mmap");
    return 0;
}

/* Allocate and initialize an inode and its corresponding vnode.
 *
 *  dir     - The directory in which to make the new inode
 *  name    - The name of the new inode
 *  namelen - Name length
 *  mode    - vn_mode of the new inode, see S_IF{} macros in stat.h
 *  devid   - devid of the new inode for special devices
 *  out     - Upon success, out must point to the newly created vnode
 *            Upon failure, out must be unchanged
 *
 * Return 0 on success, or:
 *  - ENOTSUP: mode is not S_IFCHR, S_BLK, or S_ISREG
 *  - Propagate errors from s5_alloc_inode and s5_link
 *
 * Hints:
 *  - Use mode to determine the S5_TYPE_{} for the inode.
 *  - Use s5_alloc_inode is allocate a new inode.
 *  - Use vget to obtain the vnode corresponding to the newly created inode.
 *  - Use s5_link to link the newly created inode/vnode to the parent directory.
 *    - You will need to clean up the vnode using vput in the case that 
 *      the link operation fails. 
 */
static long s5fs_mknod(struct vnode *dir, const char *name, size_t namelen,
                       int mode, devid_t devid, struct vnode **out)
{
    KASSERT(S_ISDIR(dir->vn_mode) && "should be handled at the VFS level");
    
    s5fs_t *s5fs = VNODE_TO_S5FS(dir);
    s5_node_t *dir_sn = VNODE_TO_S5NODE(dir);
    
    // Determine the S5_TYPE based on mode
    uint16_t s5_type;
    if (S_ISREG(mode)) {
        s5_type = S5_TYPE_DATA;
    } else if (S_ISCHR(mode)) {
        s5_type = S5_TYPE_CHR;
    } else if (S_ISBLK(mode)) {
        s5_type = S5_TYPE_BLK;
    } else {
        return -ENOTSUP;
    }
    
    // Allocate a new inode
    long new_ino = s5_alloc_inode(s5fs, s5_type, devid);
    if (new_ino < 0) {
        return new_ino;
    }
    
    // Get the vnode corresponding to the newly created inode
    vnode_t *new_vnode = vget(dir->vn_fs, new_ino);
    KASSERT(new_vnode != NULL);
    
    // Link the newly created inode/vnode to the parent directory
    // VFS already locks dir, no need to lock again
    long ret = s5_link(dir_sn, name, namelen, VNODE_TO_S5NODE(new_vnode));
    if (ret < 0) {
        vput(&new_vnode);
        return ret;
    }
    
    *out = new_vnode;
    return 0;
}

/* Search for a given entry within a directory.
 *
 *  dir     - The directory in which to search
 *  name    - The name to search for
 *  namelen - Name length
 *  ret     - Upon success, ret must point to the found vnode
 *
 * Return 0 on success, or:
 *  - Propagate errors from s5_find_dirent
 *
 * Hints:
 *  - Use s5_find_dirent, vget, and vref.
 *  - vref can be used in the case where the vnode you're looking for happens
 *    to be dir itself.
 */
long s5fs_lookup(vnode_t *dir, const char *name, size_t namelen,
                 vnode_t **ret)
{
    KASSERT(S_ISDIR(dir->vn_mode) && "should be handled at the VFS level");
    
    s5_node_t *dir_sn = VNODE_TO_S5NODE(dir);
    
    // Search for the directory entry
    long inode_num = s5_find_dirent(dir_sn, name, namelen, NULL);
    if (inode_num < 0) {
        return inode_num;
    }
    
    // Handle special case: entry refers to the directory itself
    if (inode_num == dir->vn_vno) {
        // Use vref to increment reference count
        vref(dir);
        *ret = dir;
        return 0;
    }
    
    // Retrieve the vnode for the found inode
    vnode_t *child_vnode = vget(dir->vn_fs, inode_num);
    KASSERT(child_vnode != NULL);
    
    *ret = child_vnode;
    return 0;
}

/* Wrapper around s5_link.
 *
 * Return whatever s5_link returns, or:
 *  - EISDIR: child is a directory
 */
static long s5fs_link(vnode_t *dir, const char *name, size_t namelen,
                      vnode_t *child)
{
    KASSERT(S_ISDIR(dir->vn_mode) && "should be handled at the VFS level");
    
    // Prevent linking directories
    if (S_ISDIR(child->vn_mode)) {
        return -EISDIR;
    }
    
    s5_node_t *dir_sn = VNODE_TO_S5NODE(dir);
    s5_node_t *child_sn = VNODE_TO_S5NODE(child);
    
    // Create the hard link using s5_link
    // Note: VFS already locks dir, so no additional locking needed
    return s5_link(dir_sn, name, namelen, child_sn);
}

/* Remove the directory entry in dir corresponding to name and namelen.
 *
 * Return 0 on success, or:
 *  - Propagate errors from s5_find_dirent
 *
 * Hints:
 *  - Use s5_find_dirent and s5_remove_dirent.
 *  - You will probably want to use vget_locked and vput_locked to protect the
 *    found vnode. Make sure your implementation of s5_remove_dirent knows what
 *    to expect.
 */
static long s5fs_unlink(vnode_t *dir, const char *name, size_t namelen)
{
    KASSERT(S_ISDIR(dir->vn_mode) && "should be handled at the VFS level");
    KASSERT(!name_match(".", name, namelen));
    KASSERT(!name_match("..", name, namelen));
    
    s5_node_t *dir_sn = VNODE_TO_S5NODE(dir);
    
    // Locate the directory entry
    long child_ino = s5_find_dirent(dir_sn, name, namelen, NULL);
    if (child_ino < 0) {
        return child_ino;
    }
    
    // Acquire and protect the found vnode
    vnode_t *child_vnode = vget_locked(dir->vn_fs, child_ino);
    KASSERT(child_vnode != NULL);
    s5_node_t *child_sn = VNODE_TO_S5NODE(child_vnode);
    
    // Remove the directory entry
    s5_remove_dirent(dir_sn, name, namelen, child_sn);
    
    // Release the vnode reference
    vput_locked(&child_vnode);
    
    return 0;
}

/* Change the name or location of a file.
 *
 *  olddir     - The directory in which the file currently resides
 *  oldname    - The old name of the file
 *  oldnamelen - Length of the old name
 *  newdir     - The directory in which to place the file
 *  newname    - The new name of the file
 *  newnamelen - Length of the new name
 *
 * Return 0 on success, or:
 *  - ENAMETOOLONG: newname is >= NAME_LEN
 *  - ENOTDIR: newdir is not a directory
 *  - EISDIR: newname is a directory 
 *  - Propagate errors from s5_find_dirent and s5_link
 *
 * Steps:
 * 1) Use s5_find_dirent and vget_locked to obtain the vnode corresponding to old name.
 * 2) If newdir already contains an entry for newname:
 *      a) Compare node numbers and do nothing if old name and new name refer to the same inode
 *	    b) Check if new-name is a directory
 *	    c) Remove the previously existing entry for new name using s5_remove_dirent
 *	    d) Link the new direct using s5_link
 * 3) If there is no entry for newname, use s5_link to add a link to the old node at new name
 * 4) Use s5_remove_dirent to remove old name's entry in olddir
 *
 * 
 * Hints:
 *  - olddir and newdir should be locked on entry and not unlocked during the
 *    duration of this function. Any other vnodes locked should be unlocked and
 *    put before return.
 *  - Be careful with locking! Because you are making changes to the vnodes,
 *    you should always be using vget_locked and vput_locked. Be sure to clean
 *    up properly in error/special cases.
 *  - You DO NOT need to support renaming of directories in Weenix. If you were to support this
 *    in the s5fs layer (which is not extra credit), you can use the following routine:
 *	  1) Use s5_find_dirent and vget_locked to obtain the vnode corresponding to old name.
 *	  2) If newer already contains an entry for newname:
 *		   a) Compare node numbers and do nothing if old name and new name refer to the same inode
 *		   b) Check if new-name is a directory
 *		   c) Remove the previously existing entry for new name using s5_remove_dirent
 *		   d) Link the new direct using s5_link
 * 	3) If there is no entry for newname, use s5_link to add a link to the old node at new name
 *	4) Use s5_remove_dirent to remove old name's entry in olddir
 */
static long s5fs_rename(vnode_t *olddir, const char *oldname, size_t oldnamelen,
                        vnode_t *newdir, const char *newname,
                        size_t newnamelen)
{
    // Validate the new name length
    if (newnamelen >= S5_NAME_LEN) {
        return -ENAMETOOLONG;
    }
    
    // Ensure newdir is actually a directory
    if (!S_ISDIR(newdir->vn_mode)) {
        return -ENOTDIR;
    }
    
    s5_node_t *olddir_sn = VNODE_TO_S5NODE(olddir);
    s5_node_t *newdir_sn = VNODE_TO_S5NODE(newdir);
    
    // Find the old file and get its vnode
    long old_ino = s5_find_dirent(olddir_sn, oldname, oldnamelen, NULL);
    if (old_ino < 0) {
        return old_ino;
    }
    
    vnode_t *old_vnode = vget_locked(olddir->vn_fs, old_ino);
    KASSERT(old_vnode != NULL);
    s5_node_t *old_sn = VNODE_TO_S5NODE(old_vnode);
    
    // Check if newdir already contains an entry for newname
    long new_ino = s5_find_dirent(newdir_sn, newname, newnamelen, NULL);
    
    if (new_ino >= 0) {
        // Entry exists for newname
        
        // Compare node numbers - if same inode, do nothing
        if (new_ino == old_ino) {
            vput_locked(&old_vnode);
            return 0;
        }
        
        // Get the existing new file's vnode
        vnode_t *new_vnode = vget_locked(newdir->vn_fs, new_ino);
        KASSERT(new_vnode != NULL);
        
        // Check if new-name is a directory
        if (S_ISDIR(new_vnode->vn_mode)) {
            vput_locked(&new_vnode);
            vput_locked(&old_vnode);
            return -EISDIR;
        }
        
        // Lock newdir before calling s5_remove_dirent
        
        // Remove the previously existing entry for new name
        s5_remove_dirent(newdir_sn, newname, newnamelen, VNODE_TO_S5NODE(new_vnode));
        
        // Link the new directory entry (VFS already locks newdir)
        long ret = s5_link(newdir_sn, newname, newnamelen, old_sn);
        
        vput_locked(&new_vnode);
        
        if (ret < 0) {
            vput_locked(&old_vnode);
            return ret;
        }
    } else if (new_ino == -ENOENT) {
        // No entry for newname, create new link
        long ret = s5_link(newdir_sn, newname, newnamelen, old_sn);
        
        if (ret < 0) {
            vput_locked(&old_vnode);
            return ret;
        }
    } else {
        vput_locked(&old_vnode);
        return new_ino;
    }
    
    // Lock olddir before calling s5_remove_dirent
    // Remove old name's entry in olddir (VFS already locks olddir)
    s5_remove_dirent(olddir_sn, oldname, oldnamelen, old_sn);
    
    vput_locked(&old_vnode);
    return 0;
}

/* Create a directory.
 *
 *  dir     - The directory in which to create the new directory
 *  name    - The name of the new directory
 *  namelen - Name length of the new directory
 *  out     - On success, must point to the new directory, unlocked
 *            On failure, must be unchanged
 *
 * Return 0 on success, or:
 *  - Propagate errors from s5_alloc_inode and s5_link
 *
 * Steps:
 * 1) Allocate an inode.
 * 2) Get the child directory vnode.
 * 3) Create the "." entry.
 * 4) Create the ".." entry.
 * 5) Create the name/namelen entry in the parent (that corresponds 
 *    to the new directory)
 *
 * Hints:
 *  - If you run into any errors, you must undo previous steps.
 *  - You may assume/assert that undo operations do not fail.
 *  - It may help to assert that linkcounts are correct.
 */
static long s5fs_mkdir(vnode_t *dir, const char *name, size_t namelen,
                       struct vnode **out)
{
    KASSERT(S_ISDIR((dir)->vn_mode) && "should be handled at the VFS level");
    
    s5fs_t *s5fs = VNODE_TO_S5FS(dir);
    s5_node_t *parent_sn = VNODE_TO_S5NODE(dir);
    
    // Create a new inode for the directory
    long new_ino = s5_alloc_inode(s5fs, S5_TYPE_DIR, 0);
    if (new_ino < 0) {
        return new_ino;
    }
    
    // Retrieve the child directory vnode
    vnode_t *child_vnode = vget(dir->vn_fs, new_ino);
    KASSERT(child_vnode != NULL);
    s5_node_t *child_sn = VNODE_TO_S5NODE(child_vnode);
    
    // Lock the child vnode for safe writing
    vlock(child_vnode);
    
    // Create the "." entry (child points to itself)
    s5_dirent_t dot_entry;
    dot_entry.s5d_inode = child_sn->inode.s5_number;
    memcpy(dot_entry.s5d_name, ".", 1);
    memset(dot_entry.s5d_name + 1, 0, S5_NAME_LEN - 1);
    
    long ret = s5_write_file(child_sn, 0, (const char *)&dot_entry, sizeof(s5_dirent_t));
    if (ret < 0) {
        vunlock(child_vnode);
        vput(&child_vnode);
        return ret;
    }
    child_sn->inode.s5_linkcount++;
    child_sn->dirtied_inode = 1;
    
    // Create the ".." entry (child points to parent)
    s5_dirent_t dotdot_entry;
    dotdot_entry.s5d_inode = parent_sn->inode.s5_number;
    memcpy(dotdot_entry.s5d_name, "..", 2);
    memset(dotdot_entry.s5d_name + 2, 0, S5_NAME_LEN - 2);
    
    ret = s5_write_file(child_sn, sizeof(s5_dirent_t), (const char *)&dotdot_entry, sizeof(s5_dirent_t));
    if (ret < 0) {
        // Clean up: reset directory length and linkcount
        child_vnode->vn_len = 0;
        child_sn->inode.s5_un.s5_size = 0;
        child_sn->inode.s5_linkcount--;
        child_sn->dirtied_inode = 1;
        vunlock(child_vnode);
        vput(&child_vnode);
        return ret;
    }
    parent_sn->inode.s5_linkcount++;
    parent_sn->dirtied_inode = 1;
    
    // Unlock the child vnode
    vunlock(child_vnode);
    
    // Create the name/namelen entry in parent (parent points to child)
    // VFS already locks dir, no need to lock again
    ret = s5_link(parent_sn, name, namelen, child_sn);
    if (ret < 0) {
        // Clean up the "." and ".." entries we just created
        child_vnode->vn_len = 0;
        child_sn->inode.s5_un.s5_size = 0;
        child_sn->inode.s5_linkcount -= 1;  // Remove link from "."
        parent_sn->inode.s5_linkcount--;    // Remove link from ".."
        child_sn->dirtied_inode = 1;
        parent_sn->dirtied_inode = 1;
        vput(&child_vnode);
        return ret;
    }
    
    KASSERT(child_sn->inode.s5_linkcount == 2);
    KASSERT(parent_sn->inode.s5_linkcount >= 2);
    
    *out = child_vnode;
    return 0;
}

/* Remove a directory.
 *
 * Return 0 on success, or:
 *  - ENOTDIR: The specified entry is not a directory
 *  - ENOTEMPTY: The directory to be removed has entries besides "." and ".."
 *  - Propagate errors from s5_find_dirent
 *
 * Hints:
 *  - If you are confident you are managing directory entries properly, you can
 *    check for ENOTEMPTY by simply checking the length of the directory to be
 *    removed. An empty directory has two entries: "." and "..". 
 *  - Remove the three entries created in s5fs_mkdir.
 */
static long s5fs_rmdir(vnode_t *parent, const char *name, size_t namelen)
{
    KASSERT(!name_match(".", name, namelen));
    KASSERT(!name_match("..", name, namelen));
    KASSERT(S_ISDIR(parent->vn_mode) && "should be handled at the VFS level");
    
    s5_node_t *parent_sn = VNODE_TO_S5NODE(parent);
    
    // Find the directory to be removed
    long child_ino = s5_find_dirent(parent_sn, name, namelen, NULL);
    if (child_ino < 0) {
        return child_ino;
    }
    
    // Protect the child vnode
    vnode_t *child_vnode = vget_locked(parent->vn_fs, child_ino);
    KASSERT(child_vnode != NULL);
    s5_node_t *child_sn = VNODE_TO_S5NODE(child_vnode);
    
    // Check if the found entry is a directory
    if (!S_ISDIR(child_vnode->vn_mode)) {
        vput_locked(&child_vnode);
        return -ENOTDIR;
    }
    
    // Check if the directory is empty
    if (child_vnode->vn_len != 2 * sizeof(s5_dirent_t)) {
        vput_locked(&child_vnode);
        return -ENOTEMPTY;
    }
    
    // Remove the three entries created in s5fs_mkdir
    s5_remove_dirent(parent_sn, name, namelen, child_sn);
    s5_remove_dirent(child_sn, "..", 2, parent_sn);
    s5_remove_dirent(child_sn, ".", 1, child_sn);

    // Release the child vnode
    vput_locked(&child_vnode);
    
    return 0;
}

/* Read a directory entry.
 *
 *  vnode - The directory from which to read an entry
 *  pos   - The position within the directory to start reading from
 *  d     - Caller-allocated dirent that must be properly initialized on
 *          successful return
 *
 * Return bytes read on success, or:
 *  - Propagate errors from s5_read_file
 *
 * Hints:
 *  - Use s5_read_file to read an s5_dirent_t. To do so, you can create a local 
 *    s5_dirent_t variable and use that as the buffer to pass into s5_read_file. 
 *  - Be careful that you read into an s5_dirent_t and populate the provided
 *    dirent_t properly.
 */
static long s5fs_readdir(vnode_t *vnode, size_t pos, struct dirent *d)
{
    KASSERT(S_ISDIR(vnode->vn_mode) && "should be handled at the VFS level");
    
    s5_node_t *dir_sn = VNODE_TO_S5NODE(vnode);
    s5_dirent_t s5_dirent;
    
    // Read a single s5_dirent_t structure
    ssize_t bytes_read = s5_read_file(dir_sn, pos, (char *)&s5_dirent, sizeof(s5_dirent_t));

    if (bytes_read < 0) {
        return bytes_read;
    }
    
    // Check for end of directory
    if (bytes_read == 0) {
        return 0;
    }
    
    // Verify we read a complete directory entry
    if (bytes_read != sizeof(s5_dirent_t)) {
        return 0;
    }
    
    // Populate the provided dirent structure
    d->d_ino = s5_dirent.s5d_inode;
    d->d_off = pos + sizeof(s5_dirent_t);
    
    // Copy the name
    size_t name_len = strnlen(s5_dirent.s5d_name, S5_NAME_LEN);
    memcpy(d->d_name, s5_dirent.s5d_name, name_len);
    d->d_name[name_len] = '\0';
    
    // Return the number of bytes that should be added to pos
    return sizeof(s5_dirent_t);
}

/* Get file status.
 *
 *  vnode - The vnode of the file in question
 *  ss    - Caller-allocated stat_t struct that must be initialized on success
 *
 * This function should not fail.
 *
 * Hint:
 *  - Initialize st_blocks using s5_inode_blocks.
 *  - Initialize st_mode using the corresponding vnode modes in stat.h.
 *  - Initialize st_rdev with the devid of special devices.
 *  - Initialize st_ino with the inode number.
 *  - Initialize st_nlink with the linkcount.
 *  - Initialize st_blksize with S5_BLOCK_SIZE.
 *  - Initialize st_size with the size of the file.
 *  - Initialize st_dev with the bd_id of the s5fs block device.
 *  - Set all other fields to 0.
 */
static long s5fs_stat(vnode_t *vnode, stat_t *ss)
{
    s5_node_t *sn = VNODE_TO_S5NODE(vnode);
    s5_inode_t *inode = &sn->inode;
    s5fs_t *s5fs = VNODE_TO_S5FS(vnode);
    
    // Initialize all fields to zero
    memset(ss, 0, sizeof(stat_t));
    
    ss->st_blocks = s5_inode_blocks(sn);
    ss->st_mode = vnode->vn_mode;
    
    // Handle special device files
    if (S_ISCHR(vnode->vn_mode) || S_ISBLK(vnode->vn_mode)) {
        ss->st_rdev = vnode->vn_devid;
    } else {
        ss->st_rdev = 0;
    }
    
    ss->st_ino = vnode->vn_vno;
    ss->st_nlink = inode->s5_linkcount;
    ss->st_blksize = S5_BLOCK_SIZE;
    ss->st_size = vnode->vn_len;
    ss->st_dev = s5fs->s5f_bdev->bd_id;
    
    return 0;
}

/**
 * Truncate the vnode and inode length to be 0. 
 * 
 * file - the vnode, whose size should be truncated 
 * 
 * This routine should only be called from do_open via 
 * vn_ops in the case that a regular file is opened with the 
 * O_TRUNC flag specified. 
 */
static void s5fs_truncate_file(vnode_t *file)
{
    KASSERT(S_ISREG(file->vn_mode) && "This routine should only be called for regular files");
    file->vn_len = 0;
    s5_node_t* s5_node = VNODE_TO_S5NODE(file); 
    s5_inode_t* s5_inode = &s5_node->inode; 
    // setting the size of the inode to be 0 as well 
    s5_inode->s5_un.s5_size = 0; 
    s5_node->dirtied_inode = 1; 
    
    // Call subroutine to free the blocks that were used 
    // VFS has already locked the file vnode before calling this function
    s5_remove_blocks(s5_node);  
}

/*
 * Wrapper around device's read_block function; first looks up block in file-system cache.
 * If not there, allocates and fills a page frame. 
 * Used for meta blocks (i.e. indirect blocks, inode storage, and super block), thus location is passed in.
 */
inline void s5_get_meta_disk_block(s5fs_t *s5fs, uint64_t blocknum, long forwrite,
                              pframe_t **pfp)
{
    mobj_lock(&s5fs->s5f_mobj);
    mobj_find_pframe(&s5fs->s5f_mobj, blocknum, pfp);
    if (*pfp)
    {
        // block is cached
        (*pfp)->pf_dirty |= forwrite;
        mobj_unlock(&s5fs->s5f_mobj);
        return;
    }
    mobj_create_pframe(&s5fs->s5f_mobj, blocknum, blocknum, pfp);
    pframe_t *pf = *pfp;
    pf->pf_addr = page_alloc();
    KASSERT(pf->pf_addr);

    blockdev_t *bd = s5fs->s5f_bdev;
    long ret = bd->bd_ops->read_block(bd, pf->pf_addr, (blocknum_t)pf->pf_loc, 1);
    pf->pf_dirty |= forwrite;  // yes, needed
    KASSERT (!ret);
    mobj_unlock(&s5fs->s5f_mobj);
    KASSERT(!ret && *pfp);
}

/*
 * Wrapper around device's read_block function; allocates and fills a page frame.
 * Assumes cache has already been searched.
 * Used for file blocks, thus file block number is supplied.
 */
static inline void s5_get_file_disk_block(vnode_t *vnode, uint64_t blocknum, uint64_t loc, long forwrite,
                              pframe_t **pfp)
{
    mobj_create_pframe(&vnode->vn_mobj, blocknum, loc, pfp);
    pframe_t *pf = *pfp;
    pf->pf_addr = page_alloc();
    KASSERT(pf->pf_addr);
    blockdev_t *bd = VNODE_TO_S5FS(vnode)->s5f_bdev;
    long ret = bd->bd_ops->read_block(bd, pf->pf_addr, (blocknum_t)pf->pf_loc, 1);
    pf->pf_dirty |= forwrite;
    KASSERT (!ret);
}

/* Wrapper around pframe_release.
 *
 * Note: All pframe_release does is unlock the pframe. Why aren't we actually
 * writing anything back yet? Because the pframe remains associated with
 * whatever mobj we provided when we originally called mobj_get_pframe. If
 * anyone tries to access the pframe later, Weenix will just give them the
 * cached page frame from the mobj. If the pframe is ever freed (most likely on
 * shutdown), then it will be written back to disk: mobj_flush_pframe ->
 * blockdev_flush_pframe.
 */
inline void s5_release_disk_block(pframe_t **pfp) { pframe_release(pfp); }

/*
 * This is where the abstraction of vnode file block/page --> disk block is
 * finally implemented. Check that the requested page lies within vnode->vn_len.
 *
 * Of course, you will want to use s5_file_block_to_disk_block. Pay attention
 * to what the forwrite argument to s5fs_get_pframe means for the alloc argument
 * in s5_file_block_to_disk_block.
 *
 * If the disk block for the corresponding file block is sparse, you should use
 * mobj_default_get_pframe on the vnode's own memory object. This will trickle
 * down to s5fs_fill_pframe if the pframe is not already resident.
 *
 * Otherwise, if the disk block is NOT sparse, you will want to simply use
 * s5_get_disk_block. NOTE: in this case, you also need to make sure you free
 * the pframe that resides in the vnode itself for the requested pagenum. To
 * do so, you will want to use mobj_find_pframe and mobj_free_pframe.
 *
 * Given the above design, we s5fs itself does not need to implement
 * flush_pframe. Any pframe that will be written to (forwrite = 1) should always
 * have a disk block backing it on successful return. Thus, the page frame will
 * reside in the block device of the filesystem, where the flush_pframe is
 * already implemented. We do, however, need to implement fill_pframe for sparse
 * blocks.
 */
static long s5fs_get_pframe(vnode_t *vnode, uint64_t pagenum, long forwrite,
                            pframe_t **pfp)
{
    if (vnode->vn_len <= pagenum * PAGE_SIZE)
        return -EINVAL;
    mobj_find_pframe(&vnode->vn_mobj, pagenum, pfp);
    if (*pfp)
    {
        // block is cached
        (*pfp)->pf_dirty |= forwrite;
        return 0;
    }
    int new;
    long loc = s5_file_block_to_disk_block(VNODE_TO_S5NODE(vnode), pagenum, forwrite, &new);
    if (loc < 0)
        return loc;
    if (loc) {
        // block is mapped 
        if (new) {
            // block didn't previously exist, thus its current contents are meaningless
            *pfp = s5_cache_and_clear_block(&vnode->vn_mobj, pagenum, loc);
        } else {
            // block must be read from disk
            s5_get_file_disk_block(vnode, pagenum, loc, forwrite, pfp);
        }
        return 0;
    }
    else
    {
        // block is in a sparse region of the file
        KASSERT(!forwrite);
        return mobj_default_get_pframe(&vnode->vn_mobj, pagenum, forwrite, pfp);
    }
}

/*
 * According the documentation for s5fs_get_pframe, this only gets called when
 * the file block for a given page number is sparse. In other words, pf
 * corresponds to a sparse block.
 */
static long s5fs_fill_pframe(vnode_t *vnode, pframe_t *pf)
{
    memset(pf->pf_addr, 0, PAGE_SIZE);
    return 0;
}

static long s5fs_flush_pframe(vnode_t *vnode, pframe_t *pf) {
    return blockdev_flush_pframe(&VNODE_TO_S5FS(vnode)->s5f_mobj, pf);
}

/*
 * Verify the superblock. 0 on success; -1 on failure.
 */
static long s5_check_super(s5_super_t *super)
{
    if (!(super->s5s_magic == S5_MAGIC &&
          (super->s5s_free_inode < super->s5s_num_inodes ||
           super->s5s_free_inode == (uint32_t)-1) &&
          super->s5s_root_inode < super->s5s_num_inodes))
    {
        return -1;
    }
    if (super->s5s_version != S5_CURRENT_VERSION)
    {
        dbg(DBG_PRINT,
            "Filesystem is version %d; "
            "only version %d is supported.\n",
            super->s5s_version, S5_CURRENT_VERSION);
        return -1;
    }
    return 0;
}

/*
 * Calculate refcounts on the filesystem.
 */
static void calculate_refcounts(int *counts, vnode_t *vnode)
{
    long ret;

    size_t pos = 0;
    dirent_t dirent;
    vnode_t *child;

    while ((ret = s5fs_readdir(vnode, pos, &dirent)) > 0)
    {
        counts[dirent.d_ino]++;
        dbg(DBG_S5FS, "incrementing count of inode %d to %d\n", dirent.d_ino,
            counts[dirent.d_ino]);
        if (counts[dirent.d_ino] == 1)
        {
            child = vget_locked(vnode->vn_fs, dirent.d_ino);
            if (S_ISDIR(child->vn_mode))
            {
                calculate_refcounts(counts, child);
            }
            vput_locked(&child);
        }
        pos += ret;
    }

    KASSERT(!ret);
}

/*
 * Verify refcounts on the filesystem. 0 on success; -1 on failure.
 */
long s5fs_check_refcounts(fs_t *fs)
{
    s5fs_t *s5fs = (s5fs_t *)fs->fs_i;
    int *refcounts;
    long ret = 0;

    refcounts = kmalloc(s5fs->s5f_super.s5s_num_inodes * sizeof(int));
    KASSERT(refcounts);
    memset(refcounts, 0, s5fs->s5f_super.s5s_num_inodes * sizeof(int));

    vlock(fs->fs_root);
    refcounts[fs->fs_root->vn_vno]++;
    calculate_refcounts(refcounts, fs->fs_root);
    refcounts[fs->fs_root->vn_vno]--;

    vunlock(fs->fs_root);

    dbg(DBG_PRINT,
        "Checking refcounts of s5fs filesystem on block "
        "device with major %d, minor %d\n",
        MAJOR(s5fs->s5f_bdev->bd_id), MINOR(s5fs->s5f_bdev->bd_id));

    for (uint32_t i = 0; i < s5fs->s5f_super.s5s_num_inodes; i++)
    {
        if (!refcounts[i])
        {
            continue;
        }

        vnode_t *vn = vget(fs, i);
        KASSERT(vn);
        s5_node_t *sn = VNODE_TO_S5NODE(vn);

        if (refcounts[i] != sn->inode.s5_linkcount)
        {
            dbg(DBG_PRINT, "   Inode %d, expecting %d, found %d\n", i,
                refcounts[i], sn->inode.s5_linkcount);
            ret = -1;
        }
        vput(&vn);
    }

    dbg(DBG_PRINT,
        "Refcount check of s5fs filesystem on block "
        "device with major %d, minor %d completed %s.\n",
        MAJOR(s5fs->s5f_bdev->bd_id), MINOR(s5fs->s5f_bdev->bd_id),
        (ret ? "UNSUCCESSFULLY" : "successfully"));

    kfree(refcounts);
    return ret;
}
