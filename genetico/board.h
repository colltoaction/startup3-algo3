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
        // Agrega una ficha en la posición libre más baja de la columna column.
        // Devuelve el número de la fila en la que fue ubicada la ficha.

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

        while (row < rows && matrix.at(row).at(j) == player) {
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

        while (col < columns && matrix.at(i).at(col) == player) {
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

        while (row < rows && col < columns && matrix.at(row).at(col) == player) {
            ++rightDownwards;
            ++row;
            ++col;
        }

        if (leftUpwards + rightDownwards >= k + 1) {
            // K en una diagonal.
            return true;
        }

        row = i + 1; // Vuelve a (i + 1, j - 1) para mirar la diagonal en el otro ángulo.
        col = j - 1;

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

        if (leftDownwards + rightUpwards >= k + 1) {
            // K en la otra diagonal.
            return true;
        }

        return false; // No contó K en línea en ninguna dirección.
    }

    int amountOfLinesOfLengthK(const int i, const int j, const int k, const Players player) {
        // Similar a la función anterior, pero cuenta la cantidad de líneas formadas.

        int downwards = 0, leftwards = 0, rightwards = 0; // Cuentan las fichas en vertical y horizontal
        int leftUpwards = 0, leftDownwards = 0, rightUpwards = 0, rightDownwards = 0; // Cuentan las fichas en diagonal
        int row = i + 1, col = j - 1; // Recorren la matriz
        int result; //Cantidad de líneas

        while (row < rows && matrix.at(row).at(j) == player) {
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

        while (col < columns && matrix.at(i).at(col) == player) {
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

        while (row < rows && col < columns && matrix.at(row).at(col) == player) {
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

        if (leftDownwards + rightUpwards >= k + 1) {
            // K en la otra diagonal.
            ++result;
        }

        return result;
    }

    int amountOfNeighbours(const int i, const int j, const Players player) {
        int res = 0;

        if (matrix.at(i).at(j-1) == player) {
            ++res;
        }
        if (matrix.at(i-1).at(j-1) == player) {
            ++res;
        }
        if (matrix.at(i-1).at(j) == player) {
            ++res;
        }
        if (matrix.at(i-1).at(j+1) == player) {
            ++res;
        }
        if (matrix.at(i).at(j+1) == player) {
            ++res;
        }
        if (matrix.at(i+1).at(j+1) == player) {
            ++res;
        }
        if (matrix.at(i+1).at(j) == player) {
            ++res;
        }
        if (matrix.at(i+1).at(j-1) == player) {
            ++res;
        }

        return res;
    }
};
