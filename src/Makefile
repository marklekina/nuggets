# Makefile for the src directory
#
# Mark Lekina Rorat, December 2022

.PHONY: all clean test valgrind

all:
	make --directory=support
	make --directory=modules
	make --directory=tests

clean:
	make clean --directory=support
	make clean --directory=modules
	make clean --directory=tests

test:
	make test --directory=tests

valgrind:
	make valgrind --directory=tests
