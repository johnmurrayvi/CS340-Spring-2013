#
# Makefile for ascii
#
CC      = gcc
PROGS   = ascii
CLFAGS  = -Wall -g3

all:    $(PROGS)

ascii:  ascii-only.o
	$(CC) ascii-only.o -o ascii

ascii-only.o:   ascii-only.c
	$(CC) $(CFLAGS) -c ascii-only.c

clean:
	rm -f *.o $(PROGS)
