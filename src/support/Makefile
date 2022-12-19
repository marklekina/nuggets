# Makefile for the support directory
#
# Mark Lekina Rorat, December 2022


LIB = support.a
OBJS = tester.o file.o mem.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJS)
	ar cr $@ $^

tester.o: tester.c
file.o: file.c
mem.o: mem.c

clean:
	rm -f core
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
	rm -f $(LIB)
