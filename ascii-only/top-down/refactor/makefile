#
# Makefile for top-down
#

CC      = gcc
CFLAGS  = -Wall -g3
SOURCES = top-down.c
HEADERS = top-down.h
PROGS   = $(firstword $(basename $(SOURCES)))

all:    $(PROGS)

$(PROGS).o: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $<

$(PROGS): $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o $(PROGS)
