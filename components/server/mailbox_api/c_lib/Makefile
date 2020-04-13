PREFIX = arm-linux-gnueabihf-

main: libmailbox.so main.o
	$(PREFIX)gcc main.o -L./ -lmailbox  -pthread  -o $@ 

main.o: main.c
	$(PREFIX)gcc -c -o $@ $<

libmailbox.so: mailbox_api.o
	$(PREFIX)gcc -shared -o $@ $^

mailbox_api.o: mailbox_api.c
	$(PREFIX)gcc -fPIC -c -o $@ $<

clean:
	-rm main *o