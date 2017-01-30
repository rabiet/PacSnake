#include <stdlib.h>
#include <stdio.h>
#include "Player.h"
#include "Ghosts.h"
#include "Position.h"
#include "Map.h"

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
    enum Direction oldDir = player->direction;

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
            player->head.y = game->map->width - 1;
            break;
        case 4:
            player->head.x = game->map->length - 1;
            break;
    }

    struct Tail *tail = player->tail;
    struct Position tempPos;
    enum Direction tempDir;

    while(tail){
        tempPos = tail->pos;
        tempDir = tail->direction;
        tail->pos = oldPos;
        tail->direction = oldDir;
        oldPos = tempPos;
        oldDir = tempDir;
        tail = tail->tail;
    }

    if(game->powerUpEatWallTime > 0){
        removeWall(game->map, player->head);
    }

    if(isWall(game->map, player->head) || isTailPos(game->player->tail, player->head)){
        game->alive = 2;
    }

    takePowerupPos(player->head, game);

    struct Ghost *ghost = game->ghost;
    while(ghost){
        if(comparePositions(&game->player->head, &ghost->pos)) {
            game->alive = 3;
        }
        ghost = ghost->next;
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

void growTail(struct Player *player){
    struct Tail *tail = malloc(sizeof(struct Tail));
    tail->tail = NULL;

    if(!player->tail) {
        tail->pos = player->head;
        tail->direction = player->direction;
        player->tail = tail;
    } else {
        struct Tail *lastTail = player->tail;

        while(lastTail->tail) {
            lastTail = lastTail->tail;
        }

        tail->pos = lastTail->pos;
        tail->direction = lastTail->direction;
        lastTail->tail = tail;
    }
}

int couldPlayerMove(struct Player *player, enum Direction direction)
{
    if (player->direction == UP && direction == DOWN) return false;
    if (player->direction == DOWN && direction == UP) return false;
    if (player->direction == RIGHT && direction == LEFT) return false;
    if (player->direction == LEFT && direction == RIGHT) return false;
    return true;
}

void turnPlayer(struct Player *player, enum Direction direction){
    if (couldPlayerMove(player, direction)) { player->direction = direction; }
}

void removeTails(struct Tail *tail){
    while(tail){
        struct Tail *t = tail;
        tail = t->tail;
        free(t);
    }
}

void turnPlayerAround(struct Player *player){
    if(!player->tail){
        player->direction = reverseDirection(player->direction);
        return;
    }

    struct Tail *first = player->tail;
    struct Tail *tail = player->tail;
    struct Tail *last = NULL;
    struct Tail *temp = NULL;

    while (tail) {
        temp = tail;
        tail = tail->tail;
        temp->tail = last;
        last = temp;
        last->direction = reverseDirection(last->direction);
    }

    struct Position pPos = player->head;
    enum Direction pDir = reverseDirection(player->direction);
    struct Position lPos = last->pos;
    enum Direction lDir = last->direction;

    player->head = lPos;
    player->direction = lDir;
    player->tail = last->tail;
    last->pos = pPos;
    last->direction = pDir;
    last->tail = NULL;
    first->tail = last;
}
