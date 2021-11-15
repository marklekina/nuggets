# Makefile for the server
#
# Palmer's Posse

C = common
LIBS = $C/libcs50.a
S = support

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$S
CC = gcc
MAKE = make 

SERVERLOGS = -DLOG_SERVER=stderr -DLOG_MESSAGE=NULL
CLIENTLOGS = -DLOG_CLIENT=stderr -DLOG_MESSAGE=NULL


server: server.o $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

server.o: server.c $C/game.h $C/player.h $C/grid.h $S/message.h $C/XYZ.c $C/mem.h $C/file.h $S/log.h
	$(CC) $(CFLAGS) $(SERVERLOGS) -c server.c -o $@


.PHONY: all test valgrind clean 

all: server 

clean: 
	rm -rf *.dSYM 
	rm -f *~ *.o
	rm -f server
	rm -f core
