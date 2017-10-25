#include <cassert>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

typedef int column;

enum class Winner {
    PLAYER2,
    DRAW,
    PLAYER1,
};

typedef int score;

class Game {
    public:
        Game(int rows, int columns, int p, int c);
        vector<column> possibleMoves();
        void addPiece(column col, Winner player);
        void removePiece(column col);
        Winner checkGame();
        bool connectC(int i, int j, Winner player);
        void printBoard();
        void printLow();
        bool isFree(column col);
        column firstFreeColumn();
    private:
        int rows;
        int columns;
        int p;
        int c;
        vector<vector<Winner>> board;
        vector<int> lowestFreeCell;

        friend class Player;
};

Game::Game(int rows, int columns, int p, int c):
    rows(rows),
    columns(columns),
    p(p),
    c(c),
    board(rows, vector<Winner>(columns, Winner::DRAW)),
    lowestFreeCell(columns, rows-1) {

    }

void Game::printLow(){
    cerr << "[ ";
    int size = lowestFreeCell.size();
    for ( int i = 0; i < size; ++i){
        cerr << lowestFreeCell.at(i) << " ";
    }
    cerr << "]";
}

bool Game::isFree(column col){
    return lowestFreeCell.at(col) > -1;
}

void Game::printBoard(){
    printLow();
    cerr << endl;
    for(int i = 0; i < rows; ++i){
        cerr << '|';
        for (int j = 0; j < columns; ++j){
            char c;
            if(board.at(i).at(j) == Winner::PLAYER1) c='O';
            if(board.at(i).at(j) == Winner::PLAYER2) c='*';
            if(board.at(i).at(j) == Winner::DRAW) c=' ';
            cerr << c <<"|";
        }
        cerr << endl;
    }
}

void Game::addPiece(column col, Winner player) {
    // Si la celda más baja desocupada es -1, quiere decir
    // que la columna está llena y no es válido poner fichas ahí.
    assert(lowestFreeCell.at(col) > -1);

    int i = lowestFreeCell.at(col);
    board.at(i).at(col) = player;
    lowestFreeCell.at(col)--;
}

void Game::removePiece(column col) {
    // Si no hay fichas en la columna (es decir, la celda desocupada
    // más baja es la última), no es válido sacar una ficha.
    assert(lowestFreeCell.at(col) < rows - 1);

    int i = lowestFreeCell.at(col) + 1;
    board.at(i).at(col) = Winner::DRAW;
    lowestFreeCell.at(col)++;
}
column Game::firstFreeColumn(){
    for (int i = 0; i < columns; ++i){
        if (isFree(i)) return i;
    }

    return -1;
}

vector<column> Game::possibleMoves() {
    vector<column> possible;
    for (column col = 0; col < columns; ++col) {
        if (isFree(col)) {
            possible.push_back(col);
        }
    }

    return possible;
}

Winner Game::checkGame() {
    // TODO
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (connectC(i, j, Winner::PLAYER1)) {
                // PLAYER1 = nosotros. ¡Chequear que esté bien!
                return Winner::PLAYER1;
            } else if (connectC(i, j, Winner::PLAYER2)) {
                return Winner::PLAYER2;
            }
        }
    }
    return Winner::DRAW;
}

bool Game::connectC(const int i, const int j, Winner player) {
    // Dice si hay C piezas en línea del jugador dado que incluyan a la posición (i, j).
    // Quiero pedirles perdón a Dijkstra, a Turing, a John von Neumann
    // y al Prof. Francisco Soulignac por estos 7 whiles separados
    // y las row y col que se resetean.

    // Cortamos rápidamente cuando la posición no tiene al jugador que se desea chequear
    if (board.at(i).at(j) != player) {
        return false;
    }

    int downwards = 0, leftwards = 0, rightwards = 0; // Cuentan las fichas en vertical y horizontal
    int leftUpwards = 0, leftDownwards = 0, rightUpwards = 0, rightDownwards = 0; // Cuentan las fichas en diagonal
    int row = i, col = j; // Recorren la matriz

    while (row < rows && board.at(row).at(j) == player) {
        // Cuenta las fichas de player mirando para abajo.
        ++downwards;
        ++row;
    }

    if (downwards >= c) {
        // C en línea vertical.
        return true;
    }

    row = i; // Vuelve a i para mirar las diagonales.

    while (col >= 0 && board.at(i).at(col) == player) {
        // Cuenta las fichas de player mirando para la izquierda.
        ++leftwards;
        --col;
    }

    col = j + 1; // Vuelve a j + 1 para mirar a la derecha.
    // Es j + 1 en vez de j para no contar dos veces board[i,j]
    // en caso de que esté ocupada por player.

    while (col < columns && board.at(i).at(col) == player) {
        // Cuenta las fichas de player mirando para la derecha.
        ++rightwards;
        ++col;
    }

    if (leftwards + rightwards >= c) {
        // C en línea horizontal.
        return true;
    }

    col = j; // Vuelve a j para mirar las diagonales.

    while (row >= 0 && col >= 0 && board.at(row).at(col) == player) {
        // Cuenta las fichas de player mirando en diagonal para arriba a la izquierda.
        ++leftUpwards;
        --row;
        --col;
    }

    row = i + 1; // Vuelve a i + 1 para mirar en diagonal para abajo a la derecha.
    col = j + 1;

    while (row < rows && col < columns && board.at(row).at(col) == player) {
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

    while (row < rows && col >= 0 && board.at(row).at(col) == player) {
        // Cuenta las fichas de player mirando en diagonal para abajo a la izquierda.
        ++leftDownwards;
        ++row;
        --col;
    }

    row = i - 1; // Vuelve a i - 1 para mirar en diagonal para arriba a la derecha.
    col = j + 1;

    while (row >= 0 && col < columns && board.at(row).at(col) == player) {
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
