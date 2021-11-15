# Makefile for the server
#
# Palmer's Posse


C = common
S = support
LIBS = $C/libcs50.a -lm

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb  -I$C -I$S
CC = gcc
MAKE = make 

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
# to where shall they log?
SERVERLOGS = -DLOG_SERVER=stderr -DLOG_MESSAGE=NULL
CLIENTLOGS = -DLOG_CLIENT=stderr -DLOG_MESSAGE=NULL



server.o: server.c $C/game.h $C/player.h $C/grid.h $S/message.h $C/XYZ.c $C/mem.h $C/file.h $S/log.h
	$(CC) $(CFLAGS) $(SERVERLOGS) -c server.c -o $@

server: server.o $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
	
.PHONY: all test valgrind clean 

all: server 


valgrind:
	$(VALGRIND) 

clean: 
	rm -rf *.dSYM 
	rm -f *~ *.o
	rm -f server
	rm -f core