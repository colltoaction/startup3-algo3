#include "../common/judge.h"


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

int alphabeta(const PossibleMove& node, int depth, int alfa,  int beta, bool maximizingPlayer) {
    if (depth == 0 || node.isTerminal()) {
        return node.heuristic();
    }

    if (maximizingPlayer) {
        int bestValue = std::numeric_limits<int>::min();
        for (PossibleMove child : node.children()) {
            // in y out son trampas para poder agregar y sacar fichas en el tablero
            child.in();
            int v = alphabeta(child, depth - 1, alfa, beta, false);
            child.out();
            bestValue = std::max(bestValue, v);
            alfa = std::max(alfa,bestValue);
            if(alfa >= beta) break; //**Beta cut-off**
        }

        return bestValue;
    }
    else {
        int bestValue = std::numeric_limits<int>::max();
        for (PossibleMove child : node.children()) {
            // in y out son trampas para poder agregar y sacar fichas en el tablero
            child.in();
            int v = alphabeta(child, depth - 1, alfa, beta, true);
            child.out();
            bestValue = std::min(bestValue, v);
            beta = std::min(beta, bestValue);
            if(alfa >= beta) break; //**Alpha cut-off**
        }

        return bestValue;
    }
}

class PlayerMinimax : public Player {
public:
    int nextMove(Game& game) {
        // TODO traer p del juego
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
        assert(max != moves.end()); // encontró alguno
        return max->move();
    }
};

class PlayerAlfaBeta : public Player {
public:
    int nextMove(Game& game) {
        // TODO traer p del juego
        auto moves = PossibleMove(game, -1).children(); // -1 ya que no se usa ese valor
        
        auto max = max_element(moves.begin(), moves.end(),
            [game](const PossibleMove& m1, const PossibleMove& m2) {
                m1.in();
                auto res1 = alphabeta(m1, game.remainingPieces(), -1, 1, false);
                m1.out();
                m2.in();
                auto res2 = alphabeta(m2, game.remainingPieces(), -1, 1, false);
                m2.out();
                return res1 < res2;
            }
        );
        assert(max != moves.end()); // encontró alguno
        return max->move();
    }
};

int main() {
    JudgeProxy judge;
    while (judge.keepPlaying()) {
        PlayerMinimax player;
        judge.play(player);
    }
}