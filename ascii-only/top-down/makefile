#
# Makefile for top-down
#
CC      = gcc
PROGS   = top-down
CFLAGS  = -Wall -g3

all:    $(PROGS)

top-down: top-down.o
	$(CC) $(CFLAGS) -o $@ $^

top-down.o: top-down.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(PROGS)
