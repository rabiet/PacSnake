#ifndef _highscore_h
#define _highscore_h

#include "PacSnake.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


struct highscore {
    char name[255];
    int score;
    struct highscore *next;
};

bool saved;

void addHS(struct highscore * newHs);

void loadHS();

void drawHS();

void writeHS(struct GameState *state);

#endif