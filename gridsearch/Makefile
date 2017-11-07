CXXFLAGS=-g -std=c++11 -pedantic -Wall

.PHONY: all

all: main

main: main.cpp ../common/judge.h ../genetico/matingpool.h
	$(CXX) $(CXXFLAGS) -o $@ $<
