# Makefile for the 'modules.a'
#
# Mark Lekina Rorat, December 2022


S = ../support

# specify where to search for support module headers and source files
vpath %.h $S
vpath %.c $S

LIB = modules.a
TESTS = pointtest

OBJS = point.o
SOBJS = log.o mem.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all test valgrind clean

all: $(LIB) $(TESTS)

$(LIB): $(OBJS) $(SOBJS)
	ar cr $@ $^

pointtest: point.c $(SOBJS)
	$(CC) $(CFLAGS) -DUNIT_TEST -I$S $^ -o $@

point.o: point.c
	$(CC) $(CFLAGS) -c -I$S $^

log.o: log.c
mem.o: mem.c

test: $(TESTS) testing.sh
	bash -v testing.sh

valgrind: $(TESTS) testing.sh
	$(VALGRIND) bash -v testing.sh

clean:
	rm -f core
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
	rm -f $(LIB)
	rm -f $(TESTS)
