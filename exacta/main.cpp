#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>

using namespace std;

typedef int column;

typedef int move;

typedef int score;

class Game {
public:
    Game Game(int rows, int columns, int p, int c);
    vector<column> possibleMoves();
    void addPiece(column col, int player);
    void removePiece(column col);

private:
    int rows;
    int columns;
    int p;
    int c;
    vector<vector<int>> board;
    vector<int> lowestFreeCell;

    friend class Player;
};

Game::Game(int rows, int columns, int p, int c):
        rows(rows),
        columns(columns),
        p(p),
        c(c),
        board(rows, vector<int>(columns, 0)),
        lowestFreeCell(columns, rows-1) {

}

class Player {
public:
    pair<score, move> minimax(int placedPieces, int maximizingPlayer);
    int bestMove();
    Player Player(Game game);
private:
    column bestMove;
    Game game;
};

void Game::addPiece(column col, int player) {
    // hay que chequear primero
    assert(lowestFreeCell.at(col) > -1);

    int i = lowestFreeCell.at(col);
    board.at(i)[col] = player;
    lowestFreeCell[col]--;
}

void Game::removePiece(column col) {
    assert(lowestFreeCell.at(col) < rows - 1);

    int i = lowestFreeCell.at(col) + 1;
    board.at(i)[col] = 0;
    lowestFreeCell[col]++;
}

vector<column> Game::possibleMoves() {
    vector<column> possible;
    for (column col = 0; col < columns; ++col) {
        if (lowestFreeCell.at(col) > -1) {
            possible.push_back(col);
        }
    }

    return possible;
}

int Player::bestMove() {
    return bestMove;
}



pair <score, move> Player::minimax(int placedPieces, int maximizingPlayer) {
    if (placedPieces == game.p) {
        return;
    }

    int winner = game.checkGame();
    if (winner != 0){
        return make_pair(winner,0);
    }


    if (maximizingPlayer == 1){
        int bestValue = -INFINITY;
        int bestMove = 0;
        for (column col : game.possibleMoves()) {
            game.addPiece(col,1);
            pair <score, move> value= minimax(placedPieces +1, -1);
            if (bestValue < value.first){
                bestValue = value.first;
                bestMove = col;
            }
            game.removePiece(col);
        }
        return make_pair(bestValue, bestMove);
    }
    else {
        int bestValue = +INFINITY;
        int bestMove = 0;
        for (int col = 0; col < game.columns; ++col) {
            game.addPiece(col,-1);
            pair <score, move> value= minimax(placedPieces, 1);
            if (bestValue > value.first){
                bestValue = value.first;
                bestMove = col;
            }
            game.removePiece(col);
        }
        return make_pair(bestValue, bestMove);
    }
//
//    01 function minimax(node, depth, maximizingPlayer)
//    02     if depth = 0 or node is a terminal node
//    03         return the heuristic value of node
//
//    04     if maximizingPlayer
//        05         bestValue := −∞
//    06         for each child of node
//    07             v := minimax(child, depth − 1, FALSE)
//    08             bestValue := max(bestValue, v)
//    09         return bestValue
//
//    10     else    (* minimizing player *)
//    11         bestValue := +∞
//    12         for each child of node
//    13             v := minimax(child, depth − 1, TRUE)
//    14             bestValue := min(bestValue, v)
//    15         return bestValue
}

Player::Player(Game game): game(game), bestMove(0){

}

int main() {
    int rows, columns, p, c;
    cin >> rows >> columns >> p >> c;

    string input;
    cin >> input;

    Game game = Game(rows,columns,p,c);
    Player player = Player(game);

    int placedPieces = 0;

    while(input != "perdiste" ||input != "ganaste"){
        if (input =="vos"){
            cout << player.minimax(placedPieces,1).second;
        }
        else {
            game.addPiece(stoi(input),-1);
            cout << player.minimax(placedPieces,1).second;
        }
        placedPieces++;
        cin >> input;
    }
    return 0;
}