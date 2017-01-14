#ifndef _player_h
#define _player_h

#include "Position.h"

struct Player {
  int length;
  struct Position head;
  struct Tail *t;
};

struct Tail {
  struct Position pos;
  struct Tail *t;
};

void movePlayer(struct Player *player);

#endif
