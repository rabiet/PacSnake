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

int isTailPos(struct Tail *tail, struct Position pos){
    while(tail && !comparePositions(&tail->pos, &pos)) {
        tail = tail->tail;
    }

    return tail ? comparePositions(&tail->pos, &pos) : 0;
}

int isPlayerPos(struct Player *player, struct Position pos){
    if(comparePositions(&player->head, &pos)){
        return 1;
    }

    return isTailPos(player->tail, pos);
}

void movePlayer(struct Player *player, struct GameState *game){
    struct Position oldPos = player->head;
    printPlayerPos(player);

    switch (player->direction) {
        case UP:
            player->head.x--;
            break;
        case DOWN:
            player->head.x++;
            break;
        case LEFT:
            player->head.y--;
            break;
        case RIGHT:
            player->head.y++;
            break;
    }

    switch (isOutOfMap(game->map, player->head)) {
        case 1:
            player->head.y = 0;
            break;
        case 2:
            player->head.x = 0;
            break;
        case 3:
            player->head.y = game->map->width;
            break;
        case 4:
            player->head.x = game->map->length;
            break;
    }

    if(isWall(game->map, player->head) || isTailPos(game->player->tail, player->head)){
        //TODO: indicate the end of the game
        printf("%s\n", "This would kill the player!");
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
