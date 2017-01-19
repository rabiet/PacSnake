#ifndef _map_h
#define _map_h
#include "Position.h"

enum MapFieldTypes {
    WALL,
    FREE,
    GHOST,
    PLAYER
};

struct Map {
    int width;
    int length;
    enum MapFieldTypes *fields;
};

struct Map *loadMap();

void printMapToConsole(struct Map *map);

void unloadMap(struct Map *map);

int isWall(struct Map *map, struct Position pos);

int isOutOfMap(struct Map *map, struct Position pos);

#endif
