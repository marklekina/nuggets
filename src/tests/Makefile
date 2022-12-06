# Makefile for the tests directory
#
# Mark Lekina Rorat, December 2022


S = ../support
M = ../modules

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S -I$M
MAKE = make

SOURCES = pointtest.c $M/point.h
LIBS = $M/modules.a $S/support.a
TESTS = pointtest

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: $(TESTS)

pointtest: pointtest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

pointtest.o: $(SOURCES)


.PHONY: all test valgrind clean

# expects bash script `testing.sh` to exist.
test: $(TESTS) testing.sh
	bash -v testing.sh


# expects bash script `testing.sh` to exist.
valgrind: $(TESTS) testing.sh
	$(VALGRIND) bash -v testing.sh

clean:
	rm -rf *.dSYM
	rm -f core
	rm -f $(TESTS)
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
