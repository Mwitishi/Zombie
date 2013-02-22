/*
 *  File: zent.c / Project: Zombie
 *  Author: Mwitishi
 *  This file contains functions for manipulating
 *  zent structures, defined in zombie.h.
 *  Function declarations, extern variables
 *  and macros are in zombie.h.
 *  zent structures contain information
 *  about entities in the game Zombie.
 */

#include "zombie.h"

//Function for creating a zent structure with specified values
struct zent zent_make(SDL_Surface *img, float x, float y, int w, int h, int tpf)
{
    struct zent ze1;
    int i1;

    ze1.img = img;
    ze1.dimg = NULL;
    ze1.x = x;
    ze1.y = y;
    //Velocity must be set afterwards
    ze1.vx = 0;
    ze1.vy = 0;
    ze1.w = w;
    ze1.h = h;
    //Set lastbox position to inexistent
    ze1.lastbox.x = -1;
    ze1.lastbox.y = (int)y;
    ze1.lastbox.w = w;
    ze1.lastbox.h = h;
    ze1.tpf = tpf;
    ze1.st = 0;
    //Calculate state amount from image
    ze1.qst = img->h / h;

    //Make frames array, set all values to maximum (in image)
    ze1.qfr = (int*) malloc(ze1.qst * sizeof(int));
    for(i1 = 0 ; i1 < ze1.qst ; i1++)
        ze1.qfr[i1] = img->w / w;

    //Default hitbox: 1 rectangle, size of the entity
    ze1.hitbox = (SDL_Rect**) malloc(sizeof(SDL_Rect*) * ze1.qst);
    ze1.qhit = (int*) malloc(sizeof(int) * ze1.qst);
    for(i1 = 0 ; i1 < ze1.qst ; i1++) {
        ze1.qhit[i1] = 1;
        ze1.hitbox[i1] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
        //Hitbox positions measured from upperleft corner of the entity
        ze1.hitbox[i1][0].x = 0;
        ze1.hitbox[i1][0].y = 0;
        ze1.hitbox[i1][0].w = w;
        ze1.hitbox[i1][0].h = h;
    }

    return ze1;
}

//Function for drawing an entity on the screen
int zent_draw(struct zent *ze1)
{
    SDL_Rect r1, r2;

    //Null checking
    if(screen == NULL) return 1;
    if(background == NULL) return 1;
    if(ze1 == NULL) return 0;
    if(ze1->img == NULL) return 1;

    //Initialize rectangles
    r1.w = r2.w = ze1->w;
    r1.h = r2.h = ze1->h;
    r1.x = (tick / (ze1->tpf)) % (ze1->qfr[ze1->st]);
    r1.y = (ze1->st) * (ze1->h);
    r2.x = (int) (ze1->x);
    r2.y = (int) (ze1->y);

    //Do not draw if unneccesary
//  if(r2.x == ze1->lastbox.x && r2.y == ze1->lastbox.y &&
//      (tick / ze1->tpf) % ze1->qfr[ze1->st] ==
//      ((tick - 1) / ze1->tpf) % ze1->qfr[ze1->st]) return 0;

    //Empty previous position, draw at new position, check error
    if((ze1->lastbox.x != -1 &&
        SDL_BlitSurface(background, &(ze1->lastbox), screen, &(ze1->lastbox)) != 0) ||
        SDL_BlitSurface(ze1->img, &r1, screen, &r2) != 0) return 1;

    //Update lastbox, next drawing will erase this one
    ze1->lastbox = r2;

    return 0;
}

//Function for freeing memory and stuff
int zent_clear(struct zent **ze1)
{
    int i1;

    //If null pointers
    if(ze1 == NULL) return 1;
    if(*ze1 == NULL) return 0;

    //Draw image
    SDL_BlitSurface(background, &((*ze1)->lastbox), screen, &((*ze1)->lastbox));
    //Draw death image
    if((*ze1)->dimg != NULL) {
        SDL_BlitSurface((*ze1)->dimg,NULL,background,&((*ze1)->lastbox));
        SDL_BlitSurface(background,&((*ze1)->lastbox),screen,&((*ze1)->lastbox));
    }

    //Free hitbox array
    if((*ze1)->hitbox != NULL) {
        for(i1 = 0 ; i1 < (*ze1)->qst ; i1++) {
            if((*ze1)->hitbox[i1] == NULL) continue;

            free((*ze1)->hitbox[i1]);
            (*ze1)->hitbox[i1] = NULL;
        }

        free((*ze1)->hitbox);
        (*ze1)->hitbox = NULL;
    }

    //Free qhit array
    if((*ze1)->qhit != NULL) {
        free((*ze1)->qhit);
        (*ze1)->qhit = NULL;
    }

    //Free qfr array
    if((*ze1)->qfr != NULL) {
        free((*ze1)->qfr);
        (*ze1)->qfr = NULL;
    }

    //Free memory, set pointer to NULL
    free(*ze1);
    *ze1 = NULL;

    return 0;
}

//Function for checking if entities collide
int zent_collide(struct zent *ze1, struct zent *ze2)
{
    int i1,i2;
    SDL_Rect r1,r2;

    //If entities are NULL, assume there aren't collisions
    if(ze1 == NULL || ze2 == NULL ||
        ze1->hitbox == NULL || ze2->hitbox == NULL) return 0;

    //Loop through hitbox rectangles in both entities
    for(i1 = 0 ; i1 < ze1->qhit[ze1->st] ; i1++)
        for(i2 = 0 ; i2 < ze2->qhit[ze2->st] ; i2++)
        {
            //Create hitboxes, set positions relative to screen position
            r1 = ze1->hitbox[ze1->st][i1];
            r1.x += (int) ze1->x;
            r1.y += (int) ze1->y;
            r2 = ze2->hitbox[ze2->st][i2];
            r2.x += (int) ze2->x;
            r2.y += (int) ze2->y;

            //If no collision, check next combination
            if(r1.x + r1.w < r2.x ||
                r1.x > r2.x + r2.w ||
                r1.y + r1.h < r2.y ||
                r1.y > r2.y + r2.h) continue;

            //If collision, return true
            return 1;
        }

    //If no collisions overall, return false
    return 0;
}
