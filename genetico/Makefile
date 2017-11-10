CXXFLAGS= -std=c++11 -pedantic -Wall -O4

COMMON_HDRS = \
        ../common/board.h \
        ../common/judge_proxy.h \
        ../common/player.h \
        ../common/player_random.h \
        ../common/game.h \
        ../common/players.h \
        ../common/possible_move.h \
        ../common/player_minimax.h \
        ../common/player_minimax_n.h \
        ../common/minimax.h

COMMON_SRCS = \
        ../common/game.cpp

COMMON_OBJS = $(subst .cpp,.o,$(COMMON_SRCS))

GENETICO_FILES = \
        genome.h \
        mating_pool.h \
        gene.h \
        player_genetic.h

.PHONY: test all main

all: main

main: main.o ${COMMON_OBJS} ${COMMON_HDRS} ${GENETICO_FILES}
	g++ $(CXXFLAGS) -o $@ $^

main_test: main_test.o ${COMMON_OBJS} ${COMMON_HDRS} ${GENETICO_FILES}
	g++ $(CXXFLAGS) -DSHOWSENSEI -DOUTPUTBEST -o $@ $^

experimentarFitness: main_test.o ${COMMON_OBJS} ${COMMON_HDRS} ${GENETICO_FILES}
	g++ $(CXXFLAGS) -DFITNESS -DFITNESSONE -DOUTPUTBEST  -o $@ $^ &&\
	 ./$@ 2> ../experimentacion/fitness.out

experimentarFitness2: main_test.o ${COMMON_OBJS} ${COMMON_HDRS} ${GENETICO_FILES}
	g++ $(CXXFLAGS) -DFITNESS -DFITNESSTWO -DOUTPUTBEST  -o $@ $^ &&\
	 ./$@ 2> ../experimentacion/fitness2.out

experimentarCorrelacionGenes: main_test.o ${COMMON_OBJS} ${COMMON_HDRS} ${GENETICO_FILES}
	g++ $(CXXFLAGS) -DGENECORRELATION -DOUTPUTBEST  -o $@ $^ &&\
	 ./$@ 2> ../experimentacion/geneweights.out

experimentarNonLinear: main_test.o ${COMMON_OBJS} ${COMMON_HDRS} ${GENETICO_FILES}
	g++ $(CXXFLAGS) -DFITNESS -DFITNESSONE -DOUTPUTBEST -DNONLINEAR  -o $@ $^ &&\
	 && ./$@ 2> ../experimentacion/fitnessNonLinear.out

