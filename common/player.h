#include <random>
#include "judge.h"
#include "genetico/genome.h"

#ifndef __PLAYER__
#define __PLAYER__


class Player {
public:
    virtual int nextMove(Game& game) = 0;
};

class PlayerMinimax_n : public Player {
private:
	int plays;
public:
	PlayerMinimax_n(int n) : plays(n){}
	int nextMove(Game& game) {
    	 // TODO traer p del juego

		auto moves = PossibleMove(game, -1).children(); // -1 ya que no se usa ese valor
        auto max = max_element(moves.begin(), moves.end(),
        	[game](const PossibleMove& m1, const PossibleMove& m2) {
        		m1.in();
        		auto res1 = minimax(m1, plays-1, false);
        		m1.out();
        		m2.in();
        		auto res2 = minimax(m2, plays-1, false);
        		m2.out();
        		return res1 < res2;
        	}
        	);


        assert(max != moves.end()); // encontró alguno

        return max->move();
    }
};

class PlayerGenetic : public Player {
private:
    Genome g;
public:
    PlayerGenetic(Genome g) : g(g) {}
    int nextMove(Game& game) {

        auto moves = game.board().possibleMoves();
        auto bestCol = max_element(moves.begin(), moves.end(),
            [this, game](const int& m1, const int& m2) {
                return g.activate(game.board(), m1) < g.activate(game.board(), m2);
            }
        );
        assert(bestCol != moves.end()); // encontró alguno

        // cerr << "PossibleMoves: " << endl;
        // for (auto i : game.board().possibleMoves())
        // {
        //     cerr << i <<" ";
        // }

        // cerr << endl << "Genetic elige: " << *bestCol << endl;

        return *bestCol;
    }
};

class PlayerRandom : public Player {
private:
    std::random_device rd;
    std::mt19937 generator = std::mt19937(rd());

public:
    int nextMove(Game& game) {
        auto moves = game.board().possibleMoves();
        std::uniform_int_distribution<int> do_move(0, moves.size() - 1);
        int m = do_move(generator);
        return moves.at(m);
    }
};

#endif
