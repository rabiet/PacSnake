#ifndef _position_h
#define _position_h

struct Position {
    int x;
    int y;
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

int comparePositions(struct Position *pos, struct Position *pos2);
enum Direction reverseDirection(enum Direction dir);

#endif
