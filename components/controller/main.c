#include "mailbox_api.h"
#include "user_software.h"
#include "traffic_light.h"
#include "communication.h"

byte stop_flag = 0;

void send_police() {send_signal(POLICE);}
void switch_off() {send_signal(SWITCH_OFF);}
void switch_on() {send_signal(SWITCH_ON);}

void setup()
{
    send_heartbeat();
}

void loop()
{
    handle_incoming();
    run_jobs();
}

int main()
{
    setup();
    while (!stop_flag) {
        loop();
    }
}