.PHONY: test all juez/random_player genetico/main minimax/limited playAgainstMinimaxN

all: test_juez

%.all: %
	@make -C $<

test_juez: juez/random_player juez/c_linea.py genetico/main
	python2 juez/c_linea.py\
	        --ui True\
	        --red_player ./genetico/parametrizado.sh\
	        --iterations 10\
	        --columns 7 --rows 6 --p 21 --c 4

juez/random_player:
	@make -C juez random_player

genetico/main:
	@make -C genetico main

minimax/limited:
	@make -C minimax limited


playAgainstMinimaxN: minimax/limited juez/c_linea.py
	python2 juez/c_linea.py\
	        --ui True\
	        --red_player ./minimax/limited\
	        --iterations 10\
	        --columns 7 --rows 6 --p 21 --c 4

playAgainstGentic: juez/c_linea.py
	python2 juez/c_linea.py\
	        --ui True\
	        --red_player ./cmake-build-debug/geneticPlayer\
	        --iterations 10\
	        --columns 7 --rows 6 --p 21 --c 4
