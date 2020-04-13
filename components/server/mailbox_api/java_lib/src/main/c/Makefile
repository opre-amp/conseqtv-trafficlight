PREFIX = arm-linux-gnueabihf-

INCLUDE_LIST += -I/usr/lib/jvm/default/include
INCLUDE_LIST += -I/usr/lib/jvm/default/include/linux
INCLUDE_LIST += -I$(COMPONENTS_PATH)/server/mailbox_api/c_lib


libmailbox_javacompat.so: hu_conseqtv_Mailbox.o
	$(PREFIX)gcc -shared -o $@ $^

hu_conseqtv_Mailbox.o: hu_conseqtv_Mailbox.c hu_conseqtv_Mailbox.h
	$(PREFIX)gcc $(INCLUDE_LIST) -fPIC -c -o $@ $<

hu_conseqtv_Mailbox.h: $(COMPONENTS_PATH)/server/mailbox_api/java_lib/build/classes/java/main/hu/conseqtv/Mailbox.class
	cd $(COMPONENTS_PATH)/server/mailbox_api/java_lib/build/classes/java/main/ && javah -jni -v -d $(COMPONENTS_PATH)/server/mailbox_api/java_lib/src/main/c hu.conseqtv.Mailbox

$(COMPONENTS_PATH)/server/mailbox_api/java_lib/build/classes/java/main/hu/conseqtv/Mailbox.class: $(COMPONENTS_PATH)/server/mailbox_api/java_lib/src/main/java/hu/conseqtv/Mailbox.java
	cd $(COMPONENTS_PATH)/server/mailbox_api/java_lib && ./gradlew build

clean:
	-bash -c "cd $(COMPONENTS_PATH)/server/mailbox_api/java_lib && ./gradlew clean"
	-rm hu_conseqtv_Mailbox.h
	-rm *o