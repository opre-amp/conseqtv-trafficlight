#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct
{
    char valid;
    unsigned int start;
    unsigned int after;
    volatile char* alternative;
    void (*fp)(void);    
    void (*fpalt)(void);    
} job;

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

/*
 * Schedules a function to be executed after a given time
 * @return      Scheduled job
 */
job* schedule(unsigned int msecs, void (*fp)(void));

/*
 * Schedules a function to be executed after a given time or when a value is no longer 0
 * @return      Scheduled job
 */
job* schedule_or(unsigned int msecs, void (*fp)(void), char* alternative, void(*fpalt)(void));

/*
 * Runs the (over)due jobs in the execution queue.
 */
void run_jobs();

/*
 * Deletes all jobs
 */
void empty_scheduling_queue();

#ifdef __cplusplus
}
#endif

#endif