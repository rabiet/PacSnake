#include "PacSnake.h"
#include "Rendering.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>


bool saved = false;

void writeHS(struct GameState *state)
{
    FILE *f = fopen("history.db", "w");
    if (f == NULL)
    {
        return;
    }
    fprintf(f, "%s|%d;%s", name, state->score, oldHS);
    fclose(f);
    saved = true;
}

void loadHS()
{
    char leaderBoard[1024];
    FILE *fptr;

    if ((fptr = fopen("history.db", "r")) == NULL){
       return;
   }

   fscanf(fptr, "%1023[0-9a-zA-Z;|!?.,äöü ]", &leaderBoard);
   fclose(fptr);
   strcpy(oldHS, leaderBoard);
   return;
}

void drawHS()
{
    darkenBackground(true);
    char string[1024];
    strcpy(string, oldHS);
    char delimiter[] = "|;";
    char *ptr;
    ptr = strtok(string, delimiter);
    renderText("Game History", 0, fieldHeight, white, 2, width / 10);
    int which = 0;
    while(ptr != NULL)
    {
        renderText(ptr, 0, (height / 4) + ((which / 2) * (width / 20)), white, (which % 2) == 0 ? 3 : 4, width / 30);
        which++;
     	ptr = strtok(NULL, delimiter);
        if (which == 16) ptr = NULL;
    }
}
