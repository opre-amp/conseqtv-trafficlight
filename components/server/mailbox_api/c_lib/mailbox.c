#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>
#include <stdarg.h>

#include "mailbox.h"

typedef struct 
{
    char string[255];
    int len;
} sstring;

#define WRITE_SSTRING _IOW('c','l',sstring*)
#define READ_SSTRING  _IOR('c','z',char*)

/*
 * The state of the traffic light
 */
typedef enum
{
    ERR          = 0,
    OFF          = 1,
    BL_YELLOW    = 2,
    YELLOW       = 3,
    RED_PR_1     = 4,
    RED_PG       = 5,
    RED_PG_STOP  = 6,
    RED_BL       = 7,
    RED_PR_2     = 8,
    RED_YELLOW   = 9,
    GREEN        = 10,
    GREEN_SIGNAL = 11
} state;

/*
 * The input signals of the traffic light
 */
typedef enum
{
    NOTHING = 0,
    SWITCH_ON = 1,
    SWITCH_OFF = 2,
    POLICE = 3,
    TIME = 4,
    STOPPED = 5
} signal;


static char stop[]      = "stop";
static char send[]      = "send";
static char test[]      = "test";
static char recv[]      = "recv";
static char time_buf[]  = "time";

static char red_buf[]   = "car_red";
static char grn[]       = "car_grn";
static char ylw[]       = "car_ylw";
static char pred_buf[]  = "ped_red";
static char pgrn[]      = "ped_grn";
static char police[]    = "police!";
static char switch1[]   = "switch1";
static char switch0[]   = "switch0";
static char tostate[]   = "tostate";

static char stopped[]   = "Stopping program...";
static char recving[]   = "Retrieving state: %02d";
static char timeset[]   = "Setting time %c to: %06d";
static char sending[]   = "Sending %5s";
static char setstate[]  = "Sending state to %02d";
static char testing[]   = "Testing %7s";
static char resultok[]  = "Test result: ok"; 
static char resultnok[] = "Test result: not ok"; 
static char error  []   = "Error executing:";
static char heartbeat[]  = "heartbeat";

mtx_t ioctl_mtx;

cnd_t get_state_cnd;
cnd_t set_state_cnd;
cnd_t send_signal_cnd;
cnd_t set_time_A_cnd;
cnd_t set_time_B_cnd;
cnd_t set_time_C_cnd;
cnd_t set_time_C__cnd;
cnd_t set_time_D_cnd;
cnd_t set_time_E_cnd;
cnd_t set_time_F_cnd;
cnd_t set_time_G_cnd;
cnd_t set_time_H_cnd;
cnd_t test_red_cnd;
cnd_t test_ylw_cnd;
cnd_t test_grn_cnd;
cnd_t test_pred_cnd;
cnd_t test_pgrn_cnd;

volatile int setstate_scanned_param;
volatile int getstate_scanned_param;
volatile char send_signal_scanned_param[8];
volatile int set_time_A_scanned_param;
volatile int set_time_B_scanned_param;
volatile int set_time_C_scanned_param;
volatile int set_time_C__scanned_param;
volatile int set_time_D_scanned_param;
volatile int set_time_E_scanned_param;
volatile int set_time_F_scanned_param;
volatile int set_time_G_scanned_param;
volatile int set_time_H_scanned_param;
volatile int test_red_scanned_param;
volatile int test_ylw_scanned_param;
volatile int test_grn_scanned_param;
volatile int test_pred_scanned_param;
volatile int test_pgrn_scanned_param;

static int fd;
static void(*error_hndlr)(char*) = NULL;
static void(*heartbeat_hndlr)(void) = NULL;

/*
 * This function tries to read the output of the controller and will 
 * unblock any waiting functions.
 */
static int reader(void* param);
thrd_t reader_thread;
char flag = 1;

int init_mailbox()
{
    mtx_init(&ioctl_mtx, mtx_plain);
    cnd_init(&get_state_cnd);
    cnd_init(&set_state_cnd);
    fd = open("/dev/mbox", O_RDWR);
    if(fd < 0) {
        return -1;
    }
    thrd_create(&reader_thread, reader, &flag);
    return 0;
}
int uninit_mailbox()
{
    flag = 0;
    thrd_join(reader_thread, NULL);
    if(fd >= 0) close(fd);
    mtx_destroy(&ioctl_mtx);
    cnd_destroy(&get_state_cnd);
    cnd_destroy(&set_state_cnd);
}

void register_heartbeat_handler(void(*hndlr)(void))
{
    heartbeat_hndlr = hndlr;
}
void register_error_handler(void(*hndlr)(char*))
{
    error_hndlr = hndlr;
}



static int reader(void* param)
{
    int status = -1;
    char buf[255];

    char time_c;
    int time_value;
    
    char testing_buf[8] = {0};

    int error_indicator = 0;

    volatile char* flag = param;
    while(*flag) {
        status = -1;
        while(status < 0) {
            mtx_lock(&ioctl_mtx);
            status = ioctl(fd, READ_SSTRING, (char*) &buf);
            mtx_unlock(&ioctl_mtx);
            if(!*flag) return 0;
        }
        if(error_indicator) {
            if(error_hndlr) error_hndlr(buf);
            error_indicator = 0;
        }
        else if(!strcmp(buf, heartbeat)) {
            if(heartbeat_hndlr) heartbeat_hndlr();
        }
        else if(!strcmp(buf, error)) {
            error_indicator = 1;
        }
        else if(sscanf(buf, recving, &setstate_scanned_param) == 1) {
            cnd_broadcast(&get_state_cnd);
        }
        else if(sscanf(buf, setstate, &getstate_scanned_param) == 1) {
            cnd_broadcast(&set_state_cnd);
        }
        else if(sscanf(buf, sending, send_signal_scanned_param) == 1) {
            cnd_broadcast(&send_signal_cnd);
        }
        else if(sscanf(buf, timeset, &time_c, &time_value) == 2) {
            switch(time_c) {
                case 'A': set_time_A_scanned_param = time_value; cnd_broadcast(&set_time_A_cnd); break;
                case 'B': set_time_B_scanned_param = time_value; cnd_broadcast(&set_time_B_cnd); break;
                case 'C': set_time_C_scanned_param = time_value; cnd_broadcast(&set_time_C_cnd); break;
                case 'c': set_time_C__scanned_param = time_value; cnd_broadcast(&set_time_C__cnd); break;
                case 'D': set_time_D_scanned_param = time_value; cnd_broadcast(&set_time_D_cnd); break;
                case 'E': set_time_E_scanned_param = time_value; cnd_broadcast(&set_time_E_cnd); break;
                case 'F': set_time_F_scanned_param = time_value; cnd_broadcast(&set_time_F_cnd); break;
                case 'G': set_time_G_scanned_param = time_value; cnd_broadcast(&set_time_G_cnd); break;
                case 'H': set_time_H_scanned_param = time_value; cnd_broadcast(&set_time_H_cnd); break;
                default: break;
            }
        }
        else if(sscanf(buf, testing, testing_buf) == 1) {}
        else if(testing_buf[0] && !strcmp(buf, resultok)) {
            if(!strcmp(testing_buf, red_buf)) { test_red_scanned_param = 1; cnd_broadcast(&test_red_cnd); }
            else if(!strcmp(testing_buf, ylw)) { test_ylw_scanned_param = 1; cnd_broadcast(&test_ylw_cnd); }
            else if(!strcmp(testing_buf, grn)) { test_grn_scanned_param = 1; cnd_broadcast(&test_grn_cnd); }
            else if(!strcmp(testing_buf, pred_buf)) { test_pred_scanned_param = 1; cnd_broadcast(&test_pred_cnd); }
            else if(!strcmp(testing_buf, pgrn)) { test_pgrn_scanned_param = 1; cnd_broadcast(&test_pgrn_cnd); }
            testing_buf[0] = 0;
        }
        else if(testing_buf[0] && !strcmp(buf, resultnok)) {
            if(!strcmp(testing_buf, red_buf)) { test_red_scanned_param = 0; cnd_broadcast(&test_red_cnd); }
            else if(!strcmp(testing_buf, ylw)) { test_ylw_scanned_param = 0; cnd_broadcast(&test_ylw_cnd); }
            else if(!strcmp(testing_buf, grn)) { test_grn_scanned_param = 0; cnd_broadcast(&test_grn_cnd); }
            else if(!strcmp(testing_buf, pred_buf)) { test_pred_scanned_param = 0; cnd_broadcast(&test_pred_cnd); }
            else if(!strcmp(testing_buf, pgrn)) { test_pgrn_scanned_param = 0; cnd_broadcast(&test_pgrn_cnd); }
            testing_buf[0] = 0;
        }
        
    }
    return 0;
}


static int call_async(cnd_t* cond_var, int sec, sstring param)
{
    struct timespec ts;
    int ret;
    mtx_lock(&ioctl_mtx);
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += sec;
    ioctl(fd, WRITE_SSTRING, &param);
    ret = cnd_timedwait(cond_var, &ioctl_mtx, &ts);
    mtx_unlock(&ioctl_mtx);

    return ret == thrd_success;
}

static sstring create_sstring(int count, ...)
{
    sstring buf;
    char* ptr = buf.string;
    va_list ap;
    va_start(ap, count);
    for(int i = 0; i < count; ++i) {
        sprintf(ptr, va_arg(ap, char*));
        ptr = buf.string + strlen(buf.string);
        ptr[0] = ' ';
        ++ptr;
    } 
    buf.len=strlen(buf.string);
    buf.string[buf.len-1] = 0;
    return buf;
}


int get_state(char* buf, int len)
{
    int ret = call_async(&get_state_cnd, 1, create_sstring(1, recv));
    if(!ret) return -1;
    switch(setstate_scanned_param) {
        case ERR:          if(strlen("ERR") < len) { strcpy(buf, "ERR"); } else { return -1; } break;
        case OFF:          if(strlen("OFF") < len) { strcpy(buf, "OFF"); } else { return -1; } break;
        case BL_YELLOW:    if(strlen("BL_YELLOW") < len) { strcpy(buf, "BL_YELLOW"); } else { return -1; } break;
        case YELLOW:       if(strlen("YELLOW") < len) { strcpy(buf, "YELLOW"); } else { return -1; } break;
        case RED_PR_1:     if(strlen("RED_PR_1") < len) { strcpy(buf, "RED_PR_1"); } else { return -1; } break;
        case RED_PG:       if(strlen("RED_PG") < len) { strcpy(buf, "RED_PG"); } else { return -1; } break;
        case RED_PG_STOP:  if(strlen("RED_PG_STOP") < len) { strcpy(buf, "RED_PG_STOP"); } else { return -1; } break;
        case RED_BL:       if(strlen("RED_BL") < len) { strcpy(buf, "RED_BL"); } else { return -1; } break;
        case RED_PR_2:     if(strlen("RED_PR_2") < len) { strcpy(buf, "RED_PR_2"); } else { return -1; } break;
        case RED_YELLOW:   if(strlen("RED_YELLOW") < len) { strcpy(buf, "RED_YELLOW"); } else { return -1; } break;
        case GREEN:        if(strlen("GREEN") < len) { strcpy(buf, "GREEN"); } else { return -1; } break;
        case GREEN_SIGNAL: if(strlen("GREEN_SIGNAL") < len) { strcpy(buf, "GREEN_SIGNAL"); } else { return -1; } break;
    }
    return 0;
}


int set_state(char* buf)
{
    int state_id, ret;
    char number_buf[3] = "xx";
    if(!strcmp("ERR", buf)) { state_id = ERR; }
    else if(!strcmp("OFF", buf)) { state_id = OFF; }
    else if(!strcmp("BL_YELLOW", buf)) { state_id = BL_YELLOW; }
    else if(!strcmp("YELLOW", buf)) { state_id = YELLOW; }
    else if(!strcmp("RED_PR_1", buf)) { state_id = RED_PR_1; }
    else if(!strcmp("RED_PG", buf)) { state_id = RED_PG; }
    else if(!strcmp("RED_PG_STOP", buf)) { state_id = RED_PG_STOP; }
    else if(!strcmp("RED_BL", buf)) { state_id = RED_BL; }
    else if(!strcmp("RED_PR_2", buf)) { state_id = RED_PR_2; }
    else if(!strcmp("RED_YELLOW", buf)) { state_id = RED_YELLOW; }
    else if(!strcmp("GREEN", buf)) { state_id = GREEN; }
    else if(!strcmp("GREEN_SIGNAL", buf)) { state_id = GREEN_SIGNAL; }
    else return -1;
    sprintf(number_buf, "%02d", state_id);

    ret = call_async(&set_state_cnd, 1, create_sstring(3, send, tostate, number_buf));
    if(!ret || getstate_scanned_param != state_id) return -1;
    return 0;
}

int send_signal(char* buf)
{
    int ret;
    ret = call_async(&send_signal_cnd, 1, create_sstring(2, send, buf));
    if(!ret || strcmp(buf, (char*)send_signal_scanned_param)) return -1;
    return 0;
}

int set_time_A(int time)
{
    if(time / 1000000) return -1;
    int ret;
    char time_buf[9];
    sprintf(time_buf, "A %06d", time);
    ret = call_async(&set_time_A_cnd, 1, create_sstring(2, time, time_buf));
    if(!ret || strcmp(time_buf+2, (char*)set_time_A_scanned_param)) return -1;
    return 0;
}
int set_time_B(int time)
{
    if(time / 1000000) return -1;
    int ret;
    char time_buf[9];
    sprintf(time_buf, "B %06d", time);
    ret = call_async(&set_time_B_cnd, 1, create_sstring(2, time, time_buf));
    if(!ret || strcmp(time_buf+2, (char*)set_time_B_scanned_param)) return -1;
    return 0;
}
int set_time_C(int time)
{
    if(time / 1000000) return -1;
    int ret;
    char time_buf[9];
    sprintf(time_buf, "C %06d", time);
    ret = call_async(&set_time_C_cnd, 1, create_sstring(2, time, time_buf));
    if(!ret || strcmp(time_buf+2, (char*)set_time_C_scanned_param)) return -1;
    return 0;
}
int set_time_C_(int time)
{
    if(time / 1000000) return -1;
    int ret;
    char time_buf[9];
    sprintf(time_buf, "c %06d", time);
    ret = call_async(&set_time_C__cnd, 1, create_sstring(2, time, time_buf));
    if(!ret || strcmp(time_buf+2, (char*)set_time_C__scanned_param)) return -1;
    return 0;
}
int set_time_D(int time)
{
    if(time / 1000000) return -1;
    int ret;
    char time_buf[9];
    sprintf(time_buf, "D %06d", time);
    ret = call_async(&set_time_D_cnd, 1, create_sstring(2, time, time_buf));
    if(!ret || strcmp(time_buf+2, (char*)set_time_D_scanned_param)) return -1;
    return 0;
}
int set_time_E(int time)
{
    if(time / 1000000) return -1;
    int ret;
    char time_buf[9];
    sprintf(time_buf, "E %06d", time);
    ret = call_async(&set_time_E_cnd, 1, create_sstring(2, time, time_buf));
    if(!ret || strcmp(time_buf+2, (char*)set_time_E_scanned_param)) return -1;
    return 0;
}
int set_time_F(int time)
{
    if(time / 1000000) return -1;
    int ret;
    char time_buf[9];
    sprintf(time_buf, "F %06d", time);
    ret = call_async(&set_time_F_cnd, 1, create_sstring(2, time, time_buf));
    if(!ret || strcmp(time_buf+2, (char*)set_time_F_scanned_param)) return -1;
    return 0;
}
int set_time_G(int time)
{
    if(time / 1000000) return -1;
    int ret;
    char time_buf[9];
    sprintf(time_buf, "G %06d", time);
    ret = call_async(&set_time_G_cnd, 1, create_sstring(2, time, time_buf));
    if(!ret || strcmp(time_buf+2, (char*)set_time_G_scanned_param)) return -1;
    return 0;
}
int set_time_H(int time)
{
    if(time / 1000000) return -1;
    int ret;
    char time_buf[9];
    sprintf(time_buf, "H %06d", time);
    ret = call_async(&set_time_H_cnd, 1, create_sstring(2, time, time_buf));
    if(!ret || strcmp(time_buf+2, (char*)set_time_H_scanned_param)) return -1;
    return 0;
}

int test_red()
{
    int ret;
    ret = call_async(&test_red_cnd, 1, create_sstring(2, test, red_buf));
    if(!ret) return -1;
    return test_red_scanned_param;
}
int test_ylw()
{
    int ret;
    ret = call_async(&test_ylw_cnd, 1, create_sstring(2, test, ylw));
    if(!ret) return -1;
    return test_ylw_scanned_param;
}
int test_grn()
{
    int ret;
    ret = call_async(&test_grn_cnd, 1, create_sstring(2, test, grn));
    if(!ret) return -1;
    return test_grn_scanned_param;
}
int test_pred()
{
    int ret;
    ret = call_async(&test_pred_cnd, 1, create_sstring(2, test, pred_buf));
    if(!ret) return -1;
    return test_pred_scanned_param;
}
int test_pgrn()
{
    int ret;
    ret = call_async(&test_pgrn_cnd, 1, create_sstring(2, test, pgrn));
    if(!ret) return -1;
    return test_pgrn_scanned_param;
}