#ifndef _map_h
#define _map_h

#include "Map_structs.h"
#include "Position.h"
#include "PacSnake.h"

enum MapFieldTypes {
    WALL,
    FREE
};


int loadMap(struct GameState *state);

void printMapToConsole(struct Map *map);

void unloadMap(struct Map *map);

int isWall(struct Map *map, struct Position pos);

int isOutOfMap(struct Map *map, struct Position pos);

void removeWall(struct Map *map, struct Position pos);

#endif
