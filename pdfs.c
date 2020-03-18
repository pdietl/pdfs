#include <linux/module.h>

#define MOD_NAME "PDFS"

static int __init pdfs_init(void)
{
	printk(KERN_ALERT "Initializing " MOD_NAME "\n");
	return 0;
}

static void pdfs_exit(void)
{
	printk(KERN_ALERT "Unloading " MOD_NAME "\n");
}

module_init(pdfs_init);
module_exit(pdfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pete Dietl");
