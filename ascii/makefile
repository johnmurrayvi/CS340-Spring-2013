#
# Makefile for ascii-only
#
CC      = gcc
PROGS   = ascii-only
CFLAGS  = -Wall -g3

all:    $(PROGS)

ascii-only: ascii-only.o
	$(CC) $(CFLAGS) -o $@ $^

ascii-only.o: ascii-only.c ascii-only.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(PROGS)
