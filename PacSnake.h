#ifndef _pacsnake_h
#define _pacsnake_h

#include "Position.h"
#include "Map_structs.h"
#include "Ghosts_structs.h"
#include "Player_structs.h"
#include <stdbool.h>

struct GameState {
    struct Map *map;
    struct Ghost *ghost;
    struct Player *player;
    struct PowerUp *powerUp;
    int speed;
    int powerUpSlowerTime;
    int powerUpFasterTime;
    int powerUpEatGhostsTime;
    int powerUpEatWallTime;
    bool alive;
    bool running;
    bool pause;
    int pauseTimeout;
};

#endif
