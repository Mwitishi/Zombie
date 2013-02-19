/*
 *  File: zia.c / Project: Zombie
 *  Author: Mwitishi
 *  This file contains functions for the IA
 *  of the zombies in game Zombie.
 *  Macros, declarations & zent structure
 *  are in zombie.h.
 */

#include "zombie.h"

//IA variables
float *prevx = NULL;
float *prevy = NULL;

//Main IA function, runs all others
int zia_run()
{
    int i1;

    //Loop through each zombie
    for(i1 = 0 ; i1 < ZOMBIE_ZOMBIE_QUAN ; i1++) {
        //Don't process inexistent zombies
        if(zombies[i1] == NULL) continue;
    }

    return 0;
}

int zia_init()
{
    int i1;

    //Allocate array
    prevx = (float*) malloc(sizeof(float) * ZOMBIE_ZOMBIE_QUAN);
    if(prevx == NULL) return 1;

    //Initialize array empty
    for(i1 = 0 ; i1 < ZOMBIE_ZOMBIE_QUAN ; i1++)
        prevx[i1] = -1;

    //Allocate array
    prevy = (float*) malloc(sizeof(float) * ZOMBIE_ZOMBIE_QUAN);
    if(prevy == NULL) return 1;

    return 0;
}

int zia_clear()
{
    //Free array
    if(prevx != NULL) {
        free(prevx);
        prevx = NULL;
    }

    //Free array
    if(prevy != NULL) {
        free(prevy);
        prevy = NULL;
    }

    return 0;
}
