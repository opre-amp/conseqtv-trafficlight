#ifndef IOCTL_OPS_H
#define IOCTL_OPS_H

#include <linux/io.h>

/*
 * Before and after using the ioctl ops, call these functions
 */
int init_ioctl(void);
void uninit_ioctl(void);

static int mbox_open(struct inode *inode, struct file *file);
static int mbox_release(struct inode *inode, struct file *file);
static ssize_t mbox_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t mbox_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long mbox_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif