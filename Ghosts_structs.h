#ifndef _ghosts_structs_h
#define _ghosts_structs_h

#include "Position.h"

struct Ghost {
    struct Position pos;
    enum Direction direction;
    struct Position homePos;
    struct Ghost *next;
};

#endif
