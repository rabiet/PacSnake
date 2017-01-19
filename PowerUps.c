#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "PowerUps.h"
#include "Map.h"

void takePowerup(struct PowerUp *powerUp){
    switch (powerUp->type) {
        case EAT_GHOSTS: break;
        case SLOWER: break;
        case FASTER: break;
        case GHOSTS_TO_CENTER: break;
        case TURNAROUND: break;
        case EAT_WALL: break;
    }
}

struct PowerUp *spawnPowerup(enum PowerUpType type, int t, struct GameState *state){
    struct PowerUp *powerUp = malloc(sizeof(struct PowerUp));

    powerUp->type = type;
    powerUp->time = t;

    struct Position pos = {
        x: rand() % state->map->width,
        y: rand() % state->map->length
    };

    while(isWall(state->map, pos)){
        pos.x = rand() % state->map->length;
        pos.y = rand() % state->map->length;
    }

    powerUp->pos = pos;

    return powerUp;
}
