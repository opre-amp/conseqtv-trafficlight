PREFIX = $(CROSS_LINUX)

INCLUDE_LIST += -I/usr/lib/jvm/default/include
INCLUDE_LIST += -I/usr/lib/jvm/default/include/linux
INCLUDE_LIST += -I$(COMPONENTS_PATH)/server/mailbox_api/c_lib

LIBRARY_PATH = -L$(COMPONENTS_PATH)/server/mailbox_api/c_lib

libmailbox_javacompat.so: hu_conseqtv_TrafficLight.o
	$(PREFIX)gcc -shared -o $@ $^ $(LIBRARY_PATH) -lmailbox

hu_conseqtv_TrafficLight.o: hu_conseqtv_TrafficLight.c hu_conseqtv_TrafficLight.h
	$(PREFIX)gcc $(INCLUDE_LIST) -fPIC -c -o $@ $<

hu_conseqtv_TrafficLight.h: $(COMPONENTS_PATH)/server/mailbox_api/java_lib/build/classes/java/main/hu/conseqtv/TrafficLight.class
	cd $(COMPONENTS_PATH)/server/mailbox_api/java_lib/build/classes/java/main/ && javah -jni -v -d $(COMPONENTS_PATH)/server/mailbox_api/java_lib/src/main/c hu.conseqtv.TrafficLight

$(COMPONENTS_PATH)/server/mailbox_api/java_lib/build/classes/java/main/hu/conseqtv/TrafficLight.class: $(COMPONENTS_PATH)/server/mailbox_api/java_lib/src/main/java/hu/conseqtv/*.java
	cd $(COMPONENTS_PATH)/server/mailbox_api/java_lib && ./gradlew build

clean:
	-bash -c "cd $(COMPONENTS_PATH)/server/mailbox_api/java_lib && ./gradlew clean"
	-rm hu_conseqtv_TrafficLight.h
	-rm *o