#include <algorithm>
#include <chrono>
#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <string>

using namespace std;
class Game;

class Player {
public:
    virtual int nextMove(Game& game) = 0;
};

class Game {
private:
    Board board_;
    int c;
    int p;
    Players currentPlayer_ = Players::NONE;
    Players winner_ = Players::NONE;
    bool gameFinished_ = false;
public:
    Game(int rows, int columns, int c, int p)
        : board_(rows, columns)
        , c(c)
        , p(p) {
    }

    void startWith(Players player) {
        currentPlayer_ = player;
    }

    void addPiece(const int column) {
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

        // alterna el jugador en cada jugada
        currentPlayer_ = currentPlayer_ == Players::US ? Players::THEM : Players::US;
    }

    void removePiece(const int column) {
        // cerr << "board_.removePiece column: " << column << endl << flush;
        Players player = board_.removePiece(column);
        // cerr << "currentPlayer_: " << (int)currentPlayer_ << endl << flush;
        // cerr << "player: " << (int)player << endl << flush;

        assert(gameFinished() || player != currentPlayer_);

        // // el jugador al que le hayamos sacado la ficha
        // currentPlayer_ = player;
        // alterna el jugador en cada jugada
        currentPlayer_ = currentPlayer_ == Players::US ? Players::THEM : Players::US;
        gameFinished_ = false;
        winner_ = Players::NONE;
    }

    bool gameFinished() {
        return gameFinished_;
    }

    Board board() const {
        return board_;
    }

    Players currentPlayer() const {
        return currentPlayer_;
    }

    Players winner() const {
        // no hay ganador si no terminó el juego
        assert(gameFinished_);
        return winner_;
    }

    int cToWin() const {
        // no hay ganador si no terminó el juego

        return c;
    }

    int remainingPieces() const {
        return 2 * p - board_.playedPieces();
    }

    pair<int,int> playMatch(Player& playerOne, Player& playerTwo) {
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
};

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
        default: assert(false);
        }
    }

    vector<PossibleMove> children() const {
        vector<PossibleMove> possible;
        for (int col : game.board().possibleMoves()) {
            possible.push_back(PossibleMove(game, col));
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
        // TODO usar p

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
