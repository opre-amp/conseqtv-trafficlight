PREFIX = $(CROSS_LINUX)

loader: loader.o
	$(PREFIX)gcc -g -o $@ $^

loader.o: loader.c
	$(PREFIX)gcc -g -c -o $@ $<

clean:
	-rm loader *o
