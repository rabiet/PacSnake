#include <stdlib.h>
#include <stdio.h>
#include "PacSnake.h"
#include "Ghosts.h"
#include "PowerUps.h"
#include "Player.h"
#include "Position.h"
#include "Map.h"

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

    struct Tail tail1 = {
        tail: NULL,
        pos: {
            x: 1,
            y: 2
        }
    };

    struct Tail tail2 = {
        tail: &tail1,
        pos: {
            x: 2,
            y: 2
        }
    };

    struct Tail tail3 = {
        tail: NULL,
        pos: {
            x: 3,
            y: 7
        }
    };

    struct Player player = {
        length: 1,
        head: {
            x: 2,
            y: 3
        },
        tail: &tail2
    };

    struct GameState game = {
        map: map,
        ghost1: &g,
        player: &player
    };

    movePlayer(game.player, &game);

    addTail(game.player, &tail3);
    movePlayer(game.player, &game);

    printPlayerPos(game.player);

//    moveGhostHome(game.ghost1, &game);
//    moveGhost(game.ghost1, &game);

    return 0;
}
