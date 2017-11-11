#ifndef STARTUP3_ALGO3_JUDGE_PROXY_H
#define STARTUP3_ALGO3_JUDGE_PROXY_H

#include <algorithm>
#include <chrono>
#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include "board.h"
#include "game.h"
#include "player.h"

using namespace std;

class JudgeProxy {
private:
    bool _keepPlaying = true;
public:
    void play(Player& player) {
        string color;
        cin >> color;

        if (color == "salir") {
            cout << "listo" << endl;
            _keepPlaying = false;
            return;
        }

        string color_otro;
        cin >> color_otro;

        // por ahora ignoramos los colores
        gameMain(player);
    }

    bool keepPlaying() {
        return _keepPlaying;
    }

    void gameMain(Player& player) {
        int rows, columns, p, c;
        cin >> columns >> rows >> c >> p;

        string input;
        cin >> input;

        Game game = Game(rows, columns, c, p);

        if (input == "vos") {
            game.startWith(Players::US);
            // column move = player.calculateMove(SolutionType::MINIMAX, placedPieces);
            // player.addPiece(move, Players::US);
            int move = player.nextMove(game);

            game.addPiece(move);
            cout << move << endl;
            // placedPieces++;
        }
        else {
            game.startWith(Players::THEM);
        }

        while (true) {
            cin >> input;
            if (input == "ganaste" || input == "perdiste" || input == "empataron") {
                cerr << "se terminó: " << input << endl << flush;
                // terminó este juego
                game.board().printBoard();
                return;
            }

            // cerr << "input: " << input << endl << flush;

            // leo jugada del otro
            int opponentMove = stoi(input);
            // cerr << "opponentMove: " << opponentMove << endl << flush;
            game.addPiece(opponentMove);


            assert(!game.gameFinished());

            // cerr << "addpiece" << endl << flush;
            // player.addPiece(opponentMove, Players::THEM);
            // player.printBoard();

            // int move = player.calculateMove(SolutionType::MINIMAX, placedPieces);
            // player.addPiece(move, Players::US);
            // placedPieces++;
            // calculo la jugada
            int move = player.nextMove(game);

            game.addPiece(move);
            cout << move << endl;
        }
    }
};

#endif //STARTUP3_ALGO3_JUDGE_PROXY_H
