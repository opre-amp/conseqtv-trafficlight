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



#ifdef __cplusplus
}
#endif


#endif