CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -O0 -ggdb -no-pie
LIBS:=-lm

all: cachesim

cachesim: cachesim.o cache.o optparser.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

cachesim.o: cachesim.c
	$(CC) $(CFLAGS) -c $^ -o $@

cache.o: cache.c
	$(CC) $(CFLAGS) -c $^ -o $@

optparser.o: optparser.c
	$(CC) $(CFLAGS) -c $^ -o $@

format:
	clang-format -i *.c *.h

clean:
	rm -f *.o
	rm -f cachesim
	

