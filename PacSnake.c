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
#include <SDL2/SDL_ttf.h>


bool alive = false;
bool running = true;
bool pause = false;

int selected = 0;
int timeout = 0;

SDL_Renderer *renderer;

int width;
int height;

SDL_Color white = {255, 255, 255, 255};
SDL_Color red   = {255, 0, 0, 255};

TTF_Font *font;



void renderTexture(SDL_Texture *texture, int x, int y)
{
    SDL_Rect helperRect;
    helperRect.x = x;
    helperRect.y = y;
    helperRect.h = 20;
    helperRect.w = 20;
    SDL_RenderCopy(renderer, texture, NULL, &helperRect);
}

void handleReturn()
{
    if (!alive) 
    { 
        if (selected == 0) 
        { 
            alive = true;
            timeout = 60;
        } 
        else running = false; 
    }else{ 
        if (timeout == 0) 
        { 
            pause = (pause) ? false : true; 
            timeout = 60; 
            printf("Game (un/)paused!");   
        } 
    }
}

void renderText(const char *text, int x, int y, SDL_Color color, int center)
{
    SDL_Surface *pauseText = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* textT = SDL_CreateTextureFromSurface(renderer, pauseText);
    SDL_FreeSurface(pauseText);
    SDL_Rect rect;
    SDL_QueryTexture( textT, NULL, NULL, &rect.w, &rect.h );
    switch (center)
    {
        case 0: rect.x = x; rect.y = y; break;                                              // Not centered, use x and y
        case 1: rect.y = height / 2 - rect.h / 2; rect.x = width / 2 - rect.w / 2; break;   // Centered in right in the middle of the screen
        case 2: rect.x = width / 2 - rect.w / 2; rect.y = y; break;                         // Centered horizontally
    }	
    SDL_RenderCopy(renderer, textT, NULL, &rect);
}

void darkenBackground()
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);
    SDL_Rect background = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &background);
}

void renderMenu()
{
    darkenBackground();
    renderText("New Game", -1, height / 2 - 50, (selected == 1) ? white : red, 2);
    renderText("Exit", -1, height / 2 + 50, (selected == 0) ? white : red, 2);
}

int main(int argc, char **argv) {
    srand(time(NULL));

    struct Ghost g2 = {
        pos: {
            x: 14,
            y: 12
        },
        direction: DOWN,
        homePos: {
            x: 14,
            y: 12
        },
        next: NULL
    };
    struct Ghost g = {
        pos: {
            x: 14,
            y: 15
        },
        direction: UP,
        homePos: {
            x: 14,
            y: 15
        },
        next: &g2
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
        ghost: &g,
        player: &player,
        powerUp: NULL
    };

    spawnPowerup(GROW, 20, &game);
    spawnPowerup(GHOSTS_TO_CENTER, 20, &game);
    spawnPowerup(GROW, 20, &game);

    width = 20 * map->width;
    height = 20 * map->length;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    SDL_Window *window = SDL_CreateWindow("PacSnake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    font = TTF_OpenFont("consola.ttf", 50);

    SDL_Texture *ghostTexture;
    SDL_Surface *imageLoader = SDL_LoadBMP("ghost.bmp");;
    ghostTexture = SDL_CreateTextureFromSurface(renderer, imageLoader);
    SDL_FreeSurface(imageLoader);

    int i = 0;
    SDL_Event event;
    printf("\n\n\n\t\t\t\tPress Enter to start the game!\n\n\n");
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            switch (event.key.keysym.sym)
            {
                case (SDLK_UP):
                case (SDLK_w): if (alive) turnPlayer(&player, UP); else { if (selected != 0) selected--;} break;
                case (SDLK_DOWN):
                case (SDLK_s): if (alive) turnPlayer(&player, DOWN); else { if (selected != 1) selected++;} break;
                case (SDLK_LEFT):
                case (SDLK_a): turnPlayer(&player, LEFT); break;
                case (SDLK_RIGHT):
                case (SDLK_d): turnPlayer(&player, RIGHT); break;
                case (SDLK_ESCAPE): running = false; break;
                case (SDLK_RETURN): handleReturn(); break;
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

        if (timeout != 0) timeout--;

        struct PowerUp *powerUp = game.powerUp;
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        while(powerUp)
        {
            SDL_Rect powerUpRect = {powerUp->pos.y * 20, powerUp->pos.x*20, 20, 20};
            SDL_RenderFillRect(renderer, &powerUpRect);
            powerUp = powerUp->next;
        }

        struct Ghost *ghost = game.ghost;
        while(ghost)
        {
            renderTexture(ghostTexture, ghost->pos.y * 20, ghost->pos.x * 20);
            ghost = ghost->next;
        }

        SDL_SetRenderDrawColor(renderer, 255, 110, 110, 255);
        SDL_Rect snek = {game.player->head.y * 20, game.player->head.x * 20, 20, 20};
        SDL_RenderFillRect(renderer, &snek);

        struct Tail *tail = game.player->tail;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        while(tail)
        {
            SDL_Rect schwanz = {tail->pos.y * 20, tail->pos.x*20, 20, 20};
            SDL_RenderFillRect(renderer, &schwanz);
            tail = tail->tail;
        }

        if (!alive)
        {
            renderMenu();
        }else if (pause){
            darkenBackground();
            renderText("GAME PAUSED", 0, 0, white, 1);
        }else{
            if (i == 15)
            {
                i = 0;
                movePlayer(game.player, &game);
                moveGhosts(game.ghost, &game);
            }
            i++;
        }

        SDL_RenderPresent(renderer);
    }
    //SDL_FreeSurface(screen);
    //SDL_FreeSurface(Ghost);
    //screen=NULL;
    //Ghost=NULL;
    TTF_CloseFont(font);
    SDL_DestroyWindow(window);
    SDL_Quit;

    return 0;
}
