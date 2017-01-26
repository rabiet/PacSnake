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
int fieldHeight;
int fieldWidth;
int offset;
int score;

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
    score = 0;

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
    spawnPowerup(GROW, 0, state);
    spawnPowerup(GROW, 0, state);

    return state;
}

void renderTexture(SDL_Texture *texture, int x, int y)
{
    SDL_Rect helperRect;
    helperRect.x = x;
    helperRect.y = y;
    helperRect.h = fieldHeight;
    helperRect.w = fieldWidth;
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

void renderText(const char *text, int x, int y, SDL_Color color, int center, int size)
{
    if (size == 0) size = (width / 10);
    font = TTF_OpenFont("consola.ttf", size);
    SDL_Surface *pauseText = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* textT = SDL_CreateTextureFromSurface(renderer, pauseText);
    SDL_FreeSurface(pauseText);
    SDL_Rect rect;
    SDL_QueryTexture(textT, NULL, NULL, &rect.w, &rect.h);
    switch (center)
    {
        case 0: rect.x = x; rect.y = y; break;                                              // Not centered, use x and y
        case 1: rect.y = height / 2 - rect.h / 2; rect.x = width / 2 - rect.w / 2; break;   // Centered in right in the middle of the screen
        case 2: rect.x = width / 2 - rect.w / 2; rect.y = y; break;                         // Centered horizontally
    }
    SDL_RenderCopy(renderer, textT, NULL, &rect);
    SDL_DestroyTexture(textT);
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
    renderText("New Game", -1, height / 2 - (width / 10), (selected == 1) ? white : red, 2, 0);
    renderText("Exit", -1, height / 2 + (width / 10), (selected == 0) ? white : red, 2, 0);
}

int main(int argc, char **argv) {
    srand(time(NULL));

    struct GameState *game = resetGame(NULL);

    if(!game){
        return -1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Rect r;
    if (SDL_GetDisplayBounds(0, &r) != 0) {
        SDL_Log("SDL_GetDisplayBounds failed: %s", SDL_GetError());
        return 1;
    }

    width = r.w;
    height = r.h;
    fieldHeight = height / game->map->length;
    fieldWidth = fieldHeight;
    offset = (width / 2) - (fieldWidth * (game->map->width / 2));

    printf("width: %d\nheight: %d\nfieldHeight: %d\noffset: %d\nlength: %d\nwidth: %d\n", width, height, fieldHeight, offset, game->map->length, game->map->width);


    SDL_Window *window = SDL_CreateWindow("PacSnake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture *ghostTexture;
    SDL_Texture *speedDownTexture;
    SDL_Texture *speedUpTexture;
    SDL_Texture *eatEnemyTexture;
    SDL_Texture *homeTexture;
    SDL_Texture *turnTexture;


    SDL_Surface *imageLoader = SDL_LoadBMP("ghost.bmp");;
    ghostTexture = SDL_CreateTextureFromSurface(renderer, imageLoader);
    SDL_FreeSurface(imageLoader);

    imageLoader = SDL_LoadBMP("eatEnemy.bmp");;
    eatEnemyTexture = SDL_CreateTextureFromSurface(renderer, imageLoader);
    SDL_FreeSurface(imageLoader);

    imageLoader = SDL_LoadBMP("speedUp.bmp");;
    speedUpTexture = SDL_CreateTextureFromSurface(renderer, imageLoader);
    SDL_FreeSurface(imageLoader);
    
    imageLoader = SDL_LoadBMP("speedDown.bmp");;
    speedDownTexture = SDL_CreateTextureFromSurface(renderer, imageLoader);
    SDL_FreeSurface(imageLoader);

    imageLoader = SDL_LoadBMP("home.bmp");
    homeTexture = SDL_CreateTextureFromSurface(renderer, imageLoader);
    SDL_FreeSurface(imageLoader);

    imageLoader = SDL_LoadBMP("turn.bmp");
    turnTexture = SDL_CreateTextureFromSurface(renderer, imageLoader);
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
                    SDL_Rect wall = {fieldWidth * j + offset, fieldHeight * k, fieldWidth, fieldHeight};
                    SDL_RenderFillRect(renderer, &wall);
                }
            }
        }

        if (game->pauseTimeout != 0) game->pauseTimeout--;

        struct PowerUp *powerUp = game->powerUp;
        while(powerUp)
        {
            switch (powerUp->type)
            {
                case EAT_GHOSTS:
                    renderTexture(eatEnemyTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight);
                    break;
                case SLOWER:
                    renderTexture(speedDownTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight);
                    break;
                case FASTER:
                    renderTexture(speedUpTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight);
                    break;
                case GHOSTS_TO_CENTER:
                    renderTexture(homeTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight);
                    break;
                case TURNAROUND:
                    renderTexture(turnTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight);
                    break;
                default:
                    setPowerUpSDLRenderColor(renderer, powerUp);
                    SDL_Rect powerUpRect = {powerUp->pos.y * fieldWidth + offset, powerUp->pos.x*fieldHeight, fieldWidth, fieldHeight};
                    SDL_RenderFillRect(renderer, &powerUpRect);
                    break;

            }
            powerUp = powerUp->next;
        }

        struct Ghost *ghost = game->ghost;
        while(ghost)
        {
            renderTexture(ghostTexture, ghost->pos.y * fieldWidth + offset, ghost->pos.x * fieldHeight);
            ghost = ghost->next;
        }

        SDL_SetRenderDrawColor(renderer, 255, 110, 110, 255);
        SDL_Rect snek = {game->player->head.y * fieldWidth + offset, game->player->head.x * fieldHeight, fieldWidth, fieldHeight};
        SDL_RenderFillRect(renderer, &snek);

        struct Tail *tail = game->player->tail;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        score = 0;
        while(tail)
        {
            SDL_Rect schwanz = {tail->pos.y * fieldWidth + offset, tail->pos.x*fieldHeight, fieldWidth, fieldHeight};
            SDL_RenderFillRect(renderer, &schwanz);
            tail = tail->tail;
            score++;
        }

        char *scoreText =(char*) malloc(13 * sizeof(char));;
        sprintf(scoreText, "%s %d", "Score: ", score);
        renderText(scoreText, 0, 0, white, 0, (width / 30));

        if (!game->alive)
        {
            renderMenu();
        }else if (game->pause){
            darkenBackground();
            renderText("GAME PAUSED", 0, 0, white, 1, 0);
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
    TTF_CloseFont(font);
    SDL_DestroyWindow(window);
    SDL_Quit;

    return 0;
}
