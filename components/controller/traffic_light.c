#include "traffic_light.h"
#include "mailbox_api.h"
#include "user_software.h"
#include "timer.h"
#include "gpio.h"

static int grn_gpio = 1;
static int ylw_gpio = 2;
static int red_gpio = 3;
static int pgrn_gpio = 14;
static int pred_gpio = 15;
static int stopped_btn = 11;
static int signal_btn = 12;
static int tst_grn = 6;
static int tst_ylw = 7;
static int tst_red = 8;
static int tst_pgrn = 9;
static int tst_pred = 10;



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
    int off, on;
    if(get_state() != OFF) return 0;
    write_gpio(red_gpio, 0);
    off = read_gpio(tst_red);
    write_gpio(red_gpio, 0);
    on = read_gpio(tst_red);
    return (!off && on);
}
char test_yellow()
{
    int off, on;
    if(get_state() != OFF) return 0;
    write_gpio(ylw_gpio, 0);
    off = read_gpio(tst_ylw);
    write_gpio(ylw_gpio, 0);
    on = read_gpio(tst_ylw);
    return (!off && on);

}
char test_green()
{
    int off, on;
    if(get_state() != OFF) return 0;
    write_gpio(grn_gpio, 0);
    off = read_gpio(tst_grn);
    write_gpio(grn_gpio, 0);
    on = read_gpio(tst_grn);
    return (!off && on);

}
char test_pred()
{
    int off, on;
    if(get_state() != OFF) return 0;
    write_gpio(pred_gpio, 0);
    off = read_gpio(tst_pred);
    write_gpio(pred_gpio, 0);
    on = read_gpio(tst_pred);
    return (!off && on);

}
char test_pgreen()
{
    int off, on;
    if(get_state() != OFF) return 0;
    write_gpio(pgrn_gpio, 0);
    off = read_gpio(tst_pgrn);
    write_gpio(pgrn_gpio, 0);
    on = read_gpio(tst_pgrn);
    return (!off && on);
}


int pedestrian_signal()
{
    return read_gpio(signal_btn);
}

int stopped_car()
{
    return read_gpio(stopped_btn);
}