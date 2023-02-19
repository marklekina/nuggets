# Makefile for the main directory
#
# Mark Lekina Rorat, December 2022


S = ../support
M = ../modules

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S -I$M
MAKE = make

LIBS = $M/modules.a $S/support.a
PROGS = server client

# for memory-leak tests
LEAKS = leaks --atExit --

.PHONY: all test leaks clean

all: $(PROGS)

server: server.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

client: client.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

server.o: server.c $M/game.h $M/grid.h $M/player.h $M/pile.h $M/point.h $S/message.h $S/mem.h $S/file.h
client.o: client.c

clean:
	rm -rf *.dSYM
	rm -f core
	rm -f $(PROGS)
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
