#ifndef STARTUP3_ALGO3_PLAYER_ALFABETA_H
#define STARTUP3_ALGO3_PLAYER_ALFABETA_H

#include <algorithm>
#include <chrono>
#include "minimax.h"
#include "possible_move.h"

// using namespace chrono;

class PlayerAlfaBeta : public Player {
public:
    int nextMove(Game &game) {
        // high_resolution_clock::time_point beginMove = high_resolution_clock::now();

        auto moves = game.possibleMoves(); // -1 ya que no se usa ese valor

        auto max = max_element(moves.begin(), moves.end(),
                               [game](PossibleMove &m1, PossibleMove &m2) {
                                   m1.in();
                                   auto res1 = alphabeta(m1, game.remainingPieces(), -1, 1, false);
                                   m1.out();
                                   m2.in();
                                   auto res2 = alphabeta(m2, game.remainingPieces(), -1, 1, false);
                                   m2.out();
                                   return res1 < res2;
                               }
        );

        // high_resolution_clock::time_point endMove = high_resolution_clock::now();

        // duration<double> timeFindingMove = duration_cast<duration<double> >(endMove - beginMove);

        assert(max != moves.end()); // encontró alguno

        // std::cerr << game.board().possibleMoves().size() << "; " << timeFindingMove.count() << "; "
                  // << game.board().columns() << "; " << game.board().rows() << "; " << game.cToWin() << endl;

        return max->move();
    }
};

#endif //STARTUP3_ALGO3_PLAYER_MINIMAX_H
