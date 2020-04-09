PREFIX = $(CROSS_COMPILE)
MEMORY = 0x20000000

ARCH = -march=armv8.2-a -mtune=cortex-a53 -marm -mfpu=neon-fp-armv8 -mfloat-abi=hard

CFLAGS = -Werror -O2  -ffreestanding -fno-exceptions -nostdinc -nostdlib -nostartfiles
CFLAGS += $(ARCH)

LDFLAGS = -Ttext $(MEMORY) -nostartfiles -Trpi.x

TARGET = controller

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

CC	= $(PREFIX)gcc
LD	= $(PREFIX)ld


all: $(TARGET).img

%.o: %.s
	$(CC) $(ARCH) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


$(TARGET).img: start.o $(OBJS) rpi.x
	$(LD) $(LDFLAGS) -o $(TARGET).elf  start.o $(OBJS)
	$(PREFIX)objcopy $(TARGET).elf -O binary $(TARGET).img
	$(ECHO) $(PREFIX)nm $(TARGET).elf

clean :
	-rm *.o
	-rm *.elf
	-rm *.img
