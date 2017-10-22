#include "game.h"

using namespace std;

enum class SolutionType{ MINIMAX, ALFA_BETA, GOLOSA};

class Player {
    public:
        column calculateMove(SolutionType h, int placedPieces);
        pair<score, column> minimax(int placedPieces, Winner maximizingPlayer);
        pair<score, column> minimaxABaux(int placedPieces, Winner maximizingPlayer, score alfa, score beta);
        pair<score, column> minimaxAB(int placedPieces);
        Player(Game game);
        void addPiece(column col, Winner player);
        void printBoard();
    private:
        Game game;
};

column Player::calculateMove(SolutionType h, int placedPieces) {
    if(h == SolutionType::MINIMAX) {
        return minimax(placedPieces, Winner::PLAYER1).second;
    }

    if(h == SolutionType::ALFA_BETA){
        return minimaxAB(placedPieces).second;
    }

    // golosa no implementada todavía
    return 0;
}

Player::Player(Game game):
    game(game) {
    }

void Player::addPiece(column col, Winner player){
    game.addPiece(col, player);
}
void Player::printBoard(){
    game.printBoard();
}

pair<score, column> Player::minimax(int placedPieces, Winner maximizingPlayer) {
    column free = game.firstFreeColumn();
    Winner winner = game.checkGame();
    if (winner == Winner::PLAYER1) {
        return make_pair(1, free);
    }

    if (winner == Winner::PLAYER2) {
        return make_pair(-1, free);
    }

    if (placedPieces == game.p) {
        // TODO
        return make_pair(0, free);
    }

    int columns = game.getColumns();
    if (maximizingPlayer == Winner::PLAYER1){
        // score bestValue = std::numeric_limits<int>::min();
        // Winner bestValue = Winner::PLAYER2;
        score bestValue = -1;
        int bestMove = free;

        for ( column col = 0; col < columns; ++col) {
            if(game.isFree(col)) {
                game.addPiece(col, Winner::PLAYER1);
                pair<score, column> value = minimax(placedPieces + 1, Winner::PLAYER2);
                if (bestValue < value.first){
                    bestValue = value.first;
                    bestMove = col;
                }

                game.removePiece(col);
            }
        }
        return make_pair(bestValue, bestMove);
    }
    else {
        // score bestValue = std::numeric_limits<int>::max();
        // Winner bestValue = Winner::PLAYER1;
        score bestValue = 1;
        int bestMove = free;
        for ( column col = 0; col < columns; ++col) {
            if(game.isFree(col)){
                game.addPiece(col, Winner::PLAYER2);
                pair<score, column> value= minimax(placedPieces, Winner::PLAYER1);
                if (bestValue > value.first){
                    bestValue = value.first;
                    bestMove = col;
                }
                game.removePiece(col);
            }
        }
        return make_pair(bestValue, bestMove);
    }
}

pair<score, column> Player::minimaxABaux(int placedPieces, Winner maximizingPlayer, score alfa, score beta) {
    column free = game.firstFreeColumn();
    Winner winner = game.checkGame();
    if (winner == Winner::PLAYER1) {
        return make_pair(1, free);
    }

    if (winner == Winner::PLAYER2) {
        return make_pair(-1, free);
    }

    if (placedPieces == game.p) {
        // TODO
        return make_pair(0, free);
    }

    int columns = game.getColumns();
    
    if (maximizingPlayer == Winner::PLAYER1) {
        // Busca la movida óptima para el jugador que maximiza, es decir nosotros.
        // score bestValue = std::numeric_limits<int>::min();
        // Winner bestValue = Winner::PLAYER2;
        score bestValue = -1;
        int bestMove = free;

        for (column col = 0; col < columns; ++col) {
            if (game.isFree(col)) {
                game.addPiece(col, Winner::PLAYER1);
                pair<score, column> value = minimaxABaux(placedPieces + 1, Winner::PLAYER2, alfa, beta);
                game.removePiece(col);
                if (bestValue < value.first) {
                    bestValue = value.first;
                    bestMove = col;
                }
                if (alfa < value.first) alfa = value.first;
                if (beta <= alfa) break;
            }
        }
        return make_pair(bestValue, bestMove);
    }
    else {
        // Buscamos la movida óptima para el jugador que minimiza, es decir el oponente. Es importante aclarar que el oponente va a intentar hacer la mejor movida para él; somos nosotros los que queremos que bestValue sea lo menor posible y por eso elegimos nuestra jugada en base a eso.
        // int bestValue = std::numeric_limits<int>::max();
        // score bestValue = std::numeric_limits<int>::max();
        // Winner bestValue = Winner::PLAYER1;
        score bestValue = 1;
        int bestMove = free;
        for ( column col = 0; col < columns; ++col) {
            if(game.isFree(col)){
                game.addPiece(col, Winner::PLAYER2);
                pair<score, column> value= minimaxABaux(placedPieces, Winner::PLAYER1, alfa, beta);
                game.removePiece(col);
                if (bestValue > value.first) {
                    bestValue = value.first;
                    bestMove = col;
                }
                if(beta > value.first) beta = value.first;
                if(beta <= alfa) break;
            }
        }
        return make_pair(bestValue, bestMove);
    }
}

pair<score, column> Player::minimaxAB(int placedPieces){
    return minimaxABaux(placedPieces, Winner::PLAYER1, -1, 1);
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

