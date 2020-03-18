#include <linux/module.h>
#include <linux/fs.h>

#define MOD_NAME "PDFS"

/* readdir */
static int pdfs_iterate(struct file *filp, struct dir_context *ctx)
{
	return 0;
}

static const struct file_operations pdfs_dir_operations = {
	.owner = THIS_MODULE,
	.iterate = pdfs_iterate
};

static struct dentry *pdfs_lookup(struct inode *parent_inode, struct dentry *child_dentry, unsigned int flags)
{
	return NULL;
}

static const struct inode_operations pdfs_inode_operations = {
	.lookup = pdfs_lookup
};

static struct inode *pdfs_get_inode(struct super_block *sb, const struct inode *dir, umode_t mode, dev_t dev)
{
	struct inode *inode = new_inode(sb);

	if (!inode)
		return NULL;

	inode->i_ino = get_next_ino();
	inode_init_owner(inode, dir, mode);
	inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);
	switch (mode & S_IFMT) {
		case S_IFDIR:
			inc_nlink(inode);
			break;
		case S_IFREG:
		case S_IFLNK:
		default:
			printk(KERN_ERR MOD_NAME " only supports inode for root directory at the moment\n");
		return NULL;
		break;
	}
	return inode;
}

static int pdfs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct inode *inode;

	sb->s_magic = 0x10032019;

	inode = pdfs_get_inode(sb, NULL, S_IFDIR, 0);
	inode->i_op = &pdfs_inode_operations;
	inode->i_fop = &pdfs_dir_operations;
	sb->s_root = d_make_root(inode);
	if (!sb->s_root)
		return -ENOMEM;

	return 0;
}

static struct dentry *pdfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data)
{
	struct dentry *ret;

	ret = mount_bdev(fs_type, flags, dev_name, data, pdfs_fill_super);

	if (unlikely(IS_ERR(ret)))
		printk(KERN_ERR "Error mounting " MOD_NAME "\n");
	else
		printk(KERN_INFO MOD_NAME " successfully mounted on [%s]\n", dev_name);
	return ret;
}

static void pdfs_kill_superblock(struct super_block *sb)
{
	printk(KERN_INFO MOD_NAME " destroyed. Unmount successful.\n");
}

struct file_system_type pdfs_fs_type = {
	.owner = THIS_MODULE,
	.name = MOD_NAME,
	.mount = pdfs_mount,
	.kill_sb = pdfs_kill_superblock
};

static int __init pdfs_init(void)
{
	int ret;

	ret = register_filesystem(&pdfs_fs_type);
	if (likely(ret == 0))
		printk(KERN_INFO "Successfully registered " MOD_NAME "\n");
	else
		printk(KERN_INFO "Failed to register" MOD_NAME "\n");
	return ret;
}

static void pdfs_exit(void)
{
	int ret;

	ret = unregister_filesystem(&pdfs_fs_type);
	if (likely(ret == 0))
		printk(KERN_INFO "Successfully unregistered " MOD_NAME "\n");
	else
		printk(KERN_INFO "Failed to unregister" MOD_NAME "\n");
}

module_init(pdfs_init);
module_exit(pdfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pete Dietl");
