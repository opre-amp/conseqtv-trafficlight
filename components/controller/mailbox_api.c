#include "mailbox_api.h"

#define TX_BUFFER  0x27000000
#define BUFFER_MAX 0x27ffffff

#define MAILBOX_TX_SET 0x400000B8 /* Core 3 Mailbox 2 write-set */
#define MAILBOX_TX_CLR 0x400000F8 /* Core 3 Mailbox 2 read & write-high-to-clear */
#define MAILBOX_RX_CLR 0x400000F4 /* Core 3 Mailbox 1 read & write-high-to-clear */

static volatile byte* tx_base = (void*)TX_BUFFER;

static volatile byte** mailbox_tx_set = (void*) MAILBOX_TX_SET;
static volatile byte** mailbox_tx_clr = (void*) MAILBOX_TX_CLR;
static volatile byte** mailbox_rx_clr = (void*) MAILBOX_RX_CLR;

static volatile byte *last = (void*)TX_BUFFER;
/*
 * Buffers data on the first available slot in the TX buffer.
 */
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