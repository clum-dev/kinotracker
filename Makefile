CFLAGS=-Wall -Werror -pedantic -std=gnu99 -g
.PHONY=all clean
.DEFAULT_GOAL:=all

OBJS_kinotracker=kinotracker.c clum-lib/strings.o clum-lib/file.o

all: kinotracker

kinotracker: $(OBJS_kinotracker)
	gcc $(CFLAGS) $(OBJS_kinotracker) -o kinotracker

clum-lib/strings.o: clum-lib/strings.c clum-lib/strings.h
	gcc $(CFLAGS) -o clum-lib/strings.o -c clum-lib/strings.c

clum-lib/file.o: clum-lib/file.c clum-lib/file.h
	gcc $(CFLAGS) -o clum-lib/file.o -c clum-lib/file.c


clean:
	rm -f kinotracker
	rm -f clum-lib/strings.o
	rm -f clum-lib/file.o
