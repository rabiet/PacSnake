#ifndef _ghosts_h
#define _ghosts_h

#include "Ghosts_structs.h"
#include "PacSnake.h"

void moveGhosts(struct Ghost *ghost, struct GameState *game);
void moveGhost(struct Ghost *ghost, struct GameState *game);
void moveGhostsHome(struct Ghost *ghost);
void moveGhostsHomePos(struct Ghost *ghost, struct Position pos);
void moveGhostHome(struct Ghost *ghost);

#endif
