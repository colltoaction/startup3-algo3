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