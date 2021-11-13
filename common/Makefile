# Makefile for the common directory
#
# Palmer's Posse

L = ../../tse-meghanokeefe/libcs50
C = ../common
CC = gcc
FLAGS =
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
OBJS = grid.o player.o game.o XYZ.o common.o
LIB = common.a

# grid.o: grid.h $L/file.h $L/mem.h 
# player.o: player.h $L/file.h $L/mem.h
# game.o: game.h
XYZ.o: $L/mem.h

.PHONY: all clean 

all: $(LIB)
$(LIB): $(OBJS)
		ar cr $(LIB) $(OBJS)


clean: 
	rm -f grid
	rm -f game
	rm -f player
	rm -f *~ *.o
	rm -rf *.dSYM

