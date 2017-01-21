#ifndef _powerups_h
#define _powerups_h

#include "Position.h"
#include "Player.h"
#include "PacSnake.h"

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
  int id;
  int time;
  struct Position pos;
  enum PowerUpType type;
  struct PowerUp *next;
};

int isPowerupPos(struct PowerUp *powerUp, struct Position pos);
void takePowerupPos(struct Position pos, struct GameState *state);
void takePowerup(struct PowerUp *powerUp, struct GameState *state);
void spawnPowerup(enum PowerUpType type, int time, struct GameState *state);

#endif
