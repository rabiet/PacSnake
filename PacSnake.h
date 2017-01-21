#ifndef _pacsnake_h
#define _pacsnake_h

#include "Position.h"
#include "Map.h"
#include "Ghosts_structs.h"
#include "Player_structs.h"

struct GameState {
    struct Map *map;
    struct Ghost *ghost1;
    struct Player *player;
    struct PowerUp *powerUp;
};

#endif
