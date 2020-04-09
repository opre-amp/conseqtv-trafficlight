#include "user_software.h"
#include "traffic_light.h"
#define QUEUE_SIZE 10


static state light_state = OFF;

signal signalqueue[QUEUE_SIZE] = {0};

void enqueue_time_signal()
{
    send_signal(TIME);
}
#include "mailbox_api.h"
char buffer[] = "RED_YELLOW -> RED_YELLOW";

static job *timing_job = 0;


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
        case RED_BL:        red(1); yellow(0); green(0); pred(0); pgreen(2); break;
        case RED_PR_2:      red(0); yellow(0); green(0); pred(1); pgreen(0); break;
        case RED_YELLOW:    red(1); yellow(1); green(0); pred(1); pgreen(0); break;
        case GREEN:         red(0); yellow(0); green(1); pred(1); pgreen(0); break;
    }

    if(timing_job) {
        timing_job->valid = 0;
        timing_job = 0;
    }
    char scheduling[] = "Scheduling timer...";
    while(!send_data(scheduling, sizeof(scheduling), 0));
    switch (light_state) {
        case YELLOW:    timing_job = schedule(3000, enqueue_time_signal); break;
        case RED_PR_1:  timing_job = schedule(3000, enqueue_time_signal); break;
        case RED_PG:    timing_job = schedule(3000, enqueue_time_signal); break;
        case RED_BL:    timing_job = schedule(3000, enqueue_time_signal); break;
        case RED_PR_2:  timing_job = schedule(3000, enqueue_time_signal); break;
        case RED_YELLOW:timing_job = schedule(3000, enqueue_time_signal); break;
        case GREEN:     timing_job = schedule(3000, enqueue_time_signal); break;
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
                    case RED_PG:        switch_state(RED_BL); break;
                    case RED_BL:        switch_state(RED_PR_2); break;
                    case RED_PR_2:      switch_state(RED_YELLOW); break;
                    case RED_YELLOW:    switch_state(GREEN); break;
                    case GREEN:         switch_state(YELLOW); break;
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
