CXXFLAGS=-g -std=c++11 -pedantic -Wall -O4

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

.PHONY: all test %.test

all: main

main: main.o ${COMMON_OBJS} ${COMMON_HDRS}
	g++ $(CXXFLAGS) -o $@ $^
