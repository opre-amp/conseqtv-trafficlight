#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#ifdef __cplusplus
extern "C"{
#endif

/*
 * These functions set the state of various leds of the system
 * @param state     0: off
 *                  1: on
 *                  2: blinking (only valid for yellow and pgreen)
 */

void red(char state);
void yellow(char state);
void green(char state);
void pred(char state);
void pgreen(char state);

/*
 * Returns 1 if the pedestrian signal is pressed, 0 otherwise
 */
int pedestrian_signal();

/*
 * Returns 1 if there is a car waiting, 0 otherwise
 */
int stopped_car();

#ifdef __cplusplus
}
#endif


#endif