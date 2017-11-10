CXXFLAGS=-g -std=c++11 -pedantic -Wall

tests := $(basename $(wildcard tests/*.test-in))
all-tests := $(addsuffix .test, $(basename $(wildcard tests/*.test-cmp)))

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

all: test

test: $(all-tests)
	@echo "$<"
	@echo "Pasaron todos los tests!"

%.test: %.test-in %.test-cmp main
	@./main < $< 2> /dev/null | diff -q $(word 2, $?) - >/dev/null || \
	(echo "Falló el test $@ en main" && exit 1)

main: main.o ${COMMON_OBJS} ${COMMON_HDRS}
	g++ $(CXXFLAGS) -o $@ $^
