CXXFLAGS= -std=c++11 -pedantic -Wall -O5

.PHONY: test all main

all: main

main: main.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) $< -o $@

test: main_test.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) -DSHOWSENSEI	 main_test.cpp -o ricardofort.o

experimentar fitness: main_test.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) -DFITNESS main_test.cpp -o ricardofort.o && ./ricardofort.o 2> ../experimentacion/fitness.out

experimentar correlacion genes: main_test.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) -DGENECORRELATION main_test.cpp -o ricardofort.o && ./ricardofort.o 2> ../experimentacion/geneweights.out
