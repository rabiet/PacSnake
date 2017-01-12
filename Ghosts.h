#ifndef _ghosts_h
#define _ghosts_h

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

void moveGhost(struct Ghost *ghost);
void moveGhostHome(struct Ghost *ghost);

#endif
