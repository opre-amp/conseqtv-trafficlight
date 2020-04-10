#ifndef MAILBOX_API_H
#define MAILBOX_API_H

#define TX_BUFFER  0x28000000
#define BUFFER_MAX 0x28ffffff
#define MAILBOX_TX_SET 0x400000B4 /* Core 3 Mailbox 1 write-set */
#define MAILBOX_TX_CLR 0x400000F4 /* Core 3 Mailbox 1 read & write-high-to-clear */
#define MAILBOX_RX_SET 0x400000B8 /* Core 3 Mailbox 2 read & write-high-to-clear */
#define MAILBOX_RX_CLR 0x400000F8 /* Core 3 Mailbox 2 write-set (WO) */


typedef char byte;

typedef struct 
{
    char string[255];
    int len;
} sstring;

/*
 * Before and after using the mailboxes, call these functions, otherwise
 * resource leaks are created
 */
void init_ptrs(void);
void uninit_ptrs(void);

sstring get_next_msg(void);

/*
 * Send data to the linux host. Maximum 255 bytes at a time.
 * Approx. 16MByte of data can be sent if the host does not process it.
 * @param buffer    Buffer to copy data from
 * @param length    Length of the data
 * @return          Length of the data buffered
 * @note            This function is _non blocking_.
 */
byte send_data(const byte* buffer, byte length);

/*
 * KThread-compatible function to receive (and print) data from a mbox
 * @param param     not used
 * @return          0 on success
 */
int mbox(void* param);


#endif