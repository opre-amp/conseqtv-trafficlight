PREFIX = arm-linux-gnueabihf-

main: mailbox_api.o main.o
	$(PREFIX)gcc -pthread -o $@ $^

%.o: %.c
	$(PREFIX)gcc -c -o $@ $<

clean:
	-rm main *.o