# - Makefile to compile album.c and helper readline.c
# - produces album executable

CC = gcc
CFLAGS = -Wall -ggdb
PROG = album
OBJS = album.o readline.o

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

album.o: readline.h
readline.o: readline.h

clean:
	rm -f *~
	rm -f $(PROG)
	rm -f *.o
