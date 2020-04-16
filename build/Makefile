COMPONENTS_PATH = $(PWD)/../components
CROSS_LINUX = arm-linux-gnueabihf-
CROSS_BARE = arm-none-eabi-
export

DEB_NAME = conseqtv-server

ARTIFACTS = controller.img loader mailbox.ko zImage modules.tar.gz dtbs.tar.gz u-boot.bin boot.scr config.txt libmailbox.so libmailbox_javacompat.so server.jar  control postinst conseqtv-server.service send_data

PI := bcm2710-rpi-3-b-plus

LINUX_REPO :=  https://github.com/raspberrypi/linux.git
LINUX_BRANCH := rpi-4.19.y-rt
LINUX_CONFIG :=  bcm2709_defconfig

UBOOT_REPO := git://git.denx.de/u-boot.git
UBOOT_CONFIG := rpi_2_defconfig


$(DEB_NAME).deb: $(ARTIFACTS)
	mkdir -p $(DEB_NAME)
	mkdir -p $(DEB_NAME)/DEBIAN
	cp control $(DEB_NAME)/DEBIAN/
	cp postinst $(DEB_NAME)/DEBIAN/
	mkdir -p $(DEB_NAME)/usr/share/$(DEB_NAME)
	cp controller.img $(DEB_NAME)/usr/share/$(DEB_NAME)/
	mkdir -p $(DEB_NAME)/usr/bin
	cp loader $(DEB_NAME)/usr/bin/loader
	cp send_data $(DEB_NAME)/usr/bin/send_data
	cd $(DEB_NAME) && tar xzf ../modules.tar.gz
	mkdir -p $(DEB_NAME)/lib/modules/$$(basename $(DEB_NAME)/lib/modules/*)/kernel/extra/ 
	cp mailbox.ko $(DEB_NAME)/lib/modules/*/kernel/extra/
	mkdir -p $(DEB_NAME)/tmp/boot
	cp zImage $(DEB_NAME)/tmp/boot/
	cd $(DEB_NAME)/tmp/boot && tar xzf ../../../dtbs.tar.gz
	cp u-boot.bin $(DEB_NAME)/tmp/boot/
	cp boot.scr $(DEB_NAME)/tmp/boot/
	cp config.txt $(DEB_NAME)/tmp/boot/
	mkdir -p $(DEB_NAME)/usr/lib
	cp libmailbox.so $(DEB_NAME)/usr/lib/
	cp libmailbox_javacompat.so $(DEB_NAME)/usr/lib/
	cp server.jar $(DEB_NAME)/tmp/
	mkdir -p $(DEB_NAME)/etc/systemd/system
	cp conseqtv-server.service $(DEB_NAME)/etc/systemd/system/
	dpkg-deb --build $(DEB_NAME)



control:
	printf "Package: $(DEB_NAME)\nVersion: 1.0-1\nSection: base\nPriority: optional\nArchitecture: armhf\nReplaces: raspberrypi-kernel\nDepends: openjdk-8-jre-headless\nMaintainer: Levente Bajczi <levente.bajczi@edu.bme.hu>\nDescription: Installs the necessary components for the server part of the traffic light project\n" > $@

postinst:
	printf "#!/bin/bash\nsudo cp /tmp/server.jar /home/*/\nsudo cp -r /tmp/boot/* /boot/\nsudo systemctl enable conseqtv-server\necho 'You will have to restart this machine for the changes to take effect.'\n" > $@
	chmod 755 $@

conseqtv-server.service:
	printf "[Unit]\nDescription=Starts the controller software on boot.\n\n[Service]\nType=simple\nExecStartPre=depmod -a\nExecStartPre=modprobe mailbox\nExecStartPre=loader /usr/share/conseqtv-server/controller.img\nExecStart=java -jar /home/pi/server.jar && while true; do sleep 1; done\nExecStop=send_data stop\nExecStopPost=rmmod mailbox\n\n[Install]\nWantedBy=multi-user.target" > $@



controller.img: $(COMPONENTS_PATH)/controller/controller.img
	cp $< $@

$(COMPONENTS_PATH)/controller/controller.img: 
	$(MAKE) -C $(COMPONENTS_PATH)/controller/



loader: $(COMPONENTS_PATH)/server/loader/loader
	cp $< $@

$(COMPONENTS_PATH)/server/loader/loader:
	$(MAKE) -C $(COMPONENTS_PATH)/server/loader



mailbox.ko: $(COMPONENTS_PATH)/server/mailbox_driver/mailbox.ko
	cp $< $@

$(COMPONENTS_PATH)/server/mailbox_driver/mailbox.ko: zImage
	$(MAKE) -C $(COMPONENTS_PATH)/server/mailbox_driver/ LINUX=$(COMPONENTS_PATH)/server/linux
	touch $@



zImage: $(COMPONENTS_PATH)/server/linux/arch/arm/boot/zImage
	cp $< $@

modules.tar.gz: $(COMPONENTS_PATH)/server/linux/modules.tar.gz
	cp $< $@

dtbs.tar.gz: $(COMPONENTS_PATH)/server/linux/arch/arm/boot/dts/dtbs.tar.gz
	cp $< $@

$(COMPONENTS_PATH)/server/linux/modules.tar.gz: $(COMPONENTS_PATH)/server/linux/arch/arm/boot/zImage
	mkdir $(COMPONENTS_PATH)/server/linux/modules -p
	$(MAKE) -C $(COMPONENTS_PATH)/server/linux modules_install INSTALL_MOD_PATH=$(COMPONENTS_PATH)/server/linux/modules ARCH=arm CROSS_COMPILE=$(CROSS_LINUX)
	cd $(COMPONENTS_PATH)/server/linux/modules && tar czvf ../modules.tar.gz *

$(COMPONENTS_PATH)/server/linux/arch/arm/boot/dts/dtbs.tar.gz: $(COMPONENTS_PATH)/server/linux/arch/arm/boot/zImage
	cd $(COMPONENTS_PATH)/server/linux/arch/arm/boot/dts && tar czvf dtbs.tar.gz $(PI).dtb overlays/*.dtb*

$(COMPONENTS_PATH)/server/linux/arch/arm/boot/zImage: $(COMPONENTS_PATH)/server/linux/.config
	+$(MAKE) -C $(COMPONENTS_PATH)/server/linux zImage modules dtbs ARCH=arm CROSS_COMPILE=$(CROSS_LINUX)

$(COMPONENTS_PATH)/server/linux/.config: $(COMPONENTS_PATH)/server/linux/.git
	$(MAKE) -C $(COMPONENTS_PATH)/server/linux $(LINUX_CONFIG)  ARCH=arm CROSS_COMPILE=$(CROSS_LINUX)
	touch $(COMPONENTS_PATH)/server/linux/.config
	
$(COMPONENTS_PATH)/server/linux/.git: 
	git clone $(LINUX_REPO) --depth 1 --branch $(LINUX_BRANCH) $(COMPONENTS_PATH)/server/linux



u-boot.bin: $(COMPONENTS_PATH)/server/u-boot/u-boot.bin
	cp $< $@ 

$(COMPONENTS_PATH)/server/u-boot/u-boot.bin: $(COMPONENTS_PATH)/server/u-boot/.config
	$(MAKE) -C $(COMPONENTS_PATH)/server/u-boot ARCH=arm CROSS_COMPILE=$(CROSS_LINUX) u-boot.bin

$(COMPONENTS_PATH)/server/u-boot/.config: $(COMPONENTS_PATH)/server/u-boot/.git
	$(MAKE) -C $(COMPONENTS_PATH)/server/u-boot ARCH=arm CROSS_COMPILE=$(CROSS_LINUX) $(UBOOT_CONFIG)	

$(COMPONENTS_PATH)/server/u-boot/.git:
	git clone $(UBOOT_REPO) $(COMPONENTS_PATH)/server/u-boot
	touch $(COMPONENTS_PATH)/server/u-boot/.git
	


boot.scr: boot.cmd
	mkimage -C none -A arm -T script -d boot.cmd boot.scr
	
boot.cmd:
	echo "setenv bootargs earlyprintk console=tty0 console=ttyAM0 root=/dev/mmcblk0p2 rootfstype=ext4 rootwait noinitrd maxcpus=3 mem=512M" > boot.cmd
	echo "fatload mmc 0:1 \$${fdt_addr_r} $(PI).dtb" >> boot.cmd
	echo "fatload mmc 0:1 \$${kernel_addr_r} zImage" >> boot.cmd
	echo "bootz \$${kernel_addr_r} - \$${fdt_addr_r}" >> boot.cmd



config.txt:
	echo "kernel=u-boot.bin" > $@



libmailbox.so: $(COMPONENTS_PATH)/server/mailbox_api/c_lib/libmailbox.so
	cp $< $@ 

$(COMPONENTS_PATH)/server/mailbox_api/c_lib/libmailbox.so:
	$(MAKE) -C $(COMPONENTS_PATH)/server/mailbox_api/c_lib



libmailbox_javacompat.so: $(COMPONENTS_PATH)/server/mailbox_api/java_lib/src/main/c/libmailbox_javacompat.so
	cp $< $@ 

$(COMPONENTS_PATH)/server/mailbox_api/java_lib/src/main/c/libmailbox_javacompat.so: $(COMPONENTS_PATH)/server/mailbox_api/c_lib/libmailbox.so
	$(MAKE) -C $(COMPONENTS_PATH)/server/mailbox_api/java_lib/src/main/c



server.jar: $(COMPONENTS_PATH)/server/mailbox_api/java_lib/build/libs/mailbox_api-1.0-SNAPSHOT.jar
	cp $< $@ 

$(COMPONENTS_PATH)/server/mailbox_api/java_lib/build/libs/mailbox_api-1.0-SNAPSHOT.jar:
	cd $(COMPONENTS_PATH)/server/mailbox_api/java_lib && ./gradlew build


send_data: $(COMPONENTS_PATH)/server/send_data/send_data
	cp $< $@ 

$(COMPONENTS_PATH)/server/send_data/send_data:
	$(MAKE) -C $(COMPONENTS_PATH)/server/send_data

clean:
	-find . -type d,f -not -name 'Makefile' -exec rm -rf {} +

deepclean: clean
	-$(MAKE) -C $(COMPONENTS_PATH)/controller/ clean
	-$(MAKE) -C $(COMPONENTS_PATH)/server/loader clean
	-$(MAKE) -C $(COMPONENTS_PATH)/server/mailbox_driver/ LINUX=$(COMPONENTS_PATH)/server/linux clean
	-$(MAKE) -C $(COMPONENTS_PATH)/server/mailbox_api/c_lib clean
	-$(MAKE) -C $(COMPONENTS_PATH)/server/mailbox_api/java_lib/src/main/c clean
	-rm -rf $(COMPONENTS_PATH)/server/{linux,u-boot}
