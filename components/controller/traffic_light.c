#include "traffic_light.h"
#include "mailbox_api.h"
#include "timer.h"
#include "gpio.h"

static int grn_gpio = 1;
static int ylw_gpio = 2;
static int red_gpio = 3;
static int pgrn_gpio = 14;
static int pred_gpio = 15;
static int stopped = 11;
static int signal = 12;



job* blinking_yellow = (void*)0;
job* blinking_pgreen = (void*)0;

void red(char state)
{
    write_gpio(red_gpio, state);
}

void blink_down_yellow();
void blink_up_yellow()
{
    write_gpio(ylw_gpio, 1);
    blinking_yellow = schedule(500, blink_down_yellow);
}
void blink_down_yellow()
{
    write_gpio(ylw_gpio, 0);
    blinking_yellow = schedule(500, blink_up_yellow);
}
void yellow(char state)
{
    if(blinking_yellow) {
        blinking_yellow->valid = 0;
        blinking_yellow = (void*)0;
    }
    if(state == 2) {
        blinking_yellow = schedule(500, blink_up_yellow);
    }
    else write_gpio(ylw_gpio, state);
}


void green(char state)
{
    write_gpio(grn_gpio, state);
}


void pred(char state)
{
    write_gpio(pred_gpio, state);
}


void blink_down_pgreen();
void blink_up_pgreen()
{
    write_gpio(pgrn_gpio, 1);
    blinking_pgreen = schedule(250, blink_down_pgreen);
}
void blink_down_pgreen()
{
    write_gpio(pgrn_gpio, 0);
    blinking_pgreen = schedule(250, blink_up_pgreen);
}
void pgreen(char state)
{
    if(blinking_pgreen) {
        blinking_pgreen->valid = 0;
        blinking_pgreen = (void*)0;
    }
    if(state == 2) {
        blinking_pgreen = schedule(250, blink_up_pgreen);
    }
    else write_gpio(pgrn_gpio, state);

}


char test_red()
{
    return 1;
}
char test_yellow()
{
    return 1;

}
char test_green()
{
    return 1;

}
char test_pred()
{
    return 1;

}
char test_pgreen()
{
    return 1;

}


int pedestrian_signal()
{
    return read_gpio(signal);
}

int stopped_car()
{
    return read_gpio(stopped);
}