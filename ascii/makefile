#
# Makefile for ascii
#
CC      = gcc
PROGS   = ascii-only
CLFAGS  = -Wall -g3 ascii-only.h

all:    $(PROGS)

ascii-only:  ascii-only.o
	$(CC) ascii-only.o -o ascii-only

ascii-only.o:   ascii-only.c
	$(CC) $(CFLAGS) -c ascii-only.c

clean:
	rm -f *.o $(PROGS)
