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
#define ZOMBIE_VERSION "v0.2"

//File and directory names
#define ZOMBIE_IMG_FOLDER "imgs/"
#define ZOMBIE_TILE_IMG "zombie_tile.png"
#define ZOMBIE_PLAYER_IMG "zombie_player.png"
#define ZOMBIE_ZOMBIE_IMG "zombie_zombie.png"
#define ZOMBIE_BOX_IMG "zombie_box.png"

//Number macros
#define ZOMBIE_SCREEN_X 640
#define ZOMBIE_SCREEN_Y 480
#define ZOMBIE_BPP 32
#define ZOMBIE_TILE_SIZE 32
#define ZOMBIE_PLAYER_SIZE 32
#define ZOMBIE_ZOMBIE_SIZE 32
#define ZOMBIE_BOX_SIZE 32
#define ZOMBIE_ZOMBIE_QUAN 10
#define ZOMBIE_BOX_QUAN 5
#define ZOMBIE_PLAYER_V 3.5
#define ZOMBIE_PLAYER_FDIR_V 2
#define ZOMBIE_ZOMBIE_V 2.5
#define ZOMBIE_TICK_MS 100
#define ZOMBIE_MAX_TICK 16

//Global variable declarations
#ifndef ZOMBIE_EXTERN
#define ZOMBIE_EXTERN

extern SDL_Surface *screen;
extern SDL_Surface *background;
extern SDL_Surface *tile;
extern SDL_Surface *img_player;
extern SDL_Surface *img_zombie;
extern SDL_Surface *img_box;
extern struct zent *player;
extern struct zent **boxes;
extern uint32_t tick;

#endif

//Structure for entities (player, zombies, shots)
struct zent{
    //Image, with all possible states & frames
    SDL_Surface *img;
    //Last position, for deleting previous drawn image
    SDL_Rect lastbox;
    //Position with decimals
    float x;
    float y;
    //Velocity woth decimals
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
    SDL_Rect *hitbox;
    //Amount of rectangles in hitbox
    int qhit;
};

//Declarations of functions in file zombie.c
SDL_Surface *zombie_load_img(char*, char);
int zombie_init();
int zombie_background_make();
int zombie_boxes_make();
int zombie_event();
int zombie_update();
int zombie_clear();

//Declarations of functions in file zent.c
struct zent zent_make(SDL_Surface*, float, float, int, int, int);
int zent_draw(struct zent*);
int zent_clear(struct zent**);

#endif
