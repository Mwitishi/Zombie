#ifndef ZOMBIE_H
#define ZOMBIE_H

//External libraries
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

//Basic macros
#define ZOMBIE_NAME "Zombie by Mwitishi"
#define ZOMBIE_VERSION "v0.1"

#define ZOMBIE_IMG_FOLDER ""
#define ZOMBIE_TILE_IMG "zombie_tile.png"
#define ZOMBIE_PLAYER_IMG "zombie_player.png"
#define ZOMBIE_ZOMBIE_IMG "zombie_zombie.png"

#define ZOMBIE_SCREEN_X 640
#define ZOMBIE_SCREEN_Y 480
#define ZOMBIE_BPP 32
#define ZOMBIE_TILE_SIZE 32

#define ZOMBIE_TICK_MS 100

//Global variable declarations
#ifndef ZOMBIE_EXTERN
#define ZOMBIE_EXTERN

extern SDL_Surface *screen;
extern SDL_Surface *background;
extern SDL_Surface *tile;
extern SDL_Surface *img_player;
extern SDL_Surface *img_zombie;

#endif

#endif
