#include "user_software.h"
#define QUEUE_SIZE 10

static state light_state = OFF;

signal signalqueue[QUEUE_SIZE] = {0};

void enqueue_time_signal()
{
    send_signal(TIME);
}

static void switch_state(state _state)
{
    light_state = _state;
}

static void step_until_wait()
{
    state last_state = light_state;
    for(int i = 0; i < QUEUE_SIZE; ++i) {
        switch(signalqueue[i]) {
            case SWITCH_ON: if(light_state == OFF) switch_state(BL_YELLOW); break;
            case SWITCH_OFF: switch_state(OFF); break;
            case POLICE:
                if(light_state == BL_YELLOW) switch_state(YELLOW);
                else if(light_state != OFF) switch_state(BL_YELLOW);
                break;
            case TIME:
                switch(light_state) {
                    case YELLOW: switch_state(RED_PR_1); break;
                    case RED_PR_1: switch_state(RED_PG); break;
                    case RED_PG: switch_state(RED_PR_2); break;
                    case RED_PR_2: switch_state(RED_YELLOW); break;
                    case RED_YELLOW: switch_state(GREEN); break;
                    case GREEN: switch_state(YELLOW); break;
                }
            default: break;
        }
        signalqueue[i] = NOTHING;
    }
    if(last_state == light_state) {
        return;
    }
    else{
        empty_scheduling_queue();
    }
    switch (light_state) {
        case YELLOW:    schedule(1000, enqueue_time_signal); break;
        case RED_PR_1:  schedule(2000, enqueue_time_signal); break;
        case RED_PG:    schedule(3000, enqueue_time_signal); break;
        case RED_PR_2:  schedule(4000, enqueue_time_signal); break;
        case RED_YELLOW:schedule(5000, enqueue_time_signal); break;
        case GREEN:     schedule(6000, enqueue_time_signal); break;
    }
}



state get_state()
{
    return light_state;
}

void set_state(state _state)
{
    light_state = _state;
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
