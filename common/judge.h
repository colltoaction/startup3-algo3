#include <algorithm>
#include <chrono>
#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

using namespace std;
class Game;

class Player {
public:
    virtual int nextMove(Game& game) = 0;
};

enum class Players {
    NONE,
    US,
    THEM,
};

class Board {
private:
    int rows_;
    int columns_;
    vector<vector<Players>> matrix;
    vector<int> lowestFreeCell_;
    int playedPieces_ = 0;
public:
    Board(int rows, int columns)
        : rows_(rows)
        , columns_(columns)
        , matrix(rows, vector<Players>(columns, Players::NONE))
        , lowestFreeCell_(columns, rows - 1) {
    }

    int addPiece(const int column, Players player) {
        // Si la celda más baja desocupada es -1, quiere decir
        // que la columna está llena y no es válido poner fichas ahí.
        // cerr << column << endl;
        assert(lowestFreeCell_.at(column) > -1);
        ++playedPieces_;

        int row = lowestFreeCell_.at(column);
        // cerr << "add: (" << row << ", " << column << ") = " << (player == Players::US ? "US" : "EM") << endl;
        matrix.at(row).at(column) = player;
        lowestFreeCell_.at(column)--;
        // printBoard();
        return row;
    }

    Players removePiece(const int column) {
        // Si no hay fichas en la columna (es decir, la celda desocupada
        // más baja es la última), no es válido sacar una ficha.
        assert(lowestFreeCell_.at(column) < rows_ - 1);
        --playedPieces_;

        int row = lowestFreeCell_.at(column) + 1;
        Players player = matrix.at(row).at(column);
        matrix.at(row).at(column) = Players::NONE;
        lowestFreeCell_.at(column)++;
        // cerr << "rem: (" << row << ", " << column << ") = " << (player == Players::US ? "US" : "EM") << endl;
        return player;
    }

    vector<int> possibleMoves() const {
        vector<int> possible;
        for (int col = 0; col < columns_; ++col) {
            if (lowestFreeCell_.at(col) > -1) {
                possible.push_back(col);
            }
        }

        return possible;
    }

    bool full() {
        assert(playedPieces_ <= columns_ * rows_);
        return playedPieces_ == columns_ * rows_;
    }

    int playedPieces() const {
        return playedPieces_;
    }

    int rows() const {
        return rows_;
    }

    int columns() const {
        return columns_;
    }

    int lowestFreeCell(int col) {
        return lowestFreeCell_.at(col);
    }

    bool positionIsInLine(const int i, const int j, const int k, const Players player) {
        // Dice si la posición (i, j) forma parte de una línea de K fichas del jugador player,
        // ya sea porque está formando un K en línea propio, porque está bloqueando un K en línea
        // del oponente o porque tiene K espacios libres.
        // La razón por la que busca K-1 en vez de K es que se incluye a la celda actual,
        // y en algunos casos ésta podría no tener que sumarse.
        // Buscar K-1 engloba todos los casos.

        int downwards = 0, leftwards = 0, rightwards = 0; // Cuentan las fichas en vertical y horizontal
        int leftUpwards = 0, leftDownwards = 0, rightUpwards = 0, rightDownwards = 0; // Cuentan las fichas en diagonal
        int row = i + 1, col = j - 1; // Recorren la matriz

        while (row < rows_ && matrix.at(row).at(j) == player) {
            // Cuenta las fichas de player mirando para abajo.
            ++downwards;
            ++row;
        }

        if (downwards >= k - 1) {
            // K en línea vertical.
            return true;
        }

        row = i; // Vuelve a i para mirar a la izquierda y a la derecha.

        while (col >= 0 && matrix.at(i).at(col) == player) {
            // Cuenta las fichas de player mirando para la izquierda.
            ++leftwards;
            --col;
        }

        col = j + 1; // Vuelve a j + 1 para mirar a la derecha.
        // Es j + 1 en vez de j para no contar matrix[i,j].

        while (col < columns_ && matrix.at(i).at(col) == player) {
            // Cuenta las fichas de player mirando para la derecha.
            ++rightwards;
            ++col;
        }

        if (leftwards + rightwards >= k - 1) {
            // K en línea horizontal.
            return true;
        }

        // Vuelve a (i - 1, j - 1) para mirar las diagonales, empezando por la superior izquierda.
        row = i - 1;
        col = j - 1;

        while (row >= 0 && col >= 0 && matrix.at(row).at(col) == player) {
            // Cuenta las fichas de player mirando en diagonal para arriba a la izquierda.
            ++leftUpwards;
            --row;
            --col;
        }

        row = i + 1; // Vuelve a i + 1 para mirar en diagonal para abajo a la derecha.
        col = j + 1;

        while (row < rows_ && col < columns_ && matrix.at(row).at(col) == player) {
            ++rightDownwards;
            ++row;
            ++col;
        }

        if (leftUpwards + rightDownwards >= k - 1) {
            // K en una diagonal.
            return true;
        }

        row = i + 1; // Vuelve a (i + 1, j - 1) para mirar la diagonal en el otro ángulo.
        col = j - 1;

        while (row < rows_ && col >= 0 && matrix.at(row).at(col) == player) {
            // Cuenta las fichas de player mirando en diagonal para abajo a la izquierda.
            ++leftDownwards;
            ++row;
            --col;
        }

        row = i - 1; // Vuelve a i - 1 para mirar en diagonal para arriba a la derecha.
        col = j + 1;

        while (row >= 0 && col < columns_ && matrix.at(row).at(col) == player) {
            ++rightUpwards;
            --row;
            ++col;
        }

        if (leftDownwards + rightUpwards >= k - 1) {
            // K en la otra diagonal.
            return true;
        }

        return false; // No contó K en línea en ninguna dirección.
    }

    int numberOfLinesOfLengthK(const int i, const int j, const int k, const Players player) {
        // Similar a la función anterior, pero cuenta la cantidad de líneas formadas.

        int downwards = 0, leftwards = 0, rightwards = 0; // Cuentan las fichas en vertical y horizontal
        int leftUpwards = 0, leftDownwards = 0, rightUpwards = 0, rightDownwards = 0; // Cuentan las fichas en diagonal
        int row = i + 1, col = j - 1; // Recorren la matriz
        int result = 0; //Cantidad de líneas

        while (row < rows_ && matrix.at(row).at(j) == player) {
            // Cuenta las fichas de player mirando para abajo.
            ++downwards;
            ++row;
        }

        if (downwards >= k - 1) {
            // K en línea vertical.
            ++result;
        }

        row = i; // Vuelve a i para mirar a la izquierda y a la derecha.

        while (col >= 0 && matrix.at(i).at(col) == player) {
            // Cuenta las fichas de player mirando para la izquierda.
            ++leftwards;
            --col;
        }

        col = j + 1; // Vuelve a j + 1 para mirar a la derecha.
        // Es j + 1 en vez de j para no contar matrix[i,j].

        while (col < columns_ && matrix.at(i).at(col) == player) {
            // Cuenta las fichas de player mirando para la derecha.
            ++rightwards;
            ++col;
        }

        if (leftwards + rightwards >= k - 1) {
            // K en línea horizontal.
            ++result;
        }

        // Vuelve a (i - 1, j - 1) para mirar las diagonales, empezando por la superior izquierda.
        row = i - 1;
        col = j - 1;

        while (row >= 0 && col >= 0 && matrix.at(row).at(col) == player) {
            // Cuenta las fichas de player mirando en diagonal para arriba a la izquierda.
            ++leftUpwards;
            --row;
            --col;
        }

        row = i + 1; // Vuelve a i + 1 para mirar en diagonal para abajo a la derecha.
        col = j + 1;

        while (row < rows_ && col < columns_ && matrix.at(row).at(col) == player) {
            ++rightDownwards;
            ++row;
            ++col;
        }

        if (leftUpwards + rightDownwards >= k + 1) {
            // K en una diagonal.
            ++result;
        }

        row = i + 1; // Vuelve a (i + 1, j - 1) para mirar la diagonal en el otro ángulo.
        col = j - 1;

        while (row < rows_ && col >= 0 && matrix.at(row).at(col) == player) {
            // Cuenta las fichas de player mirando en diagonal para abajo a la izquierda.
            ++leftDownwards;
            ++row;
            --col;
        }

        row = i - 1; // Vuelve a i - 1 para mirar en diagonal para arriba a la derecha.
        col = j + 1;

        while (row >= 0 && col < columns_ && matrix.at(row).at(col) == player) {
            ++rightUpwards;
            --row;
            ++col;
        }

        if (leftDownwards + rightUpwards >= k + 1) {
            // K en la otra diagonal.
            ++result;
        }

        return result;
    }

    int suicideMove(const int i, const int j, const int k) {
        // Dice si al ubicar una ficha en la posición (i, j) le estamos dejando servido
        // un K en línea al oponente.
        if (i == 0) {
            return false;
        } else {
            return positionIsInLine(i-1, j, k, Players::THEM);
        }
    }

    int antiSuicideMove(const int i, const int j, const int k) {
        // Dice si al ubicar una ficha en la posición (i, j) estamos dejando
        // un posible 4 en línea para nosotros.
        if (i == 0) {
            return false;
        } else {
            return positionIsInLine(i-1, j, k, Players::US);
        }
    }

    int numberOfNeighbours(const int i, const int j, const Players player) {
        int res = 0;

        if (j-1 >= 0 && matrix.at(i).at(j-1) == player) {
            ++res;
        }
        if (i-1 >= 0 && j-1 >= 0 && matrix.at(i-1).at(j-1) == player) {
            ++res;
        }
        if (i-1 >= 0 && matrix.at(i-1).at(j) == player) {
            ++res;
        }
        if (i-1 >= 0 && j+1 < columns_ && matrix.at(i-1).at(j+1) == player) {
            ++res;
        }
        if (j+1 < columns_ && matrix.at(i).at(j+1) == player) {
            ++res;
        }
        if (i+1 < rows_ && j+1 < columns_ && matrix.at(i+1).at(j+1) == player) {
            ++res;
        }
        if (i+1 < rows_ && matrix.at(i+1).at(j) == player) {
            ++res;
        }
        if (i+1 < rows_ && j-1 >= 0 && matrix.at(i+1).at(j-1) == player) {
            ++res;
        }

        return res;
    }

    int piecesInRow(const int i, const Players player) {
        int res = 0;
        for (int j = 0; j < columns_; ++j) {
            if (matrix.at(i).at(j) == player) {
                ++res;
            }
        }
        return res;
    }

    int piecesInColumn(const int j, const Players player) {
        int res = 0;
        for (int i = 0; i < rows_; ++i) {
            if (matrix.at(i).at(j) == player) {
                ++res;
            }
        }
        return res;
    }

    int piecesInUpperLeftDiagonal(const int i, const int j, const Players player) {
        int res = 0;
        int row = i, col = j;
        while (row >= 0 && col >= 0) {
            if (matrix.at(row).at(col) == player) {
                ++res;
            }
            --row;
            --col;
        }
        row = i + 1; // mira lo que falta de la diagonal
        col = j + 1;
        while (row < rows_ && col < columns_) {
            if (matrix.at(row).at(col) == player) {
                ++res;
            }
            ++row;
            ++col;
        }
        return res;
    }

    int piecesInLowerLeftDiagonal(const int i, const int j, const Players player) {
        int res = 0;
        int row = i, col = j;
        while (row < rows_ && col >= 0) {
            if (matrix.at(row).at(col) == player) {
                ++res;
            }
            ++row;
            --col;
        }
        row = i - 1; // mira lo que falta de la diagonal
        col = j + 1;
        while (row >= 0 && col < columns_) {
            if (matrix.at(row).at(col) == player) {
                ++res;
            }
            --row;
            ++col;
        }
        return res;
    }

    int columnHeight(int col) {
        return rows_ - 1 - lowestFreeCell_.at(col);
    }

    int distanceToPiece(int i, int j, Players player) {
        int downwards = 1, leftwards = 1, rightwards = 1; // Cuentan las fichas en vertical y horizontal
        int leftUpwards = 1, leftDownwards = 1, rightUpwards = 1, rightDownwards = 1; // Cuentan las fichas en diagonal
        int row = i + 1, col = j - 1; // Recorren la matriz
        int result = rows_ + columns_;

        while (row < rows_ && matrix.at(row).at(j) != player) {
            // Cuenta las fichas de player mirando para abajo.
            ++downwards;
            ++row;
        }

        if (downwards < result) result = downwards;

        row = i; // Vuelve a i para mirar a la izquierda y a la derecha.

        while (col >= 0 && matrix.at(i).at(col) != player) {
            // Cuenta las fichas de player mirando para la izquierda.
            ++leftwards;
            --col;
        }

        if (leftwards < result) result = leftwards;

        col = j + 1; // Vuelve a j + 1 para mirar a la derecha.
        // Es j + 1 en vez de j para no contar matrix[i,j].

        while (col < columns_ && matrix.at(i).at(col) != player) {
            // Cuenta las fichas de player mirando para la derecha.
            ++rightwards;
            ++col;
        }

        if (rightwards < result) result = rightwards;

        // Vuelve a (i - 1, j - 1) para mirar las diagonales, empezando por la superior izquierda.
        row = i - 1;
        col = j - 1;

        while (row >= 0 && col >= 0 && matrix.at(row).at(col) == player) {
            // Cuenta las fichas de player mirando en diagonal para arriba a la izquierda.
            ++leftUpwards;
            --row;
            --col;
        }

        if (leftUpwards < result) result = leftUpwards;

        row = i + 1; // Vuelve a i + 1 para mirar en diagonal para abajo a la derecha.
        col = j + 1;

        while (row < rows_ && col < columns_ && matrix.at(row).at(col) != player) {
            ++rightDownwards;
            ++row;
            ++col;
        }

        if (rightDownwards < result) result = rightDownwards;

        row = i + 1; // Vuelve a (i + 1, j - 1) para mirar la diagonal en el otro ángulo.
        col = j - 1;

        while (row < rows_ && col >= 0 && matrix.at(row).at(col) == player) {
            // Cuenta las fichas de player mirando en diagonal para abajo a la izquierda.
            ++leftDownwards;
            ++row;
            --col;
        }

        if (leftDownwards < result) result = leftDownwards;

        row = i - 1; // Vuelve a i - 1 para mirar en diagonal para arriba a la derecha.
        col = j + 1;

        while (row >= 0 && col < columns_ && matrix.at(row).at(col) == player) {
            ++rightUpwards;
            --row;
            ++col;
        }

        if (rightUpwards < result) result = rightUpwards;

        return result;
    }

    void printBoard(){
        cerr << endl;
        for(int i = 0; i < rows_; ++i){
            cerr << '|';
            for (int j = 0; j < columns_; ++j){
                char c = '.';
                if(matrix.at(i).at(j) == Players::US) c='O';
                if(matrix.at(i).at(j) == Players::THEM) c='*';
                if(matrix.at(i).at(j) == Players::NONE) c=' ';
                cerr << c <<"|";
            }
            cerr << endl;
        }
    }

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
