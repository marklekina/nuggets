# Makefile for server.c
#
# Palmer's Posse, November 2021

C = common
S = support

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C

SOURCES = $S/message.h $C/file.h $C/mem.h $C/game.h $C/player.h $C/XYZ.c
LIBS = $C/common.a $C/libcs50.a $S/support.a -lm

all:
	make -C common/
	make server

server: server.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

server.o: $(SOURCES)

.PHONY: clean

clean:
	make -C common/ clean
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f server
	rm -f core
