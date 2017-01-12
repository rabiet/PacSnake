#include <stdlib.h>
#include <stdio.h>
#include "Ghosts.h"
#include "Position.h"

int main(int argc, char const *argv[]) {
    struct Ghost g = {
        pos: {
            x: 1,
            y: 3
        },
        direction: UP,
        type: CLYDE,
        homePos: {
            x: 5,
            y: 5
        }
    };

    moveGhost(&g);
    moveGhostHome(&g);
    moveGhost(&g);

    return 0;
}
