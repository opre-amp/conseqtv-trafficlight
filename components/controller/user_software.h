#ifndef USER_SOFTWARE_H
#define USER_SOFTWARE_H

#include "timer.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
 * The state of the traffic light
 */
typedef enum
{
    OFF          = 1,
    BL_YELLOW    = 2,
    YELLOW       = 3,
    RED_PR_1     = 4,
    RED_PG       = 5,
    RED_PG_STOP  = 6,
    RED_BL       = 7,
    RED_PR_2     = 8,
    RED_YELLOW   = 9,
    GREEN        = 10,
    GREEN_SIGNAL = 11
} state;

/*
 * The input signals of the traffic light
 */
typedef enum
{
    NOTHING = 0,
    SWITCH_ON = 1,
    SWITCH_OFF = 2,
    POLICE = 3,
    TIME = 4,
    STOPPED = 5
} signal;

/*
 * Retrieves the current state
 */
state get_state();

/*
 * Sets the state
 */
void set_state(state _state);

/*
 * send input signal
 */
void send_signal(signal _signal);

/*
 * setters for the static variables [A,H].
 */
void set_time_A(int time);
void set_time_B(int time);
void set_time_C(int time);
void set_time_C_(int time);
void set_time_D(int time);
void set_time_E(int time);
void set_time_F(int time);
void set_time_G(int time);
void set_time_H(int time);

/*
 * getters for the static variables [A,H].
 */
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
 * Call this to update the state of the sensors.
 */
void sample_sensors();

/*
 * Get the state of the buttons
 */
int get_signal();
int get_stopped();

#ifdef __cplusplus
}
#endif


#endif