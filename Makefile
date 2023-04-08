OBJS= kinotracker.c strings.o file.o

CFLAGS=-Wall -Werror -pedantic -std=gnu99 -g
.PHONY=all clean
.DEFAULT_GOAL:=all

all: kinotracker

kinotracker: $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o kinotracker

strings.o: strings.c strings.h
	gcc $(CFLAGS) -c strings.c

file.o: file.c file.h
	gcc $(CFLAGS) -c file.c


clean:
	rm -f kinotracker

	rm -f strings.o
	rm -f file.o


