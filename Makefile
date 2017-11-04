.PHONY: test all juez/random_player minimax/main

all: test_juez

%.all: %
	@make -C $<

test_juez: juez/random_player juez/c_linea.py minimax/main
	python2 juez/c_linea.py\
	        --blue_player ./juez/random_player\
	        --red_player ./minimax/main\
	        --iterations 10\
	        --columns 4 --rows 4 --p 8 --c 3

juez/random_player:
	@make -C juez random_player

minimax/main:
	@make -C minimax main