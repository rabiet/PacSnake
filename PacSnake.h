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
    int alive;
    bool running;
    bool pause;
    int pauseTimeout;
    int score;
    int maxScore;
};

struct GameState *resetGame(struct GameState *state);

int height;
int width;
int fieldHeight;
int fieldWidth;
char name[64];

struct highscore *hs;

struct SDL_Renderer *renderer;

#endif
