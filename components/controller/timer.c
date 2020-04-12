#include "timer.h"
#include "mailbox_api.h"
#define TIMER_PER_MS 19200

static volatile unsigned int* core_timer_ls32 = (void*)0x4000001C;

void waitms(unsigned int msecs)
{
    unsigned int start = *core_timer_ls32;
    while(*core_timer_ls32 < start + msecs*TIMER_PER_MS ||
            (start > start + msecs*TIMER_PER_MS &&
            *core_timer_ls32 >= start)
        ) {/* NOP */}
}

void waits(unsigned int secs)
{
    waitms(secs * 1000);
}


#define QUEUE_SIZE 10
static job queue[QUEUE_SIZE] = {0};

job* schedule_or(unsigned int msecs, void (*fp)(void), char* alternative, void (*fpalt)(void))
{
    for(int i = 0; i < QUEUE_SIZE; ++i)
    {
        if(!queue[i].valid) {
            queue[i].valid = 1;
            queue[i].start = *core_timer_ls32;
            queue[i].after = *core_timer_ls32 + msecs*TIMER_PER_MS;
            queue[i].alternative = alternative;
            queue[i].fp = fp;
            queue[i].fpalt = fpalt;
            return &queue[i];
        }
    }
    return 0;
}

job* schedule(unsigned int msecs, void (*fp)(void))
{
    return schedule_or(msecs, fp, 0, 0);
}

void run_jobs()
{
    unsigned int time = *core_timer_ls32;

    for(int i = 0; i < QUEUE_SIZE; ++i)
    {
        if(queue[i].valid) {
            if((time >= queue[i].after && queue[i].start <= queue[i].after) ||
               (time >= queue[i].after && queue[i].start > queue[i].after && time < queue[i].start)) {
                    queue[i].valid = 0;
                    queue[i].fp();
               }
            else if(queue[i].alternative && *(queue[i].alternative)) {
                    send_data("Alternative function called!", 29);
                    queue[i].valid = 0;
                    queue[i].fpalt();
            }
        }
    }
}

void empty_scheduling_queue()
{
    for(int i = 0; i < QUEUE_SIZE; ++i)
    {
        queue[i].valid = 0;
    }

}