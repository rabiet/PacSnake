#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "PowerUps.h"
#include "Map.h"
#include "Player.h"

void printPowerUps(struct PowerUp *powerUp1){
    struct PowerUp *powerUp = powerUp1;

    while(powerUp){
        printf("PowerUp: %d/%d\n", powerUp->pos.x, powerUp->pos.y);
        powerUp = powerUp->next;
    }
}

int isPowerupPos(struct PowerUp *powerUp, struct Position pos){
    while(powerUp && !comparePositions(&powerUp->pos, &pos)) {
        powerUp = powerUp->next;
    }

    return powerUp ? comparePositions(&powerUp->pos, &pos) : 0;
}

void removePowerup(struct PowerUp *powerUp, struct GameState *state){
    struct PowerUp *last = state->powerUp;

    if(last == powerUp){
        state->powerUp = powerUp->next;
        free(powerUp);
        return;
    }
    while(last->next != powerUp && last->next){
        last = last->next;
    }

    last->next = powerUp->next;

    free(powerUp);
}

void spawnPowerup(enum PowerUpType type, int t, struct GameState *state){
    struct PowerUp *powerUp = malloc(sizeof(struct PowerUp));

    powerUp->type = type;
    powerUp->time = t;
    powerUp->next = NULL;

    struct Position pos = {
        x: rand() % state->map->length,
        y: rand() % state->map->width
    };

    while(isWall(state->map, pos) || isPlayerPos(state->player, pos) || isPowerupPos(state->powerUp, pos)){
        pos.x = rand() % state->map->length;
        pos.y = rand() % state->map->width;
    }

    powerUp->pos = pos;

    struct PowerUp *lastPowerUp = state->powerUp;

    if(lastPowerUp) {
        int o = 0;

        while(lastPowerUp->next){
            lastPowerUp = lastPowerUp->next;
            o++;
        }

        lastPowerUp->next = powerUp;
    } else {
        state->powerUp = powerUp;
    }
}

void takePowerup(struct PowerUp *powerUp, struct GameState *state){
    switch (powerUp->type) {
        case EAT_GHOSTS: break;
        case SLOWER: break;
        case FASTER: break;
        case GHOSTS_TO_CENTER:
            moveGhostsHome(state->ghost, state);
            break;
        case TURNAROUND: break;
        case EAT_WALL: break;
        case GROW:
            growTail(state->player);
            break;
    }

    removePowerup(powerUp, state);
    spawnPowerup(GROW, 20, state);
}

void takePowerupPos(struct Position pos, struct GameState *state){
    struct PowerUp *powerUp = state->powerUp;

    while(powerUp) {
        if(comparePositions(&powerUp->pos, &pos)){
            takePowerup(powerUp, state);
        }
        powerUp = powerUp->next;
    }
}
