#include <chrono>
#include <algorithm>
#include "../common/possible_move.h"
#include "../common/player_random.h"
#include "../common/judge_proxy.h"
#include "../common/minimax.h"

using namespace std::chrono;

class PlayerAlfaBeta : public Player {
public:
    int nextMove(Game &game) {
        high_resolution_clock::time_point beginMove = high_resolution_clock::now();

        auto moves = PossibleMove(game, -1).children(); // -1 ya que no se usa ese valor

        auto max = max_element(moves.begin(), moves.end(),
                               [game](const PossibleMove &m1, const PossibleMove &m2) {
                                   m1.in();
                                   auto res1 = alphabeta(m1, game.remainingPieces(), -1, 1, false);
                                   m1.out();
                                   m2.in();
                                   auto res2 = alphabeta(m2, game.remainingPieces(), -1, 1, false);
                                   m2.out();
                                   return res1 < res2;
                               }
        );

        high_resolution_clock::time_point endMove = high_resolution_clock::now();

        duration<double> timeFindingMove = duration_cast<duration<double> >(endMove - beginMove);

        assert(max != moves.end()); // encontró alguno

        std::cerr << game.board().possibleMoves().size() << "; " << timeFindingMove.count() << "; "
                  << game.board().columns() << "; " << game.board().rows() << "; " << game.cToWin() << endl;

        return max->move();
    }
};

int mainExpeAB() {
    std::cerr << "Columnas libres; Tiempo de ejecucion; Columnas; Filas; C" << endl;
    for (int rows = 2; rows <= 5; ++rows) {
        for (int cols = 2; cols <= 5; ++cols) {
            for (int c = 2; c <= 5; ++c) {
                for (int i = 0; i < 10; ++i) {
                    Game game(rows, cols, c, (rows * cols) / 2);
                    PlayerRandom playerR;
                    PlayerAlfaBeta playerAB;
                    game.playMatch(playerAB, playerR);
                }
            }
        }
    }
    return 0;
}

int main() {
//    mainExpeAB();
    JudgeProxy judge;
    while (judge.keepPlaying()) {
        PlayerAlfaBeta player;
        judge.play(player);
    }
}
