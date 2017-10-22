.PHONY: test all juez/random_player exacta/main

all: test_juez

%.all: %
	@make -C $<

test_juez: juez/random_player juez/c_linea.py exacta/main
	python2 juez/c_linea.py\
	        --blue_player ./juez/random_player\
	        --red_player ./exacta/main\
	        --iterations 10\
	        --columns 3 --rows 4 --p 21 --c 2

juez/random_player:
	@make -C juez random_player

exacta/main:
	@make -C exacta main