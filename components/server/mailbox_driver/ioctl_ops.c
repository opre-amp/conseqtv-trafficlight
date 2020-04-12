#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include "ioctl_ops.h"
#include "mailbox_api.h"

#define WRITE_SSTRING _IOW('c','l',sstring*)
#define READ_SSTRING  _IOR('c','z',char*)
#define DEVICE_NAME   "mbox"


static dev_t first;         // Global variable for the first device number 
static struct cdev c_dev;   // Global variable for the character device structure
static struct class *cl;    // Global variable for the device class
int open_count = 0;

static int mbox_open(struct inode *inode, struct file *file);
static int mbox_release(struct inode *inode, struct file *file);
static ssize_t mbox_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t mbox_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long mbox_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = mbox_read,
        .write          = mbox_write,
        .open           = mbox_open,
        .unlocked_ioctl = mbox_ioctl,
        .release        = mbox_release,
};

int init_ioctl(void)
{
    if (alloc_chrdev_region(&first, 0, 1, DEVICE_NAME) < 0)
    {
        return -1;
    }
    if ((cl = class_create(THIS_MODULE, DEVICE_NAME)) == NULL)
    {
        unregister_chrdev_region(first, 1);
        return -1;
    }
    if (device_create(cl, NULL, first, NULL, DEVICE_NAME) == NULL)
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    cdev_init(&c_dev, &fops);
    if (cdev_add(&c_dev, first, 1) == -1)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    return 0;

}
void uninit_ioctl(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
}

static int mbox_open(struct inode *inode, struct file *file)
{
    if(!open_count) enable_logging();
    ++open_count;
    return 0;
}
 
static int mbox_release(struct inode *inode, struct file *file)
{
    --open_count;
    if(!open_count) disable_logging();
    return 0;
}
 
static ssize_t mbox_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    return -EINVAL;
}
static ssize_t mbox_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    return -EINVAL;
}
 
static long mbox_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    sstring buf;
    switch(cmd) {
        case WRITE_SSTRING:
            if(copy_from_user(&buf ,(sstring*) arg, sizeof(sstring))) return -1;
            while(!send_data(buf.string, buf.len));
            break;
        case READ_SSTRING:
            buf = get_next_msg();
            if(buf.len == 0 || copy_to_user((char*) arg, buf.string, buf.len)) return -1;
            break;
    }
    return 0;
}