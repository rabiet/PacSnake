#ifndef _ghosts_h
#define _ghosts_h

#include "Ghosts_structs.h"
#include "PacSnake.h"

void moveGhost(struct Ghost *ghost, struct GameState *game);
void moveGhostHome(struct Ghost *ghost, struct GameState *game);

#endif
