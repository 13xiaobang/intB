#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>

#define MISC_DEVICE_NAME "misc_node"
#define MISC_DEVICE_MINOR 144

#define PRINT_TAG "misc_demo"
#define MEM_LEN 11 // last byte should be \0
char* mem;
static int misc_open(struct inode* inode, struct file* filep)
{
    printk(KERN_INFO "%s:%s enter\n", PRINT_TAG, __func__);
    return 0;
}

static loff_t misc_llseek(struct file* filp, loff_t offset, int orig)
{
    switch (orig) {
    case SEEK_SET:
        if (offset > strlen(mem)) {
            return -EINVAL;
        }
        filp->f_pos = offset;
        break;
    case SEEK_CUR:
        if (offset + filp->f_pos > strlen(mem))
            return -EINVAL;
        filp->f_pos += offset;
        break;
    default:
        break;
    }
    return filp->f_pos;
}

static ssize_t misc_read(struct file* filp, char __user* buff, size_t cnt, loff_t* offt)
{
    printk(KERN_INFO "%s:%s enter\n", PRINT_TAG, __func__);
    if (*offt >= strlen(mem))
        return 0;
    else {
        if (strlen(mem) - (*offt) > cnt) {
            copy_to_user(buff, mem + (*offt), cnt);
            *offt += cnt;
            return cnt;
        } else {
            int temp_ret = strlen(mem) - (*offt);
            copy_to_user(buff, mem + (*offt), temp_ret);
            *offt = strlen(mem);
            return temp_ret;
        }
    }
}

static ssize_t misc_write(struct file* filp, const char __user* buff, size_t cnt, loff_t* offt)
{
    printk(KERN_INFO "%s:%s enter\n", PRINT_TAG, __func__);
    if (cnt - (*offt) > MEM_LEN - 1) {
        (void)copy_from_user(mem, buff + (*offt), MEM_LEN - 1);
        mem[MEM_LEN - 1] = '\0';
        return MEM_LEN - 1;
    } else {
        copy_from_user(mem, buff + (*offt), cnt - (*offt));
        mem[cnt - (*offt)] = '\0';
        return cnt;
    }
}

static int misc_release(struct inode* inode, struct file* filep)
{
    printk(KERN_INFO "%s:%s enter\n", PRINT_TAG, __func__);
    return 0;
}

static struct file_operations misc_fops = {
    .owner = THIS_MODULE,
    .open = misc_open,
    .read = misc_read,
    .write = misc_write,
    .release = misc_release,
    .llseek = misc_llseek
};

static struct miscdevice miscdevice_test = {
    .minor = MISC_DEVICE_MINOR,
    .name = MISC_DEVICE_NAME,
    .fops = &misc_fops,
};

static __init int misc_init(void)
{
    int ret = misc_register(&miscdevice_test);
    mem = (char*)kmalloc(MEM_LEN, GFP_KERNEL);
    memset(mem, 0, MEM_LEN);
    printk(KERN_INFO "%s:%s enter\n", PRINT_TAG, __func__);
    if (ret < 0)
        return -EFAULT;
    return 0;
}

static __exit void misc_exit(void)
{
    kfree(mem);
    printk(KERN_INFO "%s:%s enter\n", PRINT_TAG, __func__);
    misc_deregister(&miscdevice_test);
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_AUTHOR("zhanghaifeng<zhang_heaven@163.com>");
MODULE_DESCRIPTION("Misc Driver");
MODULE_LICENSE("Dual BSD/GPL");
