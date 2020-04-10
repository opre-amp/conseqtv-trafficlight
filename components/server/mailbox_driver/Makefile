obj-m += mailbox.o
mailbox-objs += mailbox_api.o mailbox_driver.o ioctl_ops.o

all:
	make ARCH=arm CROSS_COMPILE=$(CROSS_LINUX) -C $(LINUX) modules M=$(PWD)
clean:
	make ARCH=arm CROSS_COMPILE=$(CROSS_LINUX) -C $(LINUX) clean M=$(PWD)
