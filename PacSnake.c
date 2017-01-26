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


int selected = 0;

SDL_Renderer *renderer;

int width;
int height;

SDL_Color white = {255, 255, 255, 255};
SDL_Color red   = {255, 0, 0, 255};

TTF_Font *font;

struct GameState *resetGame(struct GameState *state){
    // free old ghosts
    if(state && state->ghost){
        struct Ghost *g = state->ghost;
        while(g){
            struct Ghost *t = g;
            g = t->next;
            free(t);
        }
        state->ghost = NULL;
    }

    // create the ghosts
    struct Ghost *ghost = malloc(sizeof(struct Ghost));
    struct Ghost *ghost2 = malloc(sizeof(struct Ghost));

    ghost->pos.x = 14;
    ghost->pos.y = 15;
    ghost->direction = UP;
    ghost->homePos.x = 14;
    ghost->homePos.y = 15;
    ghost->next = ghost2;

    ghost2->pos.x = 14;
    ghost2->pos.y = 12;
    ghost2->direction = DOWN;
    ghost2->homePos.x = 14;
    ghost2->homePos.y = 12;
    ghost2->next = NULL;

    // free old map
    if(state && state->map){
        free(state->map);
        state->map = NULL;
    }

    struct Map *map = loadMap();
    if(map == NULL){
        printf("Coudn't load map...\n");
        return NULL;
    }

    // free old player
    if(state && state->player){
        if(state->player->tail){
            removeTails(state->player->tail);
        }
        free(state->player);
        state->player = NULL;
    }

    // create player
    struct Player *player = malloc(sizeof(struct Player));
    struct Tail *tail1 = malloc(sizeof(struct Tail));
    struct Tail *tail2 = malloc(sizeof(struct Tail));
    struct Tail *tail3 = malloc(sizeof(struct Tail));

    player->length = 4;
    player->head.x = 1;
    player->head.y = 2;
    player->direction = RIGHT;
    player->tail = tail1;

    tail1->tail = tail2;
    tail1->pos.x = 1;
    tail1->pos.y = 1;

    tail2->tail = tail3;
    tail2->pos.x = 2;
    tail2->pos.y = 1;

    tail3->tail = NULL;
    tail3->pos.x = 3;
    tail3->pos.y = 1;

    if(!state){
        state = malloc(sizeof(struct GameState));
    }

    // create gamestate
    state->map = map;
    state->ghost = ghost;
    state->player = player;
    state->powerUp = NULL;
    state->alive = false;
    state->running = true;
    state->pause = false;
    state->pauseTimeout = 60;
    state->speed = 15;
    state->powerUpSlowerTime = -1;
    state->powerUpFasterTime = -1;
    state->powerUpEatGhostsTime = -1;
    state->powerUpEatWallTime = -1;
    // create init powerups
    spawnPowerup(GROW, 0, state);
    spawnPowerup(GHOSTS_TO_CENTER, 0, state);
    spawnPowerup(SLOWER, 50, state);
    spawnPowerup(EAT_WALL, 20, state);
    spawnPowerup(SLOWER, 50, state);
    spawnPowerup(FASTER, 50, state);
    spawnPowerup(EAT_GHOSTS, 50, state);

    return state;
}

void renderTexture(SDL_Texture *texture, int x, int y)
{
    SDL_Rect helperRect;
    helperRect.x = x;
    helperRect.y = y;
    helperRect.h = 20;
    helperRect.w = 20;
    SDL_RenderCopy(renderer, texture, NULL, &helperRect);
}

void handleReturn(struct GameState *state)
{
    if (!state->alive)
    {
        if (selected == 0)
        {
            state = resetGame(state);
            state->alive = true;
        }
        else state->running = false;
    }else{
        if (state->pauseTimeout == 0)
        {
            state->pause = (state->pause) ? false : true;
            state->pauseTimeout = 60;
            printf("Game (un/)paused!\n");
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

    struct GameState *game = resetGame(NULL);

    if(!game){
        return -1;
    }

    width = 20 * game->map->width;
    height = 20 * game->map->length;

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
    while (game->running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                game->running = false;
            switch (event.key.keysym.sym)
            {
                case (SDLK_UP):
                case (SDLK_w): if (game->alive) turnPlayer(game->player, UP); else { if (selected != 0) selected--;} break;
                case (SDLK_DOWN):
                case (SDLK_s): if (game->alive) turnPlayer(game->player, DOWN); else { if (selected != 1) selected++;} break;
                case (SDLK_LEFT):
                case (SDLK_a): turnPlayer(game->player, LEFT); break;
                case (SDLK_RIGHT):
                case (SDLK_d): turnPlayer(game->player, RIGHT); break;
                case (SDLK_ESCAPE): game->running = false; break;
                case (SDLK_RETURN): handleReturn(game); break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);
        for (int k = 0; k < game->map->length; k++)
        {
            for (int j = 0; j < game->map->width; j++)
            {
                // let the walls blink if the eat wall powerup is active
                if(game->powerUpEatWallTime <= 0){
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                }else{
                    if(game->powerUpEatWallTime % 2 == 0){
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    }else{
                        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
                    }
                }
                if (game->map->fields[(k * game->map->width) + j] == 0)
                {
                    SDL_Rect wall = {20 * j, 20 * k, 20, 20};
                    SDL_RenderFillRect(renderer, &wall);
                }
            }
        }

        if (game->pauseTimeout != 0) game->pauseTimeout--;

        struct PowerUp *powerUp = game->powerUp;
        while(powerUp)
        {
            setPowerUpSDLRenderColor(renderer, powerUp);
            SDL_Rect powerUpRect = {powerUp->pos.y * 20, powerUp->pos.x*20, 20, 20};
            SDL_RenderFillRect(renderer, &powerUpRect);
            powerUp = powerUp->next;
        }

        struct Ghost *ghost = game->ghost;
        while(ghost)
        {
            renderTexture(ghostTexture, ghost->pos.y * 20, ghost->pos.x * 20);
            ghost = ghost->next;
        }

        SDL_SetRenderDrawColor(renderer, 255, 110, 110, 255);
        SDL_Rect snek = {game->player->head.y * 20, game->player->head.x * 20, 20, 20};
        SDL_RenderFillRect(renderer, &snek);

        struct Tail *tail = game->player->tail;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        while(tail)
        {
            SDL_Rect schwanz = {tail->pos.y * 20, tail->pos.x*20, 20, 20};
            SDL_RenderFillRect(renderer, &schwanz);
            tail = tail->tail;
        }

        if (!game->alive)
        {
            renderMenu();
        }else if (game->pause){
            darkenBackground();
            renderText("GAME PAUSED", 0, 0, white, 1);
        }else{
            if (i == game->speed)
            {
                i = 0;
                movePlayer(game->player, game);
                moveGhosts(game->ghost, game);
                updateTimedPowerUps(game);
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
