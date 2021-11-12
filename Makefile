# Makefile for the common directory
#
# Palmer's Posse

L = ../tse-meghanokeefe/libcs50
C = ../common

FLAGS =
CC = gcc
MAKE = make 

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

server: server.o $(LIBS)
	$(CC) $(CFLAGS)$^$(LIBS) -o $@
server.o: $C/game.h $C/player.h $C/grid.h $C/message.h $C/visibility.c $C/XYZ.c ../support/mem.h ../support/file.h

.PHONY: all test valgrind clean 

all: server 


valgrind:
	$(VALGRIND) 

clean: 
	rm -rf *.dSYM 
	rm -f *~ *.o
	rm -f server
	rm -f core

