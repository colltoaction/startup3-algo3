// #include "../common/judge.h"
#include "../common/player_random.h"
#include <chrono>
#include <iostream>
#include <fstream>
using namespace std::chrono;

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

class PlayerMinimax : public Player {
public:
	int nextMove(Game& game) {
    	 // TODO traer p del juego

		high_resolution_clock::time_point beginMove = high_resolution_clock::now();



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

        high_resolution_clock::time_point endMove = high_resolution_clock::now();
        
        duration<double> timeFindingMove = duration_cast< duration<double> >(endMove - beginMove);

        assert(max != moves.end()); // encontró alguno

        std::cerr << game.board().possibleMoves().size() << "; "<< timeFindingMove.count() << "; "<< game.board().columns() <<"; "<< game.board().rows() <<"; " << game.cToWin()<<endl;
        
        return max->move();
    }
};

int mainExpeMM(){
	std::cerr << "Columnas libres; Tiempo de ejecucion; Columnas; Filas; C "<< endl;
	for (int rows = 2; rows <= 5; ++rows){
		for (int cols = 2; cols <= 5; ++cols){
			for (int c = 2; c <= 5; ++c){
				for (int i = 0; i < 10; ++i){
					Game game(rows, cols, c, (rows*cols)/2);
					PlayerRandom playerR;
					PlayerMinimax playerM;
					game.playMatch(playerM, playerR);
				}
			}
		}	
	}
	return 0;
}

int main() {
	mainExpeMM();
	// JudgeProxy judge;
	/*while (judge.keepPlaying()) {
		PlayerMinimax player;

		judge.play(player);
	}*/
}
