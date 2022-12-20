# Makefile for the modules directory
#
# Mark Lekina Rorat, December 2022


S = ../support

# specify where to search for support module headers and source files
vpath %.h $S
vpath %.c $S

LIB = modules.a
OBJS = point.o pile.o player.o grid.o game.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S
CC = gcc
MAKE = make

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	ar cr $@ $^

point.o: point.c
pile.o: pile.c
player.o: player.c
grid.o: grid.c
game.o: game.c

clean:
	rm -f core
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
	rm -f $(LIB)
