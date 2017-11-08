// 

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
        int bestMove = moves.at(0).move();
        int max = 0;
        for (unsigned int i = 0; i < moves.size();++i) {
            // in y out son trampas para poder agregar y sacar fichas en el tablero
            moves.at(i).in();
            auto v = minimax(moves.at(i), plays - 1, false);
            moves.at(i).out();
            if(bestMove<v){
                bestMove = v;
                max = i;
            }
        }

        return moves.at(max).move();
    }
};