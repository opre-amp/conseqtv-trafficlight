PREFIX = $(CROSS_LINUX)

libmailbox.so: mailbox.o
	$(PREFIX)gcc -shared -o $@ $^

mailbox.o: mailbox.c
	$(PREFIX)gcc -fPIC -c -o $@ $<

clean:
	-rm *o