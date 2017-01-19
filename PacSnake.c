#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "PacSnake.h"
#include "Ghosts.h"
#include "PowerUps.h"
#include "Player.h"
#include "Position.h"
#include "Map.h"
#include <SDL2/SDL.h>


int main(int argc, char **argv) {
    srand(time(NULL));

    struct Ghost g = {
        pos: {
            x: 9,
            y: 9
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

    struct Tail tail3 = {
        tail: NULL,
        pos: {
            x: 3,
            y: 1
        }
    };

    struct Tail tail2 = {
        tail: &tail3,
        pos: {
            x: 2,
            y: 1
        }
    };

    struct Tail tail1 = {
        tail: &tail2,
        pos: {
            x: 1,
            y: 1
        }
    };

    struct Player player = {
        length: 4,
        head: {
            x: 1,
            y: 2
        },
        tail: &tail1,
        direction: RIGHT
    };

    struct GameState game = {
        map: map,
        ghost1: &g,
        player: &player
    };

    struct PowerUp *powerUp = spawnPowerup(FASTER, 20, &game);
    printf("%d, %d\n", powerUp->pos.x, powerUp->pos.y);

    const int width = 20 * map->width;
    const int height = 20 * map->length;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("PacSnake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    int i = 0;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT)
                running = false;
            switch (event.key.keysym.sym)
            {
                case (SDLK_w): turnPlayer(&player, UP); break;
                case (SDLK_s): turnPlayer(&player, DOWN); break;
                case (SDLK_a): turnPlayer(&player, LEFT); break;
                case (SDLK_d): turnPlayer(&player, RIGHT); break;
                case (SDLK_ESCAPE): running = false;
            }
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

        if (i == 15)
        {
            i = 0;
            movePlayer(game.player, &game);
        }

        SDL_SetRenderDrawColor(renderer, 255, 110, 110, 255);
        SDL_Rect snek = { game.player->head.y * 20, game.player->head.x * 20, 20, 20};
        SDL_RenderFillRect(renderer, &snek);

        struct Tail *tail = game.player->tail;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        while(tail)
        {
            SDL_Rect schwanz = { tail->pos.y * 20, tail->pos.x*20, 20, 20};
            SDL_RenderFillRect(renderer, &schwanz);
            tail = tail->tail;
        }

        SDL_RenderPresent(renderer);
        i++;
    }

    movePlayer(game.player, &game);

    //addTail(game.player, &tail3);
    movePlayer(game.player, &game);

    printPlayerPos(game.player);

//    moveGhostHome(game.ghost1, &game);
//    moveGhost(game.ghost1, &game);

    SDL_DestroyWindow(window);
    SDL_Quit;

    return 0;
}
