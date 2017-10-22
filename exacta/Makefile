CXXFLAGS=-g -std=c++11 -pedantic -Wall

tests := $(basename $(wildcard tests/*.test-in))
all-tests := $(addsuffix .test, $(basename $(wildcard tests/*.test-cmp)))

.PHONY: all test %.test

all: test
	@echo "$(all-tests)"
	@echo "Pasaron todos los tests!"

test: $(all-tests)

%.test: %.test-in %.test-cmp main
	@./main < $< 2> /dev/null | diff -q $(word 2, $?) - >/dev/null || \
	(echo "Falló el test $@ en main" && exit 1)

main: main.cpp game.h player.h
	$(CXX) $(CXXFLAGS) -o $@ $<
