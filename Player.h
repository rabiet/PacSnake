#ifndef _player_h
#define _player_h

#include "Player_structs.h"
#include "Position.h"
#include "PacSnake.h"

int isPlayerPos(struct Player *player, struct Position pos);

void movePlayer(struct Player *player, struct GameState *game);

void turnPlayer(struct Player *player, enum Direction direction);

void addTail(struct Player *player, struct Tail *newTail);

void printPlayerPos(struct Player *player);

#endif
