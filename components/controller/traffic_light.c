#include "traffic_light.h"
#include "mailbox_api.h"
#include "timer.h"

char red_on[] = "red on";
char red_off[] = "red off";

char yellow_on[] = "yellow on";
char yellow_off[] = "yellow off";

char green_on[] = "green on";
char green_off[] = "green off";

char pred_on[] = "pedestrian red on";
char pred_off[] = "pedestrian red off";

char pgreen_on[] = "pedestrian green on";
char pgreen_off[] = "pedestrian green off";

job* blinking_yellow = (void*)0;
job* blinking_pgreen = (void*)0;

void red(char state)
{
    if(state)  while(!send_data(red_on, sizeof(red_on)));
    if(!state) while(!send_data(red_off, sizeof(red_off)));
}

void blink_down_yellow();
void blink_up_yellow()
{
    while(!send_data(yellow_on, sizeof(yellow_on)));
    blinking_yellow = schedule(500, blink_down_yellow);
}
void blink_down_yellow()
{
    while(!send_data(yellow_off, sizeof(yellow_off)));
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
    if(state == 1)  while(!send_data(yellow_on, sizeof(yellow_on)));
    if(!state) while(!send_data(yellow_off, sizeof(yellow_off)));
}


void green(char state)
{
    if(state)  while(!send_data(green_on, sizeof(green_on)));
    if(!state) while(!send_data(green_off, sizeof(green_off)));
}


void pred(char state)
{
    if(state)  while(!send_data(pred_on, sizeof(pred_on)));
    if(!state) while(!send_data(pred_off, sizeof(pred_off)));
}


void blink_down_pgreen();
void blink_up_pgreen()
{
    while(!send_data(pgreen_on, sizeof(pgreen_on)));
    blinking_pgreen = schedule(250, blink_down_pgreen);
}
void blink_down_pgreen()
{
    while(!send_data(pgreen_off, sizeof(pgreen_off)));
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
    if(state == 1)  while(!send_data(pgreen_on, sizeof(pgreen_on)));
    if(!state) while(!send_data(pgreen_off, sizeof(pgreen_off)));
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
    return 0;
}

int stopped_car()
{
    return 0;
}