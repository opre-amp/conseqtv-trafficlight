#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C"{
#endif

/*
 * Performs busy-waiting for a given amount of time.
 * @param msecs     The number of milliseconds to wait for.
 */
void waitms(unsigned int msecs);

/*
 * Performs busy-waiting for a given amount of time.
 * @param secs      The number of seconds to wait for.
 */
void waits(unsigned int secs);


#ifdef __cplusplus
}
#endif

#endif