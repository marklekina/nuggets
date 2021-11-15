# Makefile for the server
#
# Palmer's Posse

C = common
S = support

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$S

OBJS = server.o
SOURCES = $C/game.h $C/player.h $C/grid.h $S/message.h $C/mem.h $C/file.h
LIBS = $C/common.a -lm

server: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(OBJS): $(SOURCES)

.PHONY: clean

clean: 
	rm -rf *.dSYM 
	rm -f *~ *.o
	rm -f server
	rm -f core
