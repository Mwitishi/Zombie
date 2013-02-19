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
float *prevx;
float *prevy;

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
    return 0;
}
