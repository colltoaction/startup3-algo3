#ifndef STARTUP3_ALGO3_GAME_CPP
#define STARTUP3_ALGO3_GAME_CPP

#include <random>
#include <chrono>
#include "player.h"
#include "possible_move.h"


Game::Game(int rows, int columns, int c, int p)
        : board_(rows, columns)
        , c(c)
        , p(p) {
}

void Game::startWith(Players player) {
    currentPlayer_ = player;
    nextPlayer_ = player == Players::US ? Players::THEM : Players::US;
}

void Game::addPiece(const int column) {
    assert(currentPlayer_ != Players::NONE);
    assert(!gameFinished_);
    int row = board_.addPiece(column, currentPlayer_);
    // cerr << "board_.addPiece: " << column << endl << flush;
    // cerr << "player: " << (int)currentPlayer_ << endl << flush;

    if (board_.positionIsInLine(row, column, c, currentPlayer_)) {
        gameFinished_ = true;
        winner_ = currentPlayer_;
    }
    else if (board_.full()) {
        gameFinished_ = true;
        winner_ = Players::NONE;
    }
    else if (remainingPieces() == 0) {
        gameFinished_ = true;
        winner_ = Players::NONE;
    }

    swapPlayers();
}

void Game::removePiece(const int column) {
    // cerr << "board_.removePiece column: " << column << endl << flush;
    Players player = board_.removePiece(column);
    // cerr << "currentPlayer_: " << (int)currentPlayer_ << endl << flush;
    // cerr << "player: " << (int)player << endl << flush;

    assert(gameFinished() || player != currentPlayer_);

    // // el jugador al que le hayamos sacado la ficha
    // currentPlayer_ = player;
    swapPlayers();
    gameFinished_ = false;
    winner_ = Players::NONE;
}

bool Game::gameFinished() {
    return gameFinished_;
}

Board Game::board() const {
    return board_;
}

Players Game::currentPlayer() const {
    return currentPlayer_;
}

Players Game::winner() const {
    // no hay ganador si no terminó el juego
    assert(gameFinished_);
    return winner_;
}

int Game::cToWin() const {
    // no hay ganador si no terminó el juego

    return c;
}

int Game::remainingPieces() const {
    return 2 * p - board_.playedPieces();
}

pair<int, int> Game::playMatch(Player& playerOne, Player& playerTwo) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine initGenerator(seed);
    uniform_real_distribution<float> initDistribution(0.0, 1.0);

    float startP = initDistribution(initGenerator);

    int numberOfMoves = 0;
    if (startP >= 0.5) {
        startWith(Players::US);
        addPiece(playerOne.nextMove(*this));
        numberOfMoves++;
    } else {
        startWith(Players::THEM);

    }

    while (!gameFinished()) {
        addPiece(playerTwo.nextMove(*this));
        numberOfMoves++;

        if(gameFinished()) break;

        addPiece(playerOne.nextMove(*this));
        numberOfMoves++;
    }


    // board().printBoard();
    // TODO manejar empate
    int didItWin = winner() == Players::US ? 1 : 0;

    return make_pair(didItWin, numberOfMoves);
}

vector<PossibleMove> Game::possibleMoves() {
    vector<PossibleMove> possible;
    for (int col : board().possibleMoves()) {
        // esto pareciera estar al revés pero así anda.
        // chequear.
        possible.emplace_back(*this, col, currentPlayer_, nextPlayer_);
    }

    return possible;
}

void Game::swapPlayers() {
    auto temp = currentPlayer_;
    currentPlayer_ = nextPlayer_;
    nextPlayer_ = temp;
}

#endif //STARTUP3_ALGO3_GAME_CPP
