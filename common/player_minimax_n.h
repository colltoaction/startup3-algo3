#ifndef STARTUP3_ALGO3_PLAYER_MINIMAX_N_H
#define STARTUP3_ALGO3_PLAYER_MINIMAX_N_H

#include <random>
#include "game.h"
#include "minimax.h"
#include "possible_move.h"

int randomChoice(const vector<int> &drawers) {
    random_device rd;
    mt19937 generator = mt19937(rd());
    uniform_int_distribution<int> dist(0, drawers.size() - 1);

    return drawers[dist(generator)];
}

class PlayerMinimax_n : public Player {
private:
    int plays;
public:
    PlayerMinimax_n(int n) : plays(n){}
    int nextMove(Game& game) {
        vector<int> drawers;
        vector<int> losers;
        auto moves = PossibleMove(game, -1).children(); // -1 ya que no se usa ese valor
        assert(!moves.empty());
        for (auto move : moves) {
            move.in();
            auto res = minimax(move, plays, false);
            move.out();

            if (res == 1) {
                return move.move();
            }

            if (res == 0) {
                drawers.push_back(move.move());
            }
            else if (res == -1) {
                losers.push_back(move.move());
            }
            else {
                // hay un error
                assert(false);
            }
        }

        if (!drawers.empty()) {
            return randomChoice(drawers);
        }

        assert(!losers.empty());
        return randomChoice(losers);
    }
};

#endif //STARTUP3_ALGO3_PLAYER_MINIMAX_N_H
