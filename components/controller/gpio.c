#include "gpio.h"

void* base = (void*)0x3f200000;
unsigned int lvl_offset = 0x34;
unsigned int set_offset = 0x1C;
unsigned int clr_offset = 0x28;

static void setup_gpio(int id, char output)
{
    volatile unsigned int* gpfsel = base + (id / 10)*4;
    unsigned int bit = (id % 10)*3;
    if(output) *gpfsel |= 1 << bit;
    else *gpfsel &= ~(1 << bit);
}

int read_gpio(int id)
{
    setup_gpio(id, 0);
    volatile unsigned int* gplvl = base + lvl_offset + (id / 32)*4;
    unsigned int bit = id % 32;
    return (*gplvl) & (1 << bit);
}

void write_gpio(int id, int level)
{
    setup_gpio(id, level);
    if(!level) return;
    volatile unsigned int* gpmod = base + clr_offset + (id / 32)*4;
    unsigned int bit = id % 32;
    (*gpmod) = (1 << bit);
}