#ifndef STARTUP3_ALGO3_POSSIBLE_MOVE_H
#define STARTUP3_ALGO3_POSSIBLE_MOVE_H

#include "game.h"

class PossibleMove {
private:
    Game& game;
    const int col;
    const int row_;
    const Players us_;
    const Players em_;
public:
    PossibleMove(Game& game, int col, Players us, Players em)
            : game(game)
            , col(col)
            , row_(game.board().lowestFreeCell(col))
            , us_(us)
            , em_(em) {
    }

    int move() const {
        return col;
    }

    int row() const {
        return row_;
    }

    Players us() const {
        return us_;
    }

    Players em() const {
        return em_;
    }

    bool isTerminal() const {
        return game.gameFinished();
    }

    int heuristic() const {
        if (!isTerminal()) {
            // llegué por depth == 0
            return 0;
        }

        if (game.winner() == us()) {
            // cerr << ">> winner US in col " << col << endl;
            return 1;
        }

        if (game.winner() == em()) {
            // cerr << ">> winner EM in col " << col << endl;
            return -1;
        }

        if (game.winner() == Players::NONE) {
            // cerr << ">> winner NO in col " << col << endl;
            return 0;
        }

        // hay un error
        assert(false);
    }

    vector<PossibleMove> children() const {
        // nos olvidamos de llamar a .in()
        assert(row_ != game.board().lowestFreeCell(col));
        return game.possibleMoves();
    }

    void in() const {
        game.addPiece(move());
    }

    void out() const {
        game.removePiece(move());
    }
};

#endif //STARTUP3_ALGO3_POSSIBLE_MOVE_H
