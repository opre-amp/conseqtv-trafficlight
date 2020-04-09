#ifndef MAILBOX_API_H
#define MAILBOX_API_H

#ifdef __cplusplus
extern "C"{
#endif


typedef char byte;

/*
 * Send data to the linux host. Maximum 255 bytes at a time.
 * Approx. 16MByte of data can be sent if the host does not process it.
 * @param buffer    Buffer to copy data from
 * @param length    Length of the data
 * @param partial   Set to 0 if only entire data should be sent
 *                  Set to non-0 if partial transmission is also OK
 * @return          Length of the data buffered
 * @note            This function is _non blocking_.
 */
byte send_data(const byte* buffer, byte length, byte partial);

/*
 * Receive data from a linux host. Maximum 255 bytes at a time.
 * @param buffer    Buffer to copy data to
 * @param maxlength Maximal length of the data (size of the buffer)
 * @return          Length of the data received
 * @note            This function is _non blocking_.
 */
byte receive_data(byte* buffer, byte maxlength);


#ifdef __cplusplus
}
#endif

#endif