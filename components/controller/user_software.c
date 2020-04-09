#include "user_software.h"
#include "traffic_light.h"
#define QUEUE_SIZE 10


static state light_state = OFF;
signal signalqueue[QUEUE_SIZE] = {0};
static job *timing_job = 0;
static char is_stopped = 0;
static char is_signalled = 0;
static int time_A = 1000;
static int time_B = 1000;
static int time_C = 1000;
static int time_C_ = 1000;
static int time_D = 1000;
static int time_E = 1000;
static int time_F = 1000;
static int time_G = 1000;
static int time_H = 1000;

void enqueue_time_signal()
{
    send_signal(TIME);
}
void enqueue_stopped_signal()
{
    send_signal(STOPPED);
}

static void switch_state(state _state)
{
    light_state = _state;
    switch(light_state)
    {
        case OFF:           red(0); yellow(0); green(0); pred(0); pgreen(0); break;
        case BL_YELLOW:     red(0); yellow(2); green(0); pred(0); pgreen(0); break;
        case YELLOW:        red(0); yellow(1); green(0); pred(1); pgreen(0); break;
        case RED_PR_1:      red(1); yellow(0); green(0); pred(1); pgreen(0); break;
        case RED_PG:        red(1); yellow(0); green(0); pred(0); pgreen(1); break;
        case RED_PG_STOP:   red(1); yellow(0); green(0); pred(0); pgreen(1); break;
        case RED_BL:        red(1); yellow(0); green(0); pred(0); pgreen(2); break;
        case RED_PR_2:      red(0); yellow(0); green(0); pred(1); pgreen(0); break;
        case RED_YELLOW:    red(1); yellow(1); green(0); pred(1); pgreen(0); break;
        case GREEN:         red(0); yellow(0); green(1); pred(1); pgreen(0); break;
        case GREEN_SIGNAL:  red(0); yellow(0); green(1); pred(1); pgreen(0); break;
    }

    if(timing_job) {
        timing_job->valid = 0;
        timing_job = 0;
    }
    switch (light_state) {
        case YELLOW:        timing_job = schedule(time_A, enqueue_time_signal); break;
        case RED_PR_1:      timing_job = schedule(time_B, enqueue_time_signal); break;
        case RED_PG:        timing_job = schedule_or(time_C, enqueue_time_signal, &is_stopped, enqueue_stopped_signal); break;
        case RED_PG_STOP:   timing_job = schedule(time_C_, enqueue_time_signal); break;
        case RED_BL:        timing_job = schedule(time_D, enqueue_time_signal); break;
        case RED_PR_2:      timing_job = schedule(time_E, enqueue_time_signal); break;
        case RED_YELLOW:    timing_job = schedule(time_F, enqueue_time_signal); break;
        case GREEN:         timing_job = schedule_or(time_G, enqueue_time_signal, &is_signalled, enqueue_time_signal); break;
        case GREEN_SIGNAL:  timing_job = schedule(time_H, enqueue_time_signal); break;
    }
}


static void step_until_wait()
{
    for(int i = 0; i < QUEUE_SIZE; ++i) {
        switch(signalqueue[i]) {
            case SWITCH_ON: if(light_state == OFF) switch_state(BL_YELLOW); break;
            case SWITCH_OFF: switch_state(OFF); break;
            case POLICE:
                if(light_state == BL_YELLOW) switch_state(YELLOW);
                else if(light_state != OFF) switch_state(BL_YELLOW);
                break;
            case TIME:
                timing_job = 0;
                switch(light_state) {
                    case YELLOW:        switch_state(RED_PR_1); break;
                    case RED_PR_1:      switch_state(RED_PG); break;
                    case RED_PG:
                    case RED_PG_STOP:   switch_state(RED_BL); break;
                    case RED_BL:        switch_state(RED_PR_2); break;
                    case RED_PR_2:      switch_state(RED_YELLOW); break;
                    case RED_YELLOW:    switch_state(GREEN); break;
                    case GREEN:         switch_state(GREEN_SIGNAL); break;
                    case GREEN_SIGNAL:  switch_state(YELLOW); break;
                }
            default: break;
        }
        signalqueue[i] = NOTHING;
    }
}



state get_state()
{
    return light_state;
}

void set_state(state _state)
{
    switch_state(_state);
}

void send_signal(signal _signal)
{
    for(int i = 0; i < QUEUE_SIZE; ++i) {
        if(!signalqueue[i]) {
            signalqueue[i] = _signal;
            break;
        }
    }
    step_until_wait();

}
