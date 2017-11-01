#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;


enum class Players {
    NONE,
    US,
    THEM,
};

class Board {
private:
    int rows;
    int columns;
    vector<vector<Players>> matrix;
    vector<int> lowestFreeCell;
public:
    Board(int rows, int columns)
        : rows(rows)
        , columns(columns)
        , matrix(rows, vector<Players>(columns, Players::NONE))
        , lowestFreeCell(columns, rows - 1) {
    }

    int addPiece(const int column, Players player) {
        // Si la celda más baja desocupada es -1, quiere decir
        // que la columna está llena y no es válido poner fichas ahí.
        assert(lowestFreeCell.at(column) > -1);
        
        int row = lowestFreeCell.at(column);
        cerr << "row: " << row << flush;
        matrix.at(row).at(column) = player;
        lowestFreeCell.at(column)--;
        return row;
    }

    Players removePiece(const int column) {
        // Si no hay fichas en la columna (es decir, la celda desocupada
        // más baja es la última), no es válido sacar una ficha.
        assert(lowestFreeCell.at(column) < rows - 1);
    
        int row = lowestFreeCell.at(column) + 1;
        Players player = matrix.at(row).at(column);
        matrix.at(row).at(column) = Players::NONE;
        lowestFreeCell.at(column)++;
        return player;
    }

    vector<int> possibleMoves() const {
        vector<int> possible;
        for (int col = 0; col < columns; ++col) {
            if (lowestFreeCell.at(col) > -1) {
                possible.push_back(col);
            }
        }
    
        return possible;
    }

    bool positionIsInLine(const int i, const int j, const int c) {
        Players player = matrix.at(i).at(j);
    
        int downwards = 0, leftwards = 0, rightwards = 0; // Cuentan las fichas en vertical y horizontal
        int leftUpwards = 0, leftDownwards = 0, rightUpwards = 0, rightDownwards = 0; // Cuentan las fichas en diagonal
        int row = i, col = j; // Recorren la matriz
    
        while (row < rows && matrix.at(row).at(j) == player) {
            // Cuenta las fichas de player mirando para abajo.
            ++downwards;
            ++row;
        }
    
        if (downwards >= c) {
            // C en línea vertical.
            return true;
        }
    
        row = i; // Vuelve a i para mirar las diagonales.
    
        while (col >= 0 && matrix.at(i).at(col) == player) {
            // Cuenta las fichas de player mirando para la izquierda.
            ++leftwards;
            --col;
        }
    
        col = j + 1; // Vuelve a j + 1 para mirar a la derecha.
        // Es j + 1 en vez de j para no contar dos veces matrix[i,j]
        // en caso de que esté ocupada por player.
    
        while (col < columns && matrix.at(i).at(col) == player) {
            // Cuenta las fichas de player mirando para la derecha.
            ++rightwards;
            ++col;
        }
    
        if (leftwards + rightwards >= c) {
            // C en línea horizontal.
            return true;
        }
    
        col = j; // Vuelve a j para mirar las diagonales.
    
        while (row >= 0 && col >= 0 && matrix.at(row).at(col) == player) {
            // Cuenta las fichas de player mirando en diagonal para arriba a la izquierda.
            ++leftUpwards;
            --row;
            --col;
        }
    
        row = i + 1; // Vuelve a i + 1 para mirar en diagonal para abajo a la derecha.
        col = j + 1;
    
        while (row < rows && col < columns && matrix.at(row).at(col) == player) {
            ++rightDownwards;
            ++row;
            ++col;
        }
    
        if (leftUpwards + rightDownwards >= c) {
            // C en una diagonal.
            return true;
        }
    
        row = i; // Vuelve a i para mirar la diagonal en el otro ángulo.
        col = j;
    
        while (row < rows && col >= 0 && matrix.at(row).at(col) == player) {
            // Cuenta las fichas de player mirando en diagonal para abajo a la izquierda.
            ++leftDownwards;
            ++row;
            --col;
        }
    
        row = i - 1; // Vuelve a i - 1 para mirar en diagonal para arriba a la derecha.
        col = j + 1;
    
        while (row >= 0 && col < columns && matrix.at(row).at(col) == player) {
            ++rightUpwards;
            --row;
            ++col;
        }
    
        if (leftDownwards + rightUpwards >= c) {
            // C en la otra diagonal.
            return true;
        }
    
        return false; // No contó C en línea en ninguna dirección.
    }
};

class Game {
private:
    Board board_;
    int c;
    Players currentPlayer_;
    bool gameFinished_ = false;
public:
    Game(int rows, int columns, int c)
        : board_(rows, columns)
        , c(c)
        , currentPlayer_(Players::NONE) {
    }

    void startWith(Players player) {
        currentPlayer_ = player;
    }
    
    void addPiece(const int column) {
        assert(!gameFinished_);
        int row = board_.addPiece(column, currentPlayer_);
        cerr << "board_.addPiece: " << column << endl << flush;
        cerr << "player: " << (int)currentPlayer_ << endl << flush;

        if (board_.positionIsInLine(row, column, c)) {
            gameFinished_ = true;
            // retornamos para que el ganador quede en currentPlayer_
            return;
        }

        // alterna el jugador en cada jugada
        currentPlayer_ = currentPlayer_ == Players::US ? Players::THEM : Players::US;
    }

    void removePiece(const int column) {
        cerr << "board_.removePiece column: " << column << endl << flush;
        Players player = board_.removePiece(column);
        cerr << "player: " << (int)player << endl << flush;
        assert(gameFinished() || player != currentPlayer_);
        gameFinished_ = false;

        // alterna el jugador en cada jugada
        currentPlayer_ = currentPlayer_ == Players::US ? Players::THEM : Players::US;
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

    // solo se llama cuando isTerminal, entonces currentPlayer es el ganador
    int heuristic() const {
        Players winner = game.currentPlayer();
        if (winner == Players::US) {
            return 1;
        }

        if (winner == Players::THEM) {
            return -1;
        }

        // TODO empate
        return 0;
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

int minimax(const PossibleMove& node, int depth, bool maximizingPlayer) {
    if (depth == 0 || node.isTerminal()) {
        return node.heuristic();
    }

    if (maximizingPlayer) {
        int bestValue = std::numeric_limits<int>::min();
        // in y out son trampas para poder agregar y sacar fichas en el tablero
        node.in();
        for (PossibleMove child : node.children()) {
            int v = minimax(child, depth - 1, false);
            bestValue = std::max(bestValue, v);
        }
        node.out();

        return bestValue;
    }
    else {
        int bestValue = std::numeric_limits<int>::max();
        // in y out son trampas para poder agregar y sacar fichas en el tablero
        node.in();
        for (PossibleMove child : node.children()) {
            int v = minimax(child, depth - 1, true);
            bestValue = std::min(bestValue, v);
        }
        node.out();

        return bestValue;
    }
}

class Player {
public:
    int nextMove(Game& game) {
        // TODO traer p del juego
        int p = 4;
        auto moves = PossibleMove(game, -1).children(); // -1 ya que no se usa ese valor
        auto max = max_element(moves.begin(), moves.end(),
            [p](const PossibleMove& m1, const PossibleMove& m2) {
                return minimax(m1, p, true) < minimax(m2, p, true);
            }
        );
        assert(max != moves.end()); // encontró alguno
        return max->move();
    }
};

class JudgeProxy {
private:
    bool _keepPlaying = true;
public:
    void play(Player player) {
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

    void gameMain(Player player) {
        int rows, columns, p, c;
        cin >> columns >> rows >> c >> p;
        // TODO usar p

        string input;
        cin >> input;

        Game game = Game(rows, columns, c);

        if (input == "vos") {
            game.startWith(Players::US);
            // column move = player.calculateMove(SolutionType::MINIMAX, placedPieces);
            // player.addPiece(move, Players::US);
            int move = player.nextMove(game);
            game.addPiece(move);
            // cout << move << endl;
            // placedPieces++;
        }
        else {
            game.startWith(Players::THEM);
        }
    
        while (true) {
            cin >> input;
            if (input == "ganaste" || input == "perdiste" || input == "empataron") {
                assert(game.gameFinished());
                cerr << "se terminó: " << input << endl << flush;
                // terminó este juego
                return;
            }
    
            // cerr << "input: " << input << endl << flush;
    
            // leo jugada del otro
            int opponentMove = stoi(input);
            cerr << "opponentMove: " << opponentMove << endl << flush;
            game.addPiece(opponentMove);
            cerr << "addpiece" << endl << flush;
            // player.addPiece(opponentMove, Players::THEM);
            // player.printBoard();
    
            // int move = player.calculateMove(SolutionType::MINIMAX, placedPieces);
            // player.addPiece(move, Players::US);
            // placedPieces++;
            // calculo la jugada
            int move = player.nextMove(game);
            game.addPiece(move);
            // player.printBoard();
            // cout << move << endl;
        }
    }
};






int main() {
    JudgeProxy judge;
    while (judge.keepPlaying()) {
        Player player;
        judge.play(player);
    }
}
