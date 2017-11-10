#include <vector>
#include <cassert>

#ifndef __ENUMPLAYERS__
#define __ENUMPLAYERS__

using namespace std;

enum class Players {
    NONE,
    US,
    THEM,
};
#endif

#ifndef __BOARD__
#define __BOARD__

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

#endif
