#ifndef STARTUP3_ALGO3_PLAYER_H
#define STARTUP3_ALGO3_PLAYER_H

#include "game.h"

class Game;

class Player {
public:
    virtual int nextMove(Game& game) = 0;
};

#endif
