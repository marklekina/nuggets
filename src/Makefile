# Makefile for the src directory
#
# Mark Lekina Rorat, March 2023

.PHONY: build all clean test leaks

build: all clean

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
