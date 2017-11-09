#include "minimax3.h"


int main(int argc, char** argv) {
    JudgeProxy judge;
    while (judge.keepPlaying()) {
        PlayerMinimax_n player(2);
        judge.play(player);
    }
}
	