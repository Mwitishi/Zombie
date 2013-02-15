#ifndef ZOMBIE_H
#define ZOMBIE_H

//External libraries
#include <stdio.h>
#include <SDL/SDL.h>

//Basic macros
#define ZOMBIE_NAME "Zombie by Mwitishi"
#define ZOMBIE_VERSION "v0.0"

//Global variable declarations
#ifndef ZOMBIE_EXTERN
#define ZOMBIE_EXTERN

extern SDL_Surface *screen;
extern SDL_Surface *background;
extern SDL_Surface *tile;

#endif

#endif
