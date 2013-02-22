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

    //Check NULL
    if(zombies == NULL) return 1;

    //Loop through each zombie
    for(i1 = 0 ; i1 < ZOMBIE_ZOMBIE_QUAN ; i1++) {
        //Don't process inexistent zombies
        if(zombies[i1] == NULL) continue;

        //Stop zombie
        zombies[i1]->vx = 0;
        zombies[i1]->vy = 0;

        //If moving left
        if(zombies[i1]->x - player->x > ZOMBIE_ZOMBIE_DIFF) {
            zombies[i1]->vx = -ZOMBIE_ZOMBIE_V;
            zombies[i1]->st &= 0xfc;
            zombies[i1]->st |= 0x03;
            continue;
        }

        //If moving right
        if(player->x - zombies[i1]->x > ZOMBIE_ZOMBIE_DIFF) {
            zombies[i1]->vx = ZOMBIE_ZOMBIE_V;
            zombies[i1]->st &= 0xfc;
            zombies[i1]->st |= 0x01;
            continue;
        }

        //If moving up
        if(zombies[i1]->y - player->y > ZOMBIE_ZOMBIE_DIFF) {
            zombies[i1]->vy = -ZOMBIE_ZOMBIE_V;
            zombies[i1]->st &= 0xfc;
            zombies[i1]->st |= 0x00;
            continue;
        }

        //If moving down
        if(player->y - zombies[i1]->y > ZOMBIE_ZOMBIE_DIFF) {
            zombies[i1]->vy = ZOMBIE_ZOMBIE_V;
            zombies[i1]->st &= 0xfc;
            zombies[i1]->st |= 0x02;
            continue;
        }
    }

    return 0;
}

//Initialization of the IA
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

//Function for cleaning up the IA variables & stuff
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
