# Makefile for the nuggets repository
#
# Mark Lekina Rorat, December 2022

.PHONY: all clean test valgrind

all:
	make --directory=src

clean:
	make clean --directory=src

test:
	make test --directory=src

leaks:
	make leaks --directory=src
