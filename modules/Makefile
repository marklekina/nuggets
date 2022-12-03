# Makefile for the Nuggets modules.
#
# Mark Lekina Rorat, December 2022

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
TESTS = point
OBJS = point.o
LIBS =

vpath %.h ../support

.PHONY: all clean

all: point

# executable depends on object files
$(TESTS): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(TESTS)

# object files depend on include files
point.o: point.h log.h

clean:
	rm -f $(TESTS)
	rm -f *~ *.o
	rm -rf *.dSYM
