#ifndef STARTUP3_ALGO3_POSSIBLE_MOVE_H
#define STARTUP3_ALGO3_POSSIBLE_MOVE_H

#include "game.h"

class PossibleMove {
private:
    Game& game;
    int col;
    int row_;
public:
    PossibleMove(Game& game, int col)
            : game(game)
            , col(col)
            , row_(game.board().lowestFreeCell(col)) {
    }

    int move() const {
        return col;
    }

    int row() const {
        return row_;
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

    void in() {
        game.addPiece(col);
    }

    void out() {
        game.removePiece(col);
    }
};

#endif //STARTUP3_ALGO3_POSSIBLE_MOVE_H
