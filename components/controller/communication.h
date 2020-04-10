#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#ifdef __cplusplus
extern "C"{
#endif

/*
 * Handles incoming data when available. Non-blocking.
 */
void handle_incoming();

/*
 * Sending a heartbeat signal to the host every 1 secs.
 */
void send_heartbeat();


#ifdef __cplusplus
}
#endif


#endif