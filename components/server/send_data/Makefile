PREFIX = $(CROSS_LINUX)

send_data: send_data.o
	$(PREFIX)gcc -g -o $@ $^

send_data.o: send_data.c
	$(PREFIX)gcc -g -c -o $@ $<

clean:
	-rm send_data *o
