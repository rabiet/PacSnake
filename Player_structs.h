#ifndef _player_structs_h
#define _player_structs_h

#include "Position.h"
#include "PacSnake.h"

struct Player {
  struct Position head;
  enum Direction direction;
  struct Tail *tail;
};

struct Tail {
  struct Position pos;
  struct Tail *tail;
  enum Direction direction;
};

#endif
