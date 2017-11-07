#include <random>
#include "judge.h"

class PlayerRandom : public Player {
private:
    std::random_device rd;
    std::mt19937 generator = std::mt19937(rd());

public:
    int nextMove(Game& game) {
        auto moves = game.board().possibleMoves();
        std::uniform_int_distribution<int> do_move(0, moves.size() - 1);
        int m = do_move(generator);
        return moves.at(m);
    }
};
