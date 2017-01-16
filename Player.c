#include <stdlib.h>
#include <stdio.h>
#include "Player.h"

void printPlayerPos(struct Player *player){
    printf("Head: %d/%d\n", player->head.x, player->head.y);

    struct Tail *tail = player->tail;

    while(tail){
        printf("Tail: %d/%d\n", tail->pos.x, tail->pos.y);
        tail = tail->tail;
    }
}

void movePlayer(struct Player *player, struct GameState *game){
    //TODO test if it kills the player
    struct Position oldPos = player->head;
    printPlayerPos(player);

    switch (player->direction) {
        case UP:
            player->head.x++;
            break;
        case DOWN:
            player->head.x--;
            break;
        case LEFT:
            player->head.y++;
            break;
        case RIGTH:
            player->head.y--;
            break;
    }

    struct Tail *tail = player->tail;
    struct Position tempPos;

    while(tail){
        tempPos = tail->pos;
        tail->pos = oldPos;
        oldPos = tempPos;
        tail = tail->tail;
    }
}

void addTail(struct Player *player, struct Tail *newTail){
    struct Tail *prevTail = player->tail;
    struct Tail *tail = player->tail;

    while(tail) {
        prevTail = tail;
        tail = tail->tail;
    }

    prevTail->tail = newTail;
}

void turnPlayer(struct Player *player, enum Direction direction){
    player->direction = direction;
}
