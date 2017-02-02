#include <stdlib.h>
#include <stdio.h>
#include "Map.h"
#include "PacSnake.h"

/*
 * prints the map to the console (it does not look nice ;) )
 */
void printMapToConsole(struct Map *map) {
    for (int i = 0; i < map->length; i++) {
        for (int j = 0; j < map->width; j++) {
            printf("%d", map->fields[(i * map->width) + j]);
        }
        putchar('\n');
    }
}

/*
 * loads the map from the file and returns it as a pointer to a struct Map
 */
int loadMap(struct GameState *state) {
    FILE *mapFile;

    mapFile = fopen("map.txt", "r");

    if (!mapFile) {
        printf("Error while reading file map.txt\n");
        return 0;
    }


    // get the size of map
    char c;
    char lengths[4];
    char widths[4];
    int  i      = 0;
    int  j      = 0;
    int  length = 0;
    int  width  = 0;

    while ((c = fgetc(mapFile)), c != '\n' && c != EOF) {
        if (j >= 4) {
            printf("Too large Mapsize\n");
            return -1;
        } else if (c == ',') {
            lengths[j] = '\0';
            sscanf(lengths, "%d", &length);

            i++;
            j = 0;
        } else if (i == 0) {
            lengths[j] = c;
            j++;
        } else if (i == 1) {
            widths[j] = c;
            j++;
        }
    }

    widths[j] = '\0';
    sscanf(widths, "%d", &width);

    // create a pointer for the values of the mapfields
    enum MapFieldTypes *map = malloc(sizeof(enum MapFieldTypes) * width * length);

    int row = 0;
    int col = 0;

    // save the mapfields to the pointer
    while ((c = fgetc(mapFile)), c != EOF) {
        if (c == '\n') {
            row++;
            col = 0;
            if(row >= length){
                break;
            }
        } else {
            if(col >= width){
                break;
            }
            switch (c) {
            case '#':
                map[row * width + col] = WALL;
                break;
            case 'G':
                if(true){
                    struct Ghost *ghost = malloc(sizeof(struct Ghost));
                    ghost->pos.x = row;
                    ghost->pos.y = col;
                    ghost->direction = UP;
                    ghost->homePos.x = row;
                    ghost->homePos.y = col;
                    ghost->next = NULL;

                    if(state->ghost){
                        struct Ghost *tempGhost = state->ghost;

                        while(tempGhost->next){
                            tempGhost = tempGhost->next;
                        }

                        tempGhost->next = ghost;
                    }else{
                        state->ghost = ghost;
                    }
                }
            case 'P':
                if(!state->player){
                    struct Player *player = malloc(sizeof(struct Player));
                    struct Tail *tail1 = malloc(sizeof(struct Tail));
                    struct Tail *tail2 = malloc(sizeof(struct Tail));
                    struct Tail *tail3 = malloc(sizeof(struct Tail));

                    player->head.x = row;
                    player->head.y = col;
                    player->direction = RIGHT;
                    player->tail = tail1;

                    tail1->tail = tail2;
                    tail1->pos.x = row;
                    tail1->pos.y = col;

                    tail2->tail = tail3;
                    tail2->pos.x = row;
                    tail2->pos.y = col;

                    tail3->tail = NULL;
                    tail3->pos.x = row;
                    tail3->pos.y = col;

                    state->player = player;
                }
            default:
                map[row * width + col] = FREE;
                break;
            }
            col++;
        }
    }

    // create the struct for the map
    struct Map *mapstruc = malloc(sizeof(struct Map));
    mapstruc->width  = width;
    mapstruc->length = length;
    mapstruc->fields = map;

    state->map = mapstruc;

    return 1;
}

/*
 * frees the pointer of the map
 */
void unloadMap(struct Map *map) {
    free(map->fields);
    free(map);
}

int isWall(struct Map *map, struct Position pos){
    if(isOutOfMap(map, pos)){
        return 0;
    }

    return map->fields[map->width * pos.x + pos.y] == WALL;
};

int isOutOfMap(struct Map *map, struct Position pos){
    if(map->width <= pos.y){
        return 1;
    }else if(map->length <= pos.x){
        return 2;
    }else if(pos.y < 0){
        return 3;
    }else if(pos.x < 0){
        return 4;
    }

    return 0;
}

void removeWall(struct Map *map, struct Position pos){
    map->fields[map->width*pos.x + pos.y] = FREE;
}
