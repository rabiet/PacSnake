#include <stdlib.h>
#include <stdio.h>
#include "Player.h"
#include "Ghosts.h"
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

    while(tail){
        tempPos = tail->pos;
        tail->pos = oldPos;
        oldPos = tempPos;
        tail = tail->tail;
    }

    if(game->powerUpEatWallTime > 0){
        removeWall(game->map, player->head);
    }

    if(game->powerUpEatGhostsTime > 0){
        struct Ghost *ghost = game->ghost;
        while(ghost->next){
            if(isPlayerPos(game->player, ghost->pos)){
                moveGhostHome(ghost);
            }
            ghost = ghost->next;
        }
    }


    if(isWall(game->map, player->head) || isTailPos(game->player->tail, player->head)){
        game->alive = false;
    }

    takePowerupPos(player->head, game);

    struct Ghost *ghost = game->ghost;
        while(ghost)
        {
            if(comparePositions(&game->player->head, &ghost->pos))
            {
                game->alive = false;
                return;
            }else if(game->player->tail && comparePositions(&game->player->tail->pos, &ghost->pos)){
                removeTails(game->player->tail);
            }else{
                struct Tail *tail = game->player->tail;
                struct Tail *oldtail = game->player->tail;

                while(tail && !comparePositions(&tail->pos, &ghost->pos)) 
                {
                    oldtail = tail;
                    tail = tail->tail;
                }

                if(tail)
                {
                    removeTails(tail);
                    if(oldtail) oldtail->tail = NULL;
                }
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
        player->tail = tail;
    } else {
        struct Tail *lastTail = player->tail;

        while(lastTail->tail) {
            lastTail = lastTail->tail;
        }

        tail->pos = lastTail->pos;
        lastTail->tail = tail;
    }
}

void turnPlayer(struct Player *player, enum Direction direction){
    player->direction = direction;
}

void removeTails(struct Tail *tail){
    while(tail){
        struct Tail *t = tail;
        tail = t->tail;
        free(t);
    }
}
