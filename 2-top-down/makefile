#
# Makefile for top-down
#

CC      = gcc
CFLAGS  = -O2
SRC     = top-down.c
HDR     = $(SRC:.c=.h)
OBJ     = $(SRC:.c=.o)
PRG     = $(basename $(SRC))


.PHONY: all test clean debug release

all:    $(PRG)

debug:  CFLAGS = -Wall -g3
debug:  clean all test

$(PRG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ): $(SRC) $(HDR)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(PRG)

test: $(PRG)
	cd tests && $(MAKE)


.DEFAULT_GOAL := all
