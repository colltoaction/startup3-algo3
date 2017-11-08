CXXFLAGS= -std=c++11 -pedantic -Wall -O4

.PHONY: test all main

all: main

main: main.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) $< -o $@

test: main_test.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) -DSHOWSENSEI -DOUTPUTBEST	 main_test.cpp -o ricardofort.o

experimentarFitness: main_test.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) -DFITNESS -DFITNESSONE -DOUTPUTBEST main_test.cpp -o ricardofort.o && ./ricardofort.o 2> ../experimentacion/fitness.out

experimentarFitness2: main_test.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) -DFITNESS -DFITNESSTWO -DOUTPUTBEST main_test.cpp -o ricardofort.o && ./ricardofort.o 2> ../experimentacion/fitness2.out

experimentarCorrelacionGenes: main_test.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) -DGENECORRELATION -DOUTPUTBEST main_test.cpp -o ricardofort.o && ./ricardofort.o 2> ../experimentacion/geneweights.out


experimentarNonLinear: main_test.cpp matingpool.h genome.h ../common/judge.h
	g++ $(CXXFLAGS) -DFITNESS -DFITNESSTWO -DOUTPUTBEST -DNONLINEAR main_test.cpp -o ricardofort.o && ./ricardofort.o 2> ../experimentacion/fitnessNonLinear.out