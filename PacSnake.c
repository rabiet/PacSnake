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
bool symbols;
SDL_Rect kasten;

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

    // free old map
    if(state && state->map){
        free(state->map);
        state->map = NULL;
    }

    // free old player
    if(state && state->player){
        if(state->player->tail){
            removeTails(state->player->tail);
        }
        free(state->player);
        state->player = NULL;
    }

    if(!state){
        state = malloc(sizeof(struct GameState));
        state->ghost = NULL;
        state->player = NULL;
        state->map = NULL;
    }

    int success = loadMap(state);
    if(!success){
        printf("Coudn't load map...\n");
        return NULL;
    }

    // create gamestate
    state->powerUp = NULL;
    state->alive = 0;
    state->running = true;
    state->pause = false;
    state->pauseTimeout = 60;
    state->score = 0;
    state->maxScore = 0;
    state->speed = 15;
    state->powerUpSlowerTime = 0;
    state->powerUpFasterTime = 0;
    state->powerUpEatGhostsTime = 0;
    state->powerUpEatWallTime = 0;

    // create init powerups
    spawnPowerup(GROW, 0, state);
    spawnPowerup(GROW, 0, state);
    spawnPowerup(GROW, 0, state);

    return state;
}

void renderTexture(SDL_Texture *texture, int x, int y, int h, int w)
{
    SDL_Rect helperRect;
    helperRect.x = x;
    helperRect.y = y;
    helperRect.h = (h != 0) ? h : fieldHeight;
    helperRect.w = (w != 0) ? w : fieldWidth;
    SDL_RenderCopy(renderer, texture, NULL, &helperRect);
}

void handleReturn(struct GameState *state)
{
    if (state->alive == 0 && state->pauseTimeout == 0)
    {
        switch (selected)
        {
            case 0:
                state = resetGame(state);
                state->alive = 1;
                break;
            case 1:
                symbols = true;
                break;
            case 2:
                printf("Not implemented");
                break;
            case 3:
                state->running = false;
                break;
        }
    }else if (state->alive > 1){
        if (state->pauseTimeout == 0) { state->alive = 0; state->pauseTimeout = 30; }
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

void darkenBackground(bool black)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, black ? 255 : 225);
    SDL_Rect background = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &background);
}

void renderMenu()
{
    darkenBackground(true);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &kasten);
    renderText("PACSNAKE", -1, fieldHeight, white, 2, height / 5);
    renderText("New Game", -1, height / 2 - (height / 5), (selected == 0) ? red : white, 2, width / 15);
    renderText("Symbols", -1, height / 2 - height / 15, (selected == 1) ? red : white, 2, width / 15);
    renderText("Highscore", -1, height / 2 + height / 15, selected == 2 ? red : white, 2, width / 15);
    renderText("Exit", -1, height / 2 + (height / 5), (selected == 3) ? red : white, 2, width / 15);
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

    kasten.x = (width / 4);
    kasten.y = (height / 4);
    kasten.h = height / 1.7;
    kasten.w = width / 2;

    printf("width: %d\nheight: %d\nfieldHeight: %d\noffset: %d\nlength: %d\nwidth: %d\n", width, height, fieldHeight, offset, game->map->length, game->map->width);


    SDL_Window *window = SDL_CreateWindow("PacSnake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture *ghostTexture;
    SDL_Texture *speedDownTexture;
    SDL_Texture *speedUpTexture;
    SDL_Texture *eatEnemyTexture;
    SDL_Texture *homeTexture;
    SDL_Texture *turnTexture;
    SDL_Texture *eatWallTexture;
    SDL_Texture *pointTexture;


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

    imageLoader = SDL_LoadBMP("eatWall.bmp");
    eatWallTexture = SDL_CreateTextureFromSurface(renderer, imageLoader);
    SDL_FreeSurface(imageLoader);

    imageLoader = SDL_LoadBMP("points.bmp");
    pointTexture = SDL_CreateTextureFromSurface(renderer, imageLoader);
    SDL_FreeSurface(imageLoader);


    int i = 0;
    SDL_Event event;


    while (game->running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                game->running = false;
            switch (event.key.keysym.sym)
            {
                case (SDLK_UP):
                case (SDLK_w): if (game->alive == 1) turnPlayer(game->player, UP); else { if (selected != 0 && game->pauseTimeout == 0) selected--; game->pauseTimeout = 5; } break;
                case (SDLK_DOWN):
                case (SDLK_s): if (game->alive == 1) turnPlayer(game->player, DOWN); else { if (selected != 3 && game->pauseTimeout == 0) selected++; game->pauseTimeout = 5; } break;
                case (SDLK_LEFT):
                case (SDLK_a): turnPlayer(game->player, LEFT); break;
                case (SDLK_RIGHT):
                case (SDLK_d): turnPlayer(game->player, RIGHT); break;
                case (SDLK_BACKSPACE): symbols = false; break;
                case (SDLK_ESCAPE): game->running = false; break;
                case (SDLK_RETURN): handleReturn(game); break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        if (symbols)
        {
            renderTexture(homeTexture, fieldHeight * 6, fieldWidth * 2, fieldHeight * 3, fieldWidth * 3);
            renderText("Sends all ghosts home", fieldHeight * 10, fieldHeight * 3, white, 0, width / 30);
            renderTexture(speedDownTexture, fieldHeight * 6, fieldWidth * 6, fieldHeight * 3, fieldWidth * 3);
            renderText("Slows down time and the universe", fieldHeight * 10, fieldHeight * 7, white, 0, width / 30);
            renderTexture(speedUpTexture, fieldHeight * 6, fieldWidth * 10, fieldHeight * 3, fieldWidth * 3);
            renderText("Speeds everything up", fieldHeight * 10, fieldHeight * 11, white, 0, width / 30);
            renderTexture(eatEnemyTexture, fieldHeight * 6, fieldWidth * 14, fieldHeight * 3, fieldWidth * 3);
            renderText("Gives you time to eat your enemies", fieldHeight * 10, fieldHeight * 15, white, 0, width / 30);
            renderTexture(turnTexture, fieldHeight * 6, fieldWidth * 18, fieldHeight * 3, fieldWidth * 3);
            renderText("Turns you around 180-Style", fieldHeight * 10, fieldHeight * 19, white, 0, width / 30);
            renderTexture(eatWallTexture, fieldHeight * 6, fieldWidth * 22, fieldHeight * 3, fieldWidth * 3);
            renderText("Lets you eat walls... for a while", fieldHeight * 10, fieldHeight * 23, white, 0, width / 30);
            renderTexture(pointTexture, fieldHeight * 6, fieldWidth * 26, fieldHeight * 3, fieldWidth * 3);
            renderText("Enlarges you and raises your score", fieldHeight * 10, fieldHeight * 27, white, 0, width / 30);

            renderText("Back", fieldHeight, fieldHeight, red, 0, width / 25);
        }else{
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
                        }else if(game->powerUpEatWallTime < 6){
                            SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
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
                        renderTexture(eatEnemyTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight, 0, 0);
                        break;
                    case SLOWER:
                        renderTexture(speedDownTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight, 0, 0);
                        break;
                    case FASTER:
                        renderTexture(speedUpTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight, 0, 0);
                        break;
                    case GHOSTS_TO_CENTER:
                        renderTexture(homeTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight, 0, 0);
                        break;
                    case TURNAROUND:
                        renderTexture(turnTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight, 0, 0);
                        break;
                    case EAT_WALL:
                        renderTexture(eatWallTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight, 0, 0);
                        break;
                    case GROW:
                        renderTexture(pointTexture, powerUp->pos.y * fieldWidth + offset, powerUp->pos.x * fieldHeight, 0, 0);
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
                renderTexture(ghostTexture, ghost->pos.y * fieldWidth + offset, ghost->pos.x * fieldHeight, 0, 0);
                ghost = ghost->next;
            }

            SDL_SetRenderDrawColor(renderer, 255, 110, 110, 255);
            SDL_Rect snek = {game->player->head.y * fieldWidth + offset, game->player->head.x * fieldHeight, fieldWidth, fieldHeight};
            SDL_RenderFillRect(renderer, &snek);

            struct Tail *tail = game->player->tail;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            int curscore = 0;
            while(tail)
            {
                SDL_Rect schwanz = {tail->pos.y * fieldWidth + offset, tail->pos.x*fieldHeight, fieldWidth, fieldHeight};
                SDL_RenderFillRect(renderer, &schwanz);
                tail = tail->tail;
                curscore++;
            }

            if(curscore > game->maxScore){
                game->maxScore = curscore;
            }

            game->score = curscore;

            char *scoreText =(char*) malloc(20 * sizeof(char));;
            sprintf(scoreText, "%s %d", "Length: ", game->score);
            renderText(scoreText, 0, 0, white, 0, (width / 30));

            char *maxScoreText =(char*) malloc(20 * sizeof(char));;
            sprintf(maxScoreText, "%s %d", "Score: ", game->maxScore);
            renderText(maxScoreText, 0, (width / 30) + 5, white, 0, (width / 30));

            int moveDown = 0;
            if (game->powerUpSlowerTime != 0)
            {
                char *slowerText =(char*) malloc(20 * sizeof(char));;
                sprintf(slowerText, "%s %d", "Slow Movement:", game->powerUpSlowerTime);
                renderText(slowerText, offset + (fieldWidth * game->map->width), moveDown * (width / 30) + 5, white, 0, (width / 70));
                moveDown++;
            }
            if (game->powerUpFasterTime != 0)
            {
                char *fasterText =(char*) malloc(20 * sizeof(char));;
                sprintf(fasterText, "%s %d", "Fast Movement:", game->powerUpFasterTime);
                renderText(fasterText, offset + (fieldWidth * game->map->width), moveDown * (width / 30) + 5, white, 0, (width / 70));
                moveDown++;
            }
            if (game->powerUpEatGhostsTime != 0)
            {
                char *eatGhostText =(char*) malloc(20 * sizeof(char));;
                sprintf(eatGhostText, "%s %d", "Eat Ghosts:", game->powerUpEatGhostsTime);
                renderText(eatGhostText, offset + (fieldWidth * game->map->width), moveDown * (width / 30) + 5, white, 0, (width / 70));
                moveDown++;
            }
            if (game->powerUpEatWallTime != 0)
            {
                char *eatWallText =(char*) malloc(20 * sizeof(char));;
                sprintf(eatWallText, "%s %d", "Eat Walls:", game->powerUpEatWallTime);
                renderText(eatWallText, offset + (fieldWidth * game->map->width), moveDown * (width / 30) + 5, white, 0, (width / 70));
                moveDown++;
            }

            if (game->alive == 0)
            {
                renderMenu();
            }else if (game->alive > 1){
                darkenBackground(true);
                renderText("You died!", 0, height / 2 - (height / 5), red, 2, width / 7);
                renderText((game->alive == 2) ? "You ran into a wall!" : "Eaten by a ghost!", 0, height / 2, white, 2, width / 20);
                char *scoreText =(char*) malloc(20 * sizeof(char));;
                sprintf(scoreText, "%s %d", "Score:", game->score);
                renderText(scoreText, 0, height / 2 + (height / 5), white, 2, width / 20);
                free(scoreText);
            }
            else if (game->pause){
                darkenBackground(true);
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
        }
        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font);
    SDL_DestroyWindow(window);
    SDL_Quit;

    return 0;
}
