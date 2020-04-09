#include "mailbox_api.h"

#define TX_BUFFER 0x27000000
#define BUFFER_SIZE 0xffffff

#define MAILBOX_TX_SET 0x400000B8 /* Core 3 Mailbox 2 write-set */
#define MAILBOX_TX_CLR 0x400000F8 /* Core 3 Mailbox 2 read & write-high-to-clear */
#define MAILBOX_RX_CLR 0x400000F4 /* Core 3 Mailbox 1 read & write-high-to-clear */

static volatile byte* tx_base = (void*)TX_BUFFER;
static volatile byte* tx_ptr  = (void*)TX_BUFFER;

static volatile byte** mailbox_tx_set = (void*) MAILBOX_TX_SET;
static volatile byte** mailbox_tx_clr = (void*) MAILBOX_TX_CLR;
static volatile byte** mailbox_rx_clr = (void*) MAILBOX_RX_CLR;


static volatile byte* buffer_data(const byte* buffer, byte* length)
{
    volatile byte* ret;
    int i;
    
    if(!*tx_base)
        tx_ptr = tx_base;
    
    if(*length > tx_base + BUFFER_SIZE - tx_ptr + 1)
        *length = tx_base + BUFFER_SIZE - tx_ptr + 1;

    tx_ptr[0] = *length;
    for(i = 0; i<*length; ++i)
        tx_ptr[i+1] = buffer[i];

    ret = tx_ptr;
    tx_ptr += *length + 1; 
    return ret;

}


byte send_data(const byte* buffer, byte length, byte partial)
{
    volatile byte* base;
    byte init_length = length;
    if(*mailbox_tx_clr) return 0;
    base = buffer_data(buffer, &length);
    if(init_length != length && !partial) return 0; 
    *mailbox_tx_set = base;
    return length;
}


byte receive_data(byte* buffer, byte maxlength)
{
    volatile byte* data;
    int i;
    if ((data = *mailbox_rx_clr)) {
        maxlength = (*data <= maxlength ? *mailbox_rx_clr = (void*)0xFFFFFFFF, *data : maxlength);
        for(i = 0; i < maxlength; ++i)  
            buffer[i] = data[i+1];
        data[0] = 0;
        return maxlength;
    }
    return 0;
}