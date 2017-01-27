#include "Position.h"

int comparePositions(struct Position *pos, struct Position *pos2){
    return pos->x == pos2->x && pos->y == pos2->y;
}

enum Direction reverseDirection(enum Direction dir){
    switch (dir) {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case LEFT:
            return RIGHT;
        case RIGHT:
            return LEFT;
    }
}
