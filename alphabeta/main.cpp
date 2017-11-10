#include <algorithm>
#include "../common/possible_move.h"
#include "../common/player_random.h"
#include "../common/player_alfabeta.h"
#include "../common/judge_proxy.h"
#include "../common/minimax.h"




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
