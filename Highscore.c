#include "PacSnake.h"
#include "Rendering.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>


extern bool saved;

void addHS(struct highscore *newHs)
{
    if (hs == NULL)
        hs = newHs;
    else {
        if (hs->score < newHs->score)
        {
            newHs->next = hs;
            hs = newHs;
        } else {
            struct highscore* current = hs;
            while (current->next != NULL && current->next->score > newHs->score)
                current = current->next;

            newHs->next = current->next;
            current->next = newHs;
        }
    }
}

void loadHS()
{
    struct highscore *toDelete = hs;
    while (toDelete != NULL)
    {
        hs = toDelete->next;
        free(toDelete);
        toDelete = hs;
    }
    char leaderBoard[4096];
    FILE *fptr;

    if ((fptr = fopen("history.db", "r")) == NULL){
       return;
   }

    char name[256];
    int score = 0;
    while (fscanf(fptr, " %[0-9a-zA-zäöüÄÖÜ !?]|%d;", name, &score) != EOF)
    {
        struct highscore *newHS = malloc(sizeof(struct highscore));
        strcpy(newHS->name, name);
        newHS->score = score;
        newHS->next = NULL;
        addHS(newHS);
    }

   fclose(fptr);
}

void drawHS()
{
    darkenBackground(true);
    renderText("Game History", 0, fieldHeight, white, 2, width / 10);
    struct highscore *current = hs;
    int i = 0;
    while(current != NULL && i < 10)
    {
        renderText(current->name, 0, (height / 4) + (i * (height / 16)), white, 3, width / 30);
        char scoreString[10];
        sprintf(scoreString, "%d", current->score);
        renderText(scoreString, 0, (height / 4) + (i * (height / 16)), white, 4, width / 30);
        current = current->next;
        i++;
    }
}

void writeHS(struct GameState *state)
{
    loadHS();
    struct highscore *newHs = malloc(sizeof(struct highscore));
    newHs->score = state->maxScore;
    strcpy(newHs->name, name);
    newHs->next = NULL;
    addHS(newHs);

    FILE *f = fopen("history.db", "w");
    if (f == NULL)
    {
        return;
    }
    struct highscore *current = hs;
    while(current != NULL)
    {
        fprintf(f, "%s|%d;", current->name, current->score);
        current = current->next;
    }

    
    fclose(f);
    saved = true;
}