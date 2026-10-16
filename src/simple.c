#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#define PROC_NAME "hello"
#define BUFFER_SIZE 128

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;
    rv = sprintf(buffer, "Hello World\n");

    if (copy_to_user(usr_buf, buffer, rv)) {
        return -EFAULT;
    }

    return rv;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops proc_ops = {
    .proc_read = proc_read,
};
#else
static const struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};
#endif

static int __init simple_init(void)
{
    printk(KERN_INFO "Loading Kernel Module\n");
    printk(KERN_INFO "GOLDEN_RATIO_PRIME: %lu\n", GOLDEN_RATIO_PRIME);
    printk(KERN_INFO "gcd(3300, 24): %lu\n", gcd(3300, 24));
    printk(KERN_INFO "jiffies: %lu, HZ: %d\n", jiffies, HZ);

    proc_create(PROC_NAME, 0666, NULL, &proc_ops);

    return 0;
}

static void __exit simple_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "jiffies on removal: %lu\n", jiffies);
    printk(KERN_INFO "Removing Kernel Module\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("Earl Justine De La Cruz");
