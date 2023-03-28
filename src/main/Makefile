# Makefile for the main directory
#
# Mark Lekina Rorat, March 2023


S = ../support
M = ../modules

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S -I$M
MAKE = make

LIBS = $M/modules.a $S/support.a
BIN = ../../bin
PROGS = $(BIN)/server $(BIN)/client

# for memory-leak tests
LEAKS = leaks --atExit --

.PHONY: all bin clean

all: $(PROGS)

$(BIN)/server: server.o | bin
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(BIN)/client: client.o | bin
	$(CC) $(CFLAGS) $^ $(LIBS) -lcurses -o $@

server.o: server.c $M/game.h $M/grid.h $M/player.h $M/pile.h $M/point.h $S/message.h $S/mem.h $S/file.h
client.o: client.c $S/message.h $S/mem.h

bin:
	mkdir -p $(BIN)

clean:
	rm -rf *.dSYM
	rm -f core
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
