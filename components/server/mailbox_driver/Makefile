CROSS_LINUX = arm-linux-gnueabihf-
obj-m += mailbox.o
mailbox-objs += mailbox_api.o mailbox_driver.o ioctl_ops.o

all: mailbox_api.c
	make ARCH=arm CROSS_COMPILE=$(CROSS_LINUX) -C $(LINUX) modules M=$(COMPONENTS_PATH)/server/mailbox_driver
clean:
	make ARCH=arm CROSS_COMPILE=$(CROSS_LINUX) -C $(LINUX) clean M=$(COMPONENTS_PATH)/server/mailbox_driver
