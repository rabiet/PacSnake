#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "PacSnake.h"
#include "Ghosts.h"
#include "PowerUps.h"
#include "Player.h"
#include "Position.h"
#include "Map.h"
#include "Highscore.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


int selected = 0;

bool saved;
SDL_Rect kasten;

SDL_Color white = {255, 255, 255, 255};
SDL_Color red   = {255, 0, 0, 255};

TTF_Font *font;

void renderTexture(SDL_Texture *texture, int x, int y, int h, int w)
{
    SDL_Rect helperRect;
    helperRect.x = x;
    helperRect.y = y;
    helperRect.h = (h != 0) ? h : fieldHeight;
    helperRect.w = (w != 0) ? w : fieldWidth;
    SDL_RenderCopy(renderer, texture, NULL, &helperRect);
}

void renderText(const char *text, int x, int y, SDL_Color color, int center, int size)
{
    if (size == 0) size = (width / 10);
    font = TTF_OpenFont("consola.ttf", size);
    SDL_Surface *pauseText = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* textT = SDL_CreateTextureFromSurface(renderer, pauseText);
    SDL_FreeSurface(pauseText);
    SDL_Rect rect;
    SDL_QueryTexture(textT, NULL, NULL, &rect.w, &rect.h);
    switch (center)
    {
        case 0: rect.x = x; rect.y = y; break;                                              // Not centered, use x and y
        case 1: rect.y = height / 2 - rect.h / 2; rect.x = width / 2 - rect.w / 2; break;   // Centered in right in the middle of the screen
        case 2: rect.x = width / 2 - rect.w / 2; rect.y = y; break;                         // Centered horizontally
        case 3: rect.x = width / 5; rect.y = y; break;
        case 4: rect.x = width - width / 5 - rect.w; rect.y = y; break;
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

void getName(struct GameState *state)
{
    bool done = false;
    char nam[64] = "";
    SDL_StartTextInput();
    while (!done) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.key.keysym.sym == SDLK_RETURN && state->pauseTimeout == 0) { done = true; }
            switch (event.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_TEXTINPUT:
                    strcat(nam, event.text.text);
                    break;
            }
        }
        if (state->pauseTimeout != 0) state->pauseTimeout--;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderText("Name:", 0, height / 2 - (width / 7), white, 2, width / 10);
        renderText(nam, 0, 0, white, 1, width / 15);
        SDL_RenderPresent(renderer);
    }
    strcpy(name, nam);
}

void handleReturn(struct GameState *state)
{
    if (state->alive == 0 && state->pauseTimeout == 0)
    {
        switch (selected)
        {
            case 0:
                state->pauseTimeout = 30;
                getName(state);
                state = resetGame(state);
                state->alive = 1;
                break;
            case 1:
                state->alive = -2;
                break;
            case 2:
                loadHS();
                state->alive = -1;
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