#include <stdlib.h>
#include <stdio.h>
#include "PowerUps.h"

void takePowerup(struct PowerUp *powerUp){
    switch (powerUp->type) {
        case EAT_GHOSTS: break;
        case SLOWER: break;
        case FASTER: break;
        case GHOSTS_TO_CENTER: break;
        case TURNAROUND: break;
    }
}
