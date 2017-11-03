cmake_minimum_required(VERSION 3.8)
project(startup3_algo3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        exacta/game.h
        exacta/main.cpp
        exacta/player.h
        juez/c_linea.py
        juez/constants.py
        juez/GraphicalBoard.py
        juez/HumanPlayer.py
        juez/LogicalBoard.py
        juez/Makefile
        juez/PlayerCommunicator.py
        juez/PlayerController.py
        juez/random_player.cpp
        juez/RandomPlayer.py
        juez/Referee.py
        Makefile)

add_executable(startup3_algo3 ${SOURCE_FILES})