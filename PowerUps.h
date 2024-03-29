#ifndef _powerups_h
#define _powerups_h

#include "Position.h"
#include "Player.h"
#include "Ghosts.h"
#include "PacSnake.h"
#include <SDL2/SDL.h>

enum PowerUpType {
    GROW,
    EAT_GHOSTS,
    SLOWER,
    FASTER,
    GHOSTS_TO_CENTER,
    TURNAROUND,
    EAT_WALL
};

struct PowerUp {
  int time;
  struct Position pos;
  enum PowerUpType type;
  struct PowerUp *next;
};

int isPowerupPos(struct PowerUp *powerUp, struct Position pos);
void takePowerupPos(struct Position pos, struct GameState *state);
void takePowerup(struct PowerUp *powerUp, struct GameState *state);
void spawnPowerup(enum PowerUpType type, int time, struct GameState *state);
void updateTimedPowerUps(struct GameState *state);
void setPowerUpSDLRenderColor(struct SDL_Renderer *renderer, struct PowerUp *powerUp);
void spawnRandomPowerup(struct GameState *state);
int getGameSpeed(struct GameState *state);

#endif
