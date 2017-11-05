.PHONY: test all juez/random_player genetico/main

all: test_juez

%.all: %
	@make -C $<

test_juez: juez/random_player juez/c_linea.py genetico/main
	python2 juez/c_linea.py\
	        --ui True\
	        --red_player ./genetico/main\
	        --iterations 10\
	        --columns 7 --rows 6 --p 21 --c 4

juez/random_player:
	@make -C juez random_player

genetico/main:
	@make -C genetico main