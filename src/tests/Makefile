# Makefile for the tests directory
#
# Mark Lekina Rorat, December 2022


S = ../support
M = ../modules

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$S -I$M
MAKE = make

LIBS = $M/modules.a $S/support.a
TESTS = pointtest piletest playertest gridtest gametest

# for memory-leak tests
LEAKS = leaks --atExit --

.PHONY: all test leaks clean

all: $(TESTS)

pointtest: pointtest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

piletest: piletest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

playertest: playertest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

gridtest: gridtest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

gametest: gametest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

pointtest.o: pointtest.c $M/point.h $S/tester.h $S/mem.h
piletest.o: piletest.c $M/pile.h $M/point.h $S/tester.h $S/mem.h
playertest.o: playertest.c $M/player.h $M/point.h $S/tester.h $S/file.h $S/mem.h
gridtest.o: gridtest.c $M/grid.h $M/point.h $S/tester.h $S/file.h $S/mem.h
gametest.o: gametest.c $M/game.h $M/grid.h $M/player.h $M/pile.h $M/point.h $S/tester.h $S/mem.h

# expects bash script `runtests.sh` to exist.
test: $(TESTS) runtests.sh
	sh runtests.sh

# expects bash script `runtests.sh` to exist.
leaks: $(TESTS) runtests.sh
	cat runtests.sh | xargs -n1 $(LEAKS)

clean:
	rm -rf *.dSYM
	rm -f core
	rm -f $(TESTS)
	rm -rf *~ *.o *.gch *.dSYM
	rm -f *.log
