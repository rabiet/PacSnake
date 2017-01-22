#include <stdlib.h>
#include <stdio.h>
#include "Ghosts.h"

void moveGhost(struct Ghost *ghost, struct GameState *game) {
    struct Position up = {ghost->pos.x-1, ghost->pos.y};
    int upFree = !isWall(game->map, up);
    struct Position down = {ghost->pos.x+1, ghost->pos.y};
    int downFree = !isWall(game->map, down);
    struct Position left = {ghost->pos.x, ghost->pos.y-1};
    int leftFree = !isWall(game->map, left);
    struct Position right = {ghost->pos.x, ghost->pos.y+1};
    int rightFree = !isWall(game->map, right);

    enum Direction dir;
    int isFree = 0;

    if(!upFree && !downFree && !leftFree && !rightFree){
        return;
    }

    while(!isFree){
        dir = rand() % 4;

        switch (dir) {
            case UP:
                isFree = upFree && ghost->direction != DOWN;
                break;
            case DOWN:
                isFree = downFree && ghost->direction != UP;
                break;
            case LEFT:
                isFree = leftFree && ghost->direction != RIGHT;
                break;
            case RIGHT:
                isFree = rightFree && ghost->direction != LEFT;
                break;
        }
    }

    ghost->direction = dir;

    switch (dir) {
        case UP:
            ghost->pos.x--;
            break;
        case DOWN:
            ghost->pos.x++;
            break;
        case LEFT:
            ghost->pos.y--;
            break;
        case RIGHT:
            ghost->pos.y++;
            break;
    }

    switch (isOutOfMap(game->map, ghost->pos)) {
        case 1:
            ghost->pos.y = 0;
            break;
        case 2:
            ghost->pos.x = 0;
            break;
        case 3:
            ghost->pos.y = game->map->width - 1;
            break;
        case 4:
            ghost->pos.x = game->map->length - 1;
            break;
    }
}

void moveGhosts(struct Ghost *ghost, struct GameState *game) {
    while(ghost->next){
        moveGhost(ghost, game);
        ghost = ghost->next;
    }
    moveGhost(ghost, game);
}

void moveGhostHome(struct Ghost *ghost, struct GameState *game) {
    ghost->pos = ghost->homePos;
}

void moveGhostsHome(struct Ghost *ghost, struct GameState *game) {
    while(ghost->next){
        moveGhostHome(ghost, game);
        ghost = ghost->next;
    }
    moveGhostHome(ghost, game);
}
