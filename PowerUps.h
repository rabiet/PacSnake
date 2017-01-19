#ifndef _powerups_h
#define _powerups_h

#include "Position.h"

enum PowerUpType {
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
};

void takePowerup(struct PowerUp *powerUp);

#endif
