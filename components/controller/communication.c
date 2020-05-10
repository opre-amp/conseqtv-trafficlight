#include "communication.h"
#include "mailbox_api.h"
#include "timer.h"
#include "user_software.h"
#include "traffic_light.h"

/*
 * Returns whether two strings are equal on a finite path (len).
 */
static byte streq(char* str1, char* str2, byte len)
{
    for(int i = 0; i < len; ++i) {
        if(str1[i] != str2[i]) return 0;
        else if(str1[i] == 0) return 1;
    }
    return 1;
}


static byte buffer[255];

static char stop[]      = "stop";
static char send[]      = "send";
static char test[]      = "test";
static char recv[]      = "recv";
static char time[]      = "time";
static char btnc[]      = "btnc";
static char btnp[]      = "btnp";
static byte param1_size = 4;

static char red_buf[]   = "car_red";
static char grn[]       = "car_grn";
static char ylw[]       = "car_ylw";
static char pred_buf[]  = "ped_red";
static char pgrn[]      = "ped_grn";
static char police[]    = "police!";
static char switch1[]   = "switch1";
static char switch0[]   = "switch0";
static char tostate[]   = "tostate";
static byte param2_size = 7;

static char stopped[]   = "Stopping program...";
static char recving[]   = "Retrieving state: ??";
static char timeset[]   = "Setting time ? to: ??????";
static char sending[]   = "Sending ???????";
static char setstate[]  = "Sending state to ??";
static char testing[]   = "Testing ???????";
static char resultok[]  = "Test result: ok"; 
static char resultnok[] = "Test result: not ok"; 
static char error  []   = "Error executing:";
static char btnc_res[]  = "Car sensor: ?";
static char btnp_res[]  = "Ped sensor: ?";
static char time_res[]  = "Timing value for ? is ??????";

extern byte stop_flag;
/*
 * Emulates memcpy but in a simple way.
 */
static void copy_to_buffer(char* buffer, char* src, byte len)
{
    for (int i = 0; i < len; ++i) {
        buffer[i] = src[i];
    }
}
/*
 * Is a character numeric?
 */
static byte is_numeric(char n) {
    if(n < '0' || n > '9') return 0;
    return 1;
}
/*
 * Turns a string into an integer.
 */
static int atoi(char* buf) {
    int ret = 0;
    for(;is_numeric(*buf);++buf) ret = ret*10 + (*buf) - '0';
    return ret;
}
/*
 * Handles incoming stimulus; this is the interface from the host's
 * controller.
 * When a valid incoming message is received, an acknowledgement msg is 
 * sent back. When an invalid msg arrives, an error msg is raised.
 */
void handle_incoming()
{
    byte length;
    if((length = receive_data(buffer, sizeof(buffer)))) {
        if (streq(buffer, stop, param1_size)) {
            set_state(OFF);
            stop_flag = 1;
            while(!send_data(stopped, sizeof(stopped)));
            return;
        }
        else if (streq(buffer, recv, param1_size)) {
            recving[18] = get_state()/10 + '0';
            recving[19] = get_state()%10 + '0';
            while(!send_data(recving, sizeof(recving)));
            return;
        }
        else if (streq(buffer, btnc, param1_size)) {
            btnc_res[12] = get_stopped() ? '1' : '0';
            while(!send_data(btnc_res, sizeof(btnc_res)));
            return;
        }
        else if (streq(buffer, btnp, param1_size)) {
            btnp_res[12] = get_signal() ? '1' : '0';
            while(!send_data(btnp_res, sizeof(btnp_res)));
            return;
        }
        else if (streq(buffer, time, param1_size)) {
            timeset[13] = buffer[param1_size + 1];
            if(is_numeric(*(buffer+param1_size+3)))
            {
                int _time = atoi(buffer+param1_size+3);
                timeset[19] = ((_time / 100000) % 10) + '0';
                timeset[20] = ((_time / 10000 ) % 10) + '0';
                timeset[21] = ((_time / 1000  ) % 10) + '0';
                timeset[22] = ((_time / 100   ) % 10) + '0';
                timeset[23] = ((_time / 10    ) % 10) + '0';
                timeset[24] = ((_time / 1     ) % 10) + '0';
                while(!send_data(timeset, sizeof(timeset)));
                switch(timeset[13]) {
                    case 'A': set_time_A(_time); break;
                    case 'B': set_time_B(_time); break;
                    case 'C': set_time_C(_time); break;
                    case 'c': set_time_C_(_time); break;
                    case 'D': set_time_D(_time); break;
                    case 'E': set_time_E(_time); break;
                    case 'F': set_time_F(_time); break;
                    case 'G': set_time_G(_time); break;
                    case 'H': set_time_H(_time); break;
                }
            } 
            else if(*(buffer+param1_size+3) == '?')
            {
                int timing;
                switch(time_res[17] = *(buffer+param1_size+3)) {
                    case 'A': timing = get_time_A(); break;
                    case 'B': timing = get_time_B(); break;
                    case 'C': timing = get_time_C(); break;
                    case 'c': timing = get_time_C_(); break;
                    case 'D': timing = get_time_D(); break;
                    case 'E': timing = get_time_E(); break;
                    case 'F': timing = get_time_F(); break;
                    case 'G': timing = get_time_G(); break;
                    case 'H': timing = get_time_H(); break;
                }
                time_res[22] = ((timing / 100000) % 10) + '0';
                time_res[23] = ((timing / 10000 ) % 10) + '0';
                time_res[24] = ((timing / 1000  ) % 10) + '0';
                time_res[25] = ((timing / 100   ) % 10) + '0';
                time_res[26] = ((timing / 10    ) % 10) + '0';
                time_res[27] = ((timing / 1     ) % 10) + '0';
                while(!send_data(time_res, sizeof(time_res)));
            }
            return;
        }
        else if (streq(buffer, send, param1_size)) {
            if (streq(buffer + param1_size + 1, police, param2_size)) {
                copy_to_buffer(sending + 8, police, param2_size);
                while(!send_data(sending, sizeof(sending)));
                send_signal(POLICE);
                return;
            }
            else if (streq(buffer + param1_size + 1, switch0, param2_size)) {
                copy_to_buffer(sending + 8, switch0, param2_size);
                while(!send_data(sending, sizeof(sending)));
                send_signal(SWITCH_OFF);
                return;
            }
            else if (streq(buffer + param1_size + 1, switch1, param2_size)) {
                copy_to_buffer(sending + 8, switch1, param2_size);
                while(!send_data(sending, sizeof(sending)));
                send_signal(SWITCH_ON);
                return;
            }
            else if (streq(buffer + param1_size + 1, tostate, param2_size) &&
              is_numeric(buffer[param1_size + 1 + param2_size + 1]) &&
              is_numeric(buffer[param1_size + 1 + param2_size + 2])) {
                setstate[17] = buffer[param1_size + 1 + param2_size + 1];
                setstate[18] = buffer[param1_size + 1 + param2_size + 2];
                while(!send_data(setstate, sizeof(setstate)));
                set_state(10*(setstate[17] - '0') + (setstate[18] - '0'));
                return;
            }
        }
        else if (streq(buffer, test, param1_size)) {
            if (streq(buffer + param1_size + 1, red_buf, param2_size)) {
                copy_to_buffer(testing + 8, red_buf, param2_size);
                while(!send_data(testing, sizeof(testing)));
                if (test_red()) while(!send_data(resultok, sizeof(resultok)));
                else while(!send_data(resultnok, sizeof(resultnok)));
                return;
            }
            else if (streq(buffer + param1_size + 1, grn, param2_size)) {
                copy_to_buffer(testing + 8, grn, param2_size);
                while(!send_data(testing, sizeof(testing)));
                if (test_green()) while(!send_data(resultok, sizeof(resultok)));
                else while(!send_data(resultnok, sizeof(resultnok)));
                return;
            }
            else if (streq(buffer + param1_size + 1, ylw, param2_size)) {
                copy_to_buffer(testing + 8, ylw, param2_size);
                while(!send_data(testing, sizeof(testing)));
                if (test_yellow()) while(!send_data(resultok, sizeof(resultok)));
                else while(!send_data(resultnok, sizeof(resultnok)));
                return;
            }
            else if (streq(buffer + param1_size + 1, pred_buf, param2_size)) {
                copy_to_buffer(testing + 8, pred_buf, param2_size);
                while(!send_data(testing, sizeof(testing)));
                if (test_pred()) while(!send_data(resultok, sizeof(resultok)));
                else while(!send_data(resultnok, sizeof(resultnok)));
                return;
            }
            else if (streq(buffer + param1_size + 1, pgrn, param2_size)) {
                copy_to_buffer(testing + 8, pgrn, param2_size);
                while(!send_data(testing, sizeof(testing)));
                if (test_pgreen()) while(!send_data(resultok, sizeof(resultok)));
                else while(!send_data(resultnok, sizeof(resultnok)));
                return;
            }
        }
        while(!send_data(error, sizeof(error)));
        while(!send_data(buffer, length));
    }

}

static char heartbeat[] = "heartbeat";
/*
 * Every 1 sec this sends a "heartbeat" message to the host.
 */
void send_heartbeat()
{
    send_data(heartbeat, sizeof(heartbeat));
    schedule(1000, send_heartbeat);
}