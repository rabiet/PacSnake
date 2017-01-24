#ifndef _player_h
#define _player_h

#include "Player_structs.h"
#include "Position.h"
#include "PacSnake.h"
#include "PowerUps.h"

int isTailPos(struct Tail *tail, struct Position pos);
int isPlayerPos(struct Player *player, struct Position pos);

void movePlayer(struct Player *player, struct GameState *game);

void turnPlayer(struct Player *player, enum Direction direction);

void addTail(struct Player *player, struct Tail *newTail);
void growTail(struct Player *player);
void removeTails(struct Tail *player);

void printPlayerPos(struct Player *player);
#endif
