# Makefile for the nuggets repository
#
# Mark Lekina Rorat, March 2023

.PHONY: build all clean test leaks

build:
	make build --directory=src

all:
	make all --directory=src

clean:
	make clean --directory=src

test:
	make test --directory=src

leaks:
	make leaks --directory=src
