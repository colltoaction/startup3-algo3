CXXFLAGS=-g -std=c++11 -pedantic -Wall -O3

.PHONY: all

all: main

main: main.cpp ../common/judge.h ../genetico/matingpool.h
	$(CXX) $(CXXFLAGS) -o $@ $<
