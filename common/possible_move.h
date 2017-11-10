#ifndef STARTUP3_ALGO3_POSSIBLE_MOVE_H
#define STARTUP3_ALGO3_POSSIBLE_MOVE_H

#include "game.h"

class PossibleMove {
private:
    Game& game;
    int col;
public:
    PossibleMove(Game& game, int col)
            : game(game)
            , col(col) {
    }

    int move() const {
        return col;
    }

    bool isTerminal() const {
        return game.gameFinished();
    }

    int heuristic() const {
        if (!game.gameFinished()) {
            return 0;
        }

        switch (game.winner()) {
            case Players::US:
                // cerr << ">> winner US in col " << col << endl;
                return 1;
            case Players::THEM:
                // cerr << ">> winner EM in col " << col << endl;
                return -1;
            case Players::NONE:
                // cerr << ">> winner NO in col " << col << endl;
                return 0;
        }
    }

    vector<PossibleMove> children() const {
        vector<PossibleMove> possible;
        for (int col : game.board().possibleMoves()) {
            possible.emplace_back(game, col);
        }

        return possible;
    }

    void in() const {
        game.addPiece(col);
    }
    void out() const {
        game.removePiece(col);
    }
};

#endif //STARTUP3_ALGO3_POSSIBLE_MOVE_H
