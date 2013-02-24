/*
 *  File: zombie.h / Project: Zombie
 *  Author: Mwitishi
 *  This is the main include file for project Zombie.
 *  All source files in the project include it.
 *  All other include files (if any) are included here.
 *  All external library includes are here.
 *  Macros are defined here.
 *  Extern declarations are here, but
 *  global variables are declared in zombie.c.
 *  The zent structure is defined here.
 *  Declarations of functions are here.
 */

#ifndef ZOMBIE_H
#define ZOMBIE_H

//External libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//SDL libraries
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

//Basic macros
#define ZOMBIE_NAME "Zombie by Mwitishi"
#define ZOMBIE_VERSION "v0.4"
#define DEBUGMODE 0

//File and directory names
#define ZOMBIE_IMG_FOLDER "imgs/"
#define ZOMBIE_TILE_IMG "zombie_tile.png"
#define ZOMBIE_PLAYER_IMG "zombie_player.png"
#define ZOMBIE_ZOMBIE_IMG "zombie_zombie.png"
#define ZOMBIE_ZDEAD_IMG "zombie_zdead.png"
#define ZOMBIE_BOX_IMG "zombie_box.png"
#define ZOMBIE_SHOT_IMG "zombie_shot.png"
#define ZOMBIE_HEALTH_IMG "zombie_health.png"

//Number macros
#define ZOMBIE_SCREEN_X 640
#define ZOMBIE_SCREEN_Y 480
#define ZOMBIE_BPP 32
#define ZOMBIE_TILE_SIZE 32
#define ZOMBIE_PLAYER_SIZE 32
#define ZOMBIE_ZOMBIE_SIZE 32
#define ZOMBIE_BOX_SIZE 32
#define ZOMBIE_SHOT_SIZE 2
#define ZOMBIE_HEALTH_SIZE 16
#define ZOMBIE_BOX_MARG 32
#define ZOMBIE_HEALTH_MARG 16
#define ZOMBIE_ZOMBIE_DIFF 16
#define ZOMBIE_ZOMBIE_QUAN 5
#define ZOMBIE_BOX_QUAN 10
#define ZOMBIE_SHOT_QUAN 10
#define ZOMBIE_PLAYER_V 3.5
#define ZOMBIE_PLAYER_FDIR_V 2
#define ZOMBIE_ZOMBIE_V 2.5
#define ZOMBIE_SHOT_V 10
#define ZOMBIE_PLAYER_TPF 2
#define ZOMBIE_ZOMBIE_TPF 1
#define ZOMBIE_BOX_TPF 1
#define ZOMBIE_SHOT_TPF 1
#define ZOMBIE_ZOMBIE_GEN_PROB 100
#define ZOMBIE_GEN_TRY 3
#define ZOMBIE_MAX_HEALTH 10
#define ZOMBIE_HIT_HLOSS 1
#define ZOMBIE_RELOAD_TIME 3
#define ZOMBIE_TICK_MS 100
#define ZOMBIE_MAX_TICK 16

//Global variable declarations (macro defining avoids double declarations)
#ifndef ZOMBIE_EXTERN
#define ZOMBIE_EXTERN

extern SDL_Surface *screen;
extern SDL_Surface *background;
extern SDL_Surface *tile;
extern SDL_Surface *img_player;
extern SDL_Surface *img_zombie;
extern SDL_Surface *img_zdead;
extern SDL_Surface *img_box;
extern struct zent *player;
extern struct zent **zombies;
extern struct zent **boxes;
extern struct zent **shots;
extern uint32_t tick;
extern int reload;
extern int health;

#endif

//Structure for entities (player, zombies, shots)
struct zent{
    //Image, with all possible states & frames, & image when death
    SDL_Surface *img;
    SDL_Surface *dimg;
    //Last position, for deleting previous drawn image
    SDL_Rect lastbox;
    SDL_Rect dbox;
    //Position with decimals
    float x;
    float y;
    //Velocity with decimals
    float vx;
    float vy;
    //Entity size
    int w;
    int h;
    //Current state - number of states
    int st;
    int qst;
    //Ticks per frame
    int tpf;
    //Frame amounts (array, 1 entry per state)
    int *qfr;
    //Hitbox, as multiple rectangles
    SDL_Rect **hitbox;
    //Amount of rectangles in hitbox
    int *qhit;
};

//Declarations of functions in file zombie.c
SDL_Surface *zombie_load_img(char*, char);
int zombie_init();
int zombie_background_make();
int zombie_boxes_make();
int zombie_event();
int zombie_update();
int zombie_shoot();
int zombie_clear();

//Declarations of functions in file zent.c
struct zent zent_make(SDL_Surface*, float, float, int, int, int);
int zent_draw(struct zent*);
int zent_clear(struct zent**);
int zent_collide(struct zent*, struct zent*);

//Declarations of functions in file zia.c
int zia_run();
int zia_init();
int zia_clear();

#endif
