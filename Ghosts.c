#include <stdlib.h>
#include <stdio.h>
#include "Ghosts.h"

void moveGhost(struct Ghost *ghost){
    switch (ghost->type) {
        case BLINKY:
            printf("%s\n", "Blinky");
            break;
        case PINKY:
            printf("%s\n", "Pinky");
            break;
        case CLYDE:
            printf("%s : %d / %d : %d\n", "Clyde", ghost->pos.x, ghost->pos.y, ghost->direction);
            break;
        case INKY:
            printf("%s\n", "Inky");
            break;
    }
}

void moveGhostHome(struct Ghost *ghost){
    ghost->pos = ghost->homePos;
}
