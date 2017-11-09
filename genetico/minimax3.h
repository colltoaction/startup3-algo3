#ifdef PLAYINGAGAINSTMINIMAXN

#include "../common/judge.h"

#endif
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

        if(bestResult == 0){

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