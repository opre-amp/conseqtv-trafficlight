#ifndef MAILBOX_API_H
#define MAILBOX_API_H

#ifdef __cplusplus
extern "C"{
#endif

/*
 * Call these functions before and after using the mailbox API.
 * These return 0 if the operation was successful, or -1 if it was not.
 */

int init_mailbox();
int uninit_mailbox();

/*
 * These functions set up the function pointers of the "listeners"
 * so that they get called when either a heartbeat or an error occurs. 
 */
void register_heartbeat_handler(void(*hndlr)(void));
void register_error_handler(void(*hndlr)(char*));

/*
 * Retrieves the current state
 */
int get_state(char* buf, int len);

/*
 * Sets the state
 */
int set_state(char* buf);

/*
 * send input signal
 */
int send_signal(char* buf);

int set_time_A(int time);
int set_time_B(int time);
int set_time_C(int time);
int set_time_C_(int time);
int set_time_D(int time);
int set_time_E(int time);
int set_time_F(int time);
int set_time_G(int time);
int set_time_H(int time);

int get_time_A();
int get_time_B();
int get_time_C();
int get_time_C_();
int get_time_D();
int get_time_E();
int get_time_F();
int get_time_G();
int get_time_H();
/*
 * These functions test the various leds of the system. They return 1 if
 * the LED is working as expected.
 */
int test_red();
int test_ylw();
int test_grn();
int test_pred();
int test_pgrn();

/*
 * Get the state of the buttons
 */
int get_signal();
int get_stopped();



#ifdef __cplusplus
}
#endif


#endif