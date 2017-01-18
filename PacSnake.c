#include <stdlib.h>
#include <stdio.h>
#include "PacSnake.h"
#include "Ghosts.h"
#include "PowerUps.h"
#include "Player.h"
#include "Position.h"
#include "Map.h"
#include <stdbool.h>

#include <SDL2/SDL.h>

int gameTick(){

}

void printMapToSDL(struct Map *map) {
    
}

int main(int argc, char **argv) {
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
        tail: &tail2,
        direction: RIGHT
    };

    struct GameState game = {
        map: map,
        ghost1: &g,
        player: &player
    };

    const int width = 20 * map->width;
    const int height = 20 * map->length;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("PacSnake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT)
                running = false;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < map->length; i++) 
        {
            for (int j = 0; j < map->width; j++) 
            {
                if (map->fields[(i * map->width) + j] == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_Rect wall = {20 * j, 20 * i, 20, 20};
                    SDL_RenderFillRect(renderer, &wall);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    movePlayer(game.player, &game);

    addTail(game.player, &tail3);
    movePlayer(game.player, &game);

    printPlayerPos(game.player);

//    moveGhostHome(game.ghost1, &game);
//    moveGhost(game.ghost1, &game);

    SDL_DestroyWindow(window);
    SDL_Quit;

    return 0;
}
