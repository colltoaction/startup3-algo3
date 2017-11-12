#ifndef STARTUP3_ALGO3_MINIMAX_H
#define STARTUP3_ALGO3_MINIMAX_H

#include <algorithm>
#include "possible_move.h"
#include "../common/possible_move.h"

int minimax(const PossibleMove &node, int depth, bool maximizingPlayer) {
    if (depth == 0 || node.isTerminal()) {
        return node.heuristic(!maximizingPlayer);
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
    } else {
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

int alphabeta(const PossibleMove &node, int depth, int alfa, int beta, bool maximizingPlayer) {
    if (depth == 0 || node.isTerminal()) {
        return node.heuristic(!maximizingPlayer);
    }

    if (maximizingPlayer) {
        int bestValue = ::std::numeric_limits<int>::min();
        for (PossibleMove child : node.children()) {
            // in y out son trampas para poder agregar y sacar fichas en el tablero
            child.in();
            int v = alphabeta(child, depth - 1, alfa, beta, false);
            child.out();
            bestValue = max(bestValue, v);
            alfa = max(alfa, bestValue);
            if (alfa >= beta) break; //**Beta cut-off**
        }

        return bestValue;
    } else {
        int bestValue = ::std::numeric_limits<int>::max();
        for (PossibleMove child : node.children()) {
            // in y out son trampas para poder agregar y sacar fichas en el tablero
            child.in();
            int v = alphabeta(child, depth - 1, alfa, beta, true);
            child.out();
            bestValue = min(bestValue, v);
            beta = min(beta, bestValue);
            if (alfa >= beta) break; //**Alpha cut-off**
        }

        return bestValue;
    }
}

#endif //STARTUP3_ALGO3_MINIMAX_H
