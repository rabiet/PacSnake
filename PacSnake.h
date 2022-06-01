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
    int difficulty;
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

extern int height;
extern int width;
extern int fieldHeight;
extern int fieldWidth;
extern char name[64];

extern struct highscore *hs;

extern struct SDL_Renderer *renderer;

#endif
