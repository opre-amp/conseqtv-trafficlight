#include "timer.h"

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