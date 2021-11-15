# Makefile for the common directory
#
# Palmer's Posse, November 2021

S = ../support

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S

OBJS = grid.o player.o game.o XYZ.o
LIBS = common.a $S/support.a -lm

$(LIBS): $(OBJS)
	ar cr $@ $^

grid.o: grid.h file.h mem.h 

player.o: player.h mem.h $S/message.h

game.o: game.h file.h mem.h grid.h player.h

XYZ.o: mem.h grid.h player.h $S/message.h

clean: 
	rm -f grid
	rm -f game
	rm -f player
	rm -f core
	rm -f XYZ
	rm -f *~ *.o
	rm -rf *.dSYM
