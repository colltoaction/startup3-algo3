#include <random>
#include "../genetico/genome.h"

#ifndef __GAME__
#define __GAME__

class Player;

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

#endif

#ifndef __PLAYER__
#define __PLAYER__

class Player {
public:
    virtual int nextMove(Game& game) = 0;
};

int minimax(const PossibleMove& node, int depth, bool maximizingPlayer) {
    if (depth == 0 || node.isTerminal()) {
        return node.heuristic();
    }

    if (maximizingPlayer) {
        int bestValue = std::numeric_limits<int>::min();
        for (PossibleMove child : node.children()) {
            // in y out son trampas para poder agregar y sacar fichas en el tablero
            child.in();
            int v = minimax(child, depth - 1, false);
            bestValue = std::max(bestValue, v);
            child.out();
        }

        return bestValue;
    }
    else {
        int bestValue = std::numeric_limits<int>::max();
        for (PossibleMove child : node.children()) {
            // in y out son trampas para poder agregar y sacar fichas en el tablero
            child.in();
            int v = minimax(child, depth - 1, true);
            bestValue = std::min(bestValue, v);
            child.out();
        }

        return bestValue;
    }
}

class PlayerMinimax : public Player {
public:
    int nextMove(Game& game) {
         // TODO traer p del juego

        high_resolution_clock::time_point beginMove = high_resolution_clock::now();

        auto moves = PossibleMove(game, -1).children(); // -1 ya que no se usa ese valor
        auto max = max_element(moves.begin(), moves.end(),
            [game](const PossibleMove& m1, const PossibleMove& m2) {
                m1.in();
                auto res1 = minimax(m1, game.remainingPieces(), false);
                m1.out();
                m2.in();
                auto res2 = minimax(m2, game.remainingPieces(), false);
                m2.out();
                return res1 < res2;
            }
            );

        high_resolution_clock::time_point endMove = high_resolution_clock::now();

        duration<double> timeFindingMove = duration_cast< duration<double> >(endMove - beginMove);

        assert(max != moves.end()); // encontró alguno

        std::cerr << game.board().possibleMoves().size() << "; "<< timeFindingMove.count() << "; "<< game.board().columns() <<"; "<< game.board().rows() <<"; " << game.cToWin()<<endl;

        return max->move();
    }
};

int minimax(const PossibleMove& node, int depth, bool maximizingPlayer) {
    if (depth == 0 || node.isTerminal()) {
        return node.heuristic();
    }

    if (maximizingPlayer) {
        int bestValue = std::numeric_limits<int>::min();
        for (PossibleMove child : node.children()) {
            // in y out son trampas para poder agregar y sacar fichas en el tablero
            child.in();
            int v = minimax(child, depth - 1, false);
            bestValue = std::max(bestValue, v);
            child.out();
        }

        return bestValue;
    }
    else {
        int bestValue = std::numeric_limits<int>::max();
        for (PossibleMove child : node.children()) {
            // in y out son trampas para poder agregar y sacar fichas en el tablero
            child.in();
            int v = minimax(child, depth - 1, true);
            bestValue = std::min(bestValue, v);
            child.out();
        }

        return bestValue;
    }
}

class PlayerMinimax_n : public Player {
private:
    int plays;
public:
    PlayerMinimax_n(int n) : plays(n){}
    int nextMove(Game& game) {
        auto moves = PossibleMove(game, -1).children();
        int bestResult = -1;
        vector<int> losers;
        int max = 0;
        for (unsigned int i = 0; i < moves.size();++i) {
            moves.at(i).in();
            auto v = minimax(moves.at(i), 2*plays-2, false);
            moves.at(i).out();
            if(v == -1) losers.push_back(i);
            if(bestResult<v){
                bestResult = v;
                max = i;
            }

        }

        if(bestResult == 0) {
            bool loser = false;
            do{
                // binomial_distribution<int> randomMove(moves.size()/2, 0.5);
                // max = randomMove.operator();
                max = rand() % moves.size();
                for (unsigned int i = 0; i < losers.size(); ++i){
                    if(losers.at(i) == max){
                        loser = true;
                        break;
                    }
                    if(losers.at(i) > max){
                        loser= false;
                        break;
                    }
                }
            } while (loser);
        }

        return moves.at(max).move();
    }
};

class PlayerGenetic : public Player {
private:
    Genome g;
public:
    PlayerGenetic(Genome g) : g(g) {}
    int nextMove(Game& game) {

        auto moves = game.board().possibleMoves();
        auto bestCol = max_element(moves.begin(), moves.end(),
            [this, game](const int& m1, const int& m2) {
                return g.activate(game.board(), m1) < g.activate(game.board(), m2);
            }
        );
        assert(bestCol != moves.end()); // encontró alguno

        // cerr << "PossibleMoves: " << endl;
        // for (auto i : game.board().possibleMoves())
        // {
        //     cerr << i <<" ";
        // }

        // cerr << endl << "Genetic elige: " << *bestCol << endl;

        return *bestCol;
    }
};

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

#endif
