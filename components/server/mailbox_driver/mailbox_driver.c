#include <linux/module.h>
#include <linux/delay.h>
#include <linux/io.h>

#include "mailbox_api.h"
#include "ioctl_ops.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Levente Bajczi");
MODULE_DESCRIPTION("Receives and sends data via mailboxes");
MODULE_VERSION("0.01");

static int __init mailbox_driver_init(void)
{
    init_ptrs();
    return init_ioctl();
}

static void __exit mailbox_driver_exit(void)
{
    uninit_ptrs();
    uninit_ioctl();
}

module_init(mailbox_driver_init);
module_exit(mailbox_driver_exit);