#include <stdlib.h>
#include <stdio.h>
#include "Ghosts.h"
#include "PowerUps.h"
#include "Player.h"
#include "Position.h"
#include "MapLoader.h"

int gameTick(){

}

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

    struct Map *map = loadMap();
    if(map == NULL){
        return -1;
    }
    printMapToConsole(map);

    moveGhost(&g);
    moveGhostHome(&g);
    moveGhost(&g);

    return 0;
}
