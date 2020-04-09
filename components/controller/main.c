#include "mailbox_api.h"
#include "user_software.h"
#include "traffic_light.h"

byte stop_flag = 1;

void send_police() {send_signal(POLICE);}
void switch_off() {send_signal(SWITCH_OFF);}
void switch_on() {send_signal(SWITCH_ON);}

void setup()
{
    schedule(1000, switch_on);
    schedule(5000, switch_off);
    schedule(10000, switch_on);
    schedule(15000, send_police);
    schedule(35000, send_police);
}
void loop()
{
    run_jobs();
}

int main()
{
    setup();
    while (stop_flag) {
        loop();
    }
}