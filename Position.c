#include "Position.h"

int comparePositions(struct Position *pos, struct Position *pos2){
    return pos->x == pos2->x && pos->y == pos2->y;
}
