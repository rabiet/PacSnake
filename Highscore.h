#ifndef _highscore_h
#define _highscore_h

#include "PacSnake.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


bool saved;

void writeHS(struct GameState *state);

void loadHS();

void drawHS();

#endif