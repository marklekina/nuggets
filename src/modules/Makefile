# Makefile for the modules directory
#
# Mark Lekina Rorat, December 2022


S = ../support

# specify where to search for support module headers and source files
vpath %.h $S
vpath %.c $S

LIBS = modules.a

OBJS = point.o
SOBJS = log.o mem.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S
CC = gcc
MAKE = make

.PHONY: all clean

all: $(LIBS)

$(LIBS): $(OBJS) $(SOBJS)
	ar cr $@ $^

point.o: point.c
log.o: log.c
mem.o: mem.c

clean:
	rm -f core
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
	rm -f $(LIBS)
