#ifndef STARTUP3_ALGO3_PLAYER_MINIMAX_N_H
#define STARTUP3_ALGO3_PLAYER_MINIMAX_N_H

#include "game.h"
#include "minimax.h"
#include "possible_move.h"

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


        if(bestResult == 0) {
            bool loser = false;
            do{
                // binomial_distribution<int> randomMove(moves.size()/2, 0.5);
                // max = randomMove.operator();
                max = rand() % moves.size();
                for (int i : losers) {
                    if(i == max){
                        loser = true;
                        break;
                    }
                    if(i > max){
                        loser= false;
                        break;
                    }
                    int end = losers.size()-1;
                    if(i == end and losers.at(i)< max){
                        loser= false;
                        break;
                    }
                }
            } while (loser);
        }
        return moves.at(max).move();
    }
};

#endif //STARTUP3_ALGO3_PLAYER_MINIMAX_N_H
