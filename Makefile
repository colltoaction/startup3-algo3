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

playAgainstGrid: juez/c_linea.py
	python2 juez/c_linea.py\
	        --ui True\
	        --red_player ./cmake-build-debug/gridSensei_2\
	        --iterations 10\
	        --columns 7 --rows 6 --p 21 --c 4

testAgainstCatedra: juez/c_linea.py
	python2 juez/c_linea.py\
	        --red_player ./cmake-build-debug/geneticPlayer\
	        --blue_player python2 ./juez/PlayerController.py\
	        --iterations 100\
	        --columns 7 --rows 6 --p 21 --c 4

testGridAgainstCatedra: juez/c_linea.py
	python2 juez/c_linea.py\
	        --red_player ./cmake-build-debug/gridSensei_2\
	        --blue_player python2 ./juez/PlayerController.py\
	        --iterations 100\
	        --columns 7 --rows 6 --p 21 --c 4


testGridAgainstGenetic: juez/c_linea.py
	python2 juez/c_linea.py\
	        --ui True\
	        --red_player ./cmake-build-debug/gridSensei_3\
	        --blue_player ./cmake-build-debug/geneticPlayer\
	        --iterations 100\
	        --columns 7 --rows 6 --p 21 --c 4