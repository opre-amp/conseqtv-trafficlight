#include <linux/io.h>
#include <linux/kthread.h>

#include "mailbox_api.h"

char flag = 1;
volatile byte* tx_base;
volatile byte** mailbox_tx_set;
volatile byte** mailbox_tx_clr;
struct task_struct* mbox_task;

void init_ptrs()
{
    tx_base = ioremap(TX_BUFFER, 10000);
    mailbox_tx_set = ioremap(MAILBOX_TX_SET, 4);
    mailbox_tx_clr = ioremap(MAILBOX_TX_CLR, 4);
    mbox_task = kthread_run(mbox, NULL, "Receiving data from a mailbox");
}

void uninit_ptrs()
{
    flag = 0;
    kthread_stop(mbox_task);
    iounmap(mailbox_tx_clr);
    iounmap(mailbox_tx_set);
    iounmap(tx_base);
}

void set_flag(char flag);

int mbox(void* param)
{
    volatile unsigned int *set = (unsigned int *)ioremap(MAILBOX_RX_SET, 4);
    volatile unsigned int *clr = (unsigned int *)ioremap(MAILBOX_RX_CLR, 4);
    volatile char* data;
    int i;
    volatile char *length;
    char buff[100];

    while (flag) {
        while(!(data = (void*)*clr) && flag);
        if(!flag) break;
        *clr = 0xFFFFFFFF;
        length = ioremap((unsigned int)data, 1);
        data = ioremap((unsigned int)data+1, *length);
        for(i = 0; i < *length; ++i)
            buff[i] = data[i];
        buff[(unsigned int)*length] = 0;
        printk(KERN_INFO "Received data: %s (length %d)\n", buff, *length);
        *length = 0;
        iounmap(data);
        iounmap(length);
    }
    printk(KERN_INFO "Shutting down thread...\n");
    iounmap(clr);
    iounmap(set);
    return 0;
}

static volatile byte* buffer_data(const byte* buffer, byte length)
{
    volatile byte *ptr;
    int i;
    
    for(ptr = tx_base; ptr <= (byte*)(BUFFER_MAX - 255); ptr+=256) {
        if(*ptr == 0) break;
    }
    if(*ptr) {
        return 0;
    }
    ptr[0] = length;
    for(i = 0; i<length; ++i)
        ptr[i+1] = buffer[i];

    return ptr;
}

byte send_data(const byte* buffer, byte length)
{
    volatile byte* base;
    if(*mailbox_tx_clr) return 0;
    base = buffer_data(buffer, length);
    if(!base) return 0;
    printk(KERN_INFO "Sending data %s (length %d)\n", buffer, *base);
    *mailbox_tx_set = (byte*)(base - tx_base + TX_BUFFER);
    return length;
}