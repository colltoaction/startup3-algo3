CXXFLAGS= -std=c++11 -pedantic -Wall -O5

.PHONY: test all main

all: main

main: main.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) $< -o $@

test: main_test.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) main_test.cpp -o ricardofort.o
