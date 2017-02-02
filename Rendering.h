#ifndef _rendering_h
#define _rendering_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "PacSnake.h"
#include "Highscore.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


int selected;

SDL_Rect kasten;

SDL_Color white;
SDL_Color red;


void renderTexture(SDL_Texture *texture, int x, int y, int h, int w);

void renderText(const char *text, int x, int y, SDL_Color color, int center, int size);

void darkenBackground(bool black);

char * getName(struct GameState *state);

void handleReturn(struct GameState *state);

void renderMenu();

#endif