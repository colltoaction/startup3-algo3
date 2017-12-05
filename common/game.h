#ifndef STARTUP3_ALGO3_GAME_H
#define STARTUP3_ALGO3_GAME_H

#include "board.h"

class Player;

class PossibleMove;

class Game {
private:
    Board board_;
    int c;
    int p;
    Players currentPlayer_ = Players::NONE;
    Players nextPlayer_ = Players::NONE;
    Players winner_ = Players::NONE;
    bool gameFinished_ = false;

    void swapPlayers();

public:
    Game(int rows, int columns, int c, int p);

    void startWith(Players player);

    void addPiece(const int column);

    void removePiece(const int column);

    bool gameFinished();

    Board board() const;

    Players currentPlayer() const;

    Players winner() const;

    int cToWin() const;

    int remainingPieces() const;

    pair<int,int> playMatch(Player& playerOne, Player& playerTwo);

    vector<PossibleMove> possibleMoves();

    pair<int, int> playAgainstCatedra(Player &playerOne);
};

#endif //STARTUP3_ALGO3_GAME_H
