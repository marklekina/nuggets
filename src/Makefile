# Makefile for the src directory
#
# Mark Lekina Rorat, December 2022

.PHONY: all clean test valgrind

all:
	make --directory=support
	make --directory=modules
	make --directory=tests
	make --directory=main

clean:
	make clean --directory=support
	make clean --directory=modules
	make clean --directory=tests
	make clean --directory=main

test:
	make test --directory=tests

leaks:
	make leaks --directory=tests
