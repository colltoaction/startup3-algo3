#include <iostream>
#include "../common/game.h"
#include "../common/player_minimax.h"
#include "../common/player_random.h"
#include "../common/judge_proxy.h"

int mainExpeMM() {
    std::cerr << "Columnas libres; Tiempo de ejecucion; Columnas; Filas; C " << endl;
    for (int rows = 2; rows <= 5; ++rows) {
        for (int cols = 2; cols <= 5; ++cols) {
            for (int c = 2; c <= 5; ++c) {
                for (int i = 0; i < 10; ++i) {
                    Game game(rows, cols, c, (rows * cols) / 2);
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
//    mainExpeMM();
    JudgeProxy judge;
    while (judge.keepPlaying()) {
        PlayerMinimax player;

        judge.play(player);
    }
}
