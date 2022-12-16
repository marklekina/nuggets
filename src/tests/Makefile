# Makefile for the tests directory
#
# Mark Lekina Rorat, December 2022


S = ../support
M = ../modules

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S -I$M
MAKE = make

LIBS = $M/modules.a $S/support.a
TESTS = pointtest piletest playertest

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: $(TESTS)

pointtest: pointtest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

piletest: piletest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

playertest: playertest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

pointtest.o: pointtest.c $M/point.h $S/tester.h
piletest.o: piletest.c $M/pile.h $M/point.h $S/tester.h
playertest.o: playertest.c $M/player.h $M/point.h $S/tester.h $S/file.h


.PHONY: all test valgrind clean

# expects bash script `testing.sh` to exist.
test: $(TESTS) testing.sh
	sh testing.sh


# expects bash script `testing.sh` to exist.
valgrind: $(TESTS) testing.sh
	cat testing.sh | xargs -n1 $(VALGRIND)

clean:
	rm -rf *.dSYM
	rm -f core
	rm -f $(TESTS)
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
