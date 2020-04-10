#include <linux/io.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/mutex.h>

#include "mailbox_api.h"

char flag = 1;
volatile byte* tx_base;
volatile byte** mailbox_tx_set;
volatile byte** mailbox_tx_clr;
struct task_struct* mbox_task;
struct mutex list_mtx;

void init_ptrs()
{
    mutex_init(&list_mtx);
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

struct msgs_buffer_member {
     sstring msg;
     struct msgs_buffer_member* next;
};
typedef struct msgs_buffer_member msgs_buffer_member;

msgs_buffer_member* head = NULL;
msgs_buffer_member* tail = NULL;

static void add_msg(msgs_buffer_member* str)
{
    printk(KERN_INFO "LIST: Adding message %s\n", str->msg.string);
    mutex_lock(&list_mtx);
    if(!head) {
        head = str;
        tail = str;
    }
    else{
        tail->next = str;
        tail = str;
    }
    mutex_unlock(&list_mtx);
}

sstring get_next_msg(void)
{
    sstring ret;
    msgs_buffer_member* tmp;
    mutex_lock(&list_mtx);
    if(head) {
        
        ret = head->msg;

        tmp = head;
        if(tail == head) tail = NULL;
        head = head->next;
        printk(KERN_INFO "LIST: Retrieving message %s (length: %d)\n", ret.string, ret.len);

        kfree(tmp);
    }
    else {
        ret.len = 0;
        ret.string[0] = 0;
    }
    mutex_unlock(&list_mtx);
    return ret;
}

int mbox(void* param)
{
    volatile unsigned int *set = (unsigned int *)ioremap(MAILBOX_RX_SET, 4);
    volatile unsigned int *clr = (unsigned int *)ioremap(MAILBOX_RX_CLR, 4);
    volatile char* data;
    int i;
    volatile char *length;
    msgs_buffer_member* msg_member;

    printk(KERN_INFO "Starting thread...\n");
    
    while (flag) {
        while(!(data = (void*)*clr) && flag);
        if(!flag) break;
        *clr = 0xFFFFFFFF;
        length = ioremap((unsigned int)data, 1);
        data = ioremap((unsigned int)data+1, *length);
        msg_member = kmalloc(sizeof(msgs_buffer_member), GFP_KERNEL);
        msg_member->next = NULL;
        msg_member->msg.len = *length;
        for(i = 0; i < *length; ++i)
            msg_member->msg.string[i] = data[i];
        msg_member->msg.string[(unsigned int)*length] = 0;
        printk(KERN_INFO "Received data: %s (length %d)\n", msg_member->msg.string, *length);
        add_msg(msg_member);
        *length = 0;
        iounmap(data);
        iounmap(length);
    }
    printk(KERN_INFO "Shutting down thread...\n");
    iounmap(clr);
    iounmap(set);
    return 0;
}

volatile byte *last = (void*)TX_BUFFER;

static volatile byte* buffer_data(const byte* buffer, byte length)
{
    volatile byte *ptr;
    int i;
    
    for(ptr = tx_base; ptr <= last; ptr+=256) {
        if(*ptr == 0) break;
    }
    if(ptr > last) last = ptr;
    
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