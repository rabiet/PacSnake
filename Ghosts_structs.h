#ifndef _ghosts_structs_h
#define _ghosts_structs_h

#include "Position.h"

// http://mentalfloss.com/uk/games/31287/the-different-strategies-of-each-of-pac-mans-ghosts
enum GhostType {
    BLINKY,
    PINKY,
    CLYDE,
    INKY
};

struct Ghost {
    struct Position pos;
    enum Direction direction;
    enum GhostType type;
    struct Position homePos;
};

#endif
