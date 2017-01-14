#ifndef _maploader_h
#define _maploader_h

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

#endif
