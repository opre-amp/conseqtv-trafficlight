PREFIX = $(CROSS_LINUX)

loader: loader.o
	$(PREFIX)gcc -o $@ $^

loader.o: loader.c
	$(PREFIX)gcc -c -o $@ $<

clean:
	-rm loader *o