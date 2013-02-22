/*
 *  File: zombie.c / Project: Zombie
 *  Author: Mwitishi
 *  This file contains the main function and other code chunks
 *  put in separate functions for convenience.
 *  Declarations, includes and macros are in zombie.h.
 *  Global variables of the program are declared here
 *  but extern declarations are in zombie.h.
 *  Video, keyboard and audio are managed using
 *  SDL library, freely distributed.
 *  This is C, not C++.
 *  Tested under Ubuntu (with gcc) and Windows
 *  (cross-compiled in Ubuntu using MinGW).
 */

//This definition avoids declaring extern variables
#define ZOMBIE_EXTERN

#include "zombie.h"

//Global variables
//Screen & images
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *tile = NULL;
SDL_Surface *img_player = NULL;
SDL_Surface *img_zombie = NULL;
SDL_Surface *img_box = NULL;
SDL_Surface *img_shot = NULL;
//Entities
struct zent *player = NULL;
struct zent **zombies = NULL;
struct zent **boxes = NULL;
struct zent **shots = NULL;
//Counters
uint32_t tick = 0;
int reload = 0;

//Function for loading an image and preparing it
SDL_Surface *zombie_load_img(char *name, char alpha)
{
    //Stores image path
    char *str1 = NULL;
    //Store pre- and post-conversion images
    SDL_Surface *img1 = NULL, *img2 = NULL;

    if(DEBUGMODE) printf("Trying to load image.\n");

    //Check if valid name
    if(name == NULL) return NULL;

    if(DEBUGMODE) printf("Image name: %s. Trying to make path.\n", name);

    //Allocate memory for image path
    str1 = (char*) malloc(strlen(ZOMBIE_IMG_FOLDER) + strlen(name) + 1);
    if(str1 == NULL) return NULL;

    if(DEBUGMODE) printf("Memory for str1 allocated.\n");

    //Make image path
    sprintf(str1, "%s%s", ZOMBIE_IMG_FOLDER, name);

    if(DEBUGMODE) printf("Image path: %s. Trying to load image.\n", str1);

    //Load image, free memory
    img1 = IMG_Load(str1);
    free(str1);
    str1 = NULL;
    if(img1 == NULL) return NULL;

    if(DEBUGMODE) printf("Image loaded into img1. Trying to convert to required format.\n");

    //Convert to display format, free old image
    if(alpha) img2 = SDL_DisplayFormatAlpha(img1);
    else img2 = SDL_DisplayFormat(img1);
    SDL_FreeSurface(img1);
    img1 = NULL;

    if(DEBUGMODE) printf("Image loaded and returned successfully.\n");

    //Return converted image
    return img2;
}

//Initializing stuff
int zombie_init()
{
    int i1;

    if(DEBUGMODE) printf("Trying to initialize SDL.\n");

    //SDL initializing,check for error
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return 1;

    if(DEBUGMODE) printf("SDL initialized successfully. Trying to create screen.\n");

    //Create screen, check for error
    screen = SDL_SetVideoMode(ZOMBIE_SCREEN_X,ZOMBIE_SCREEN_Y,ZOMBIE_BPP,SDL_SWSURFACE);
    if(screen == NULL) return 1;

    if(DEBUGMODE) printf("Screen created successfully. Trying to set title.\n");

    //Set window title
    SDL_WM_SetCaption(ZOMBIE_NAME, NULL);

    if(DEBUGMODE) printf("Title set successfully. Trying to create background.\n");

    //Load background
    if(zombie_background_make() != 0) return 1;

    if(DEBUGMODE) printf("Background created successfully. Trying to load images.\n");

    //Loading images
    img_player = zombie_load_img(ZOMBIE_PLAYER_IMG, 1);
    if(img_player == NULL) return 1;

    img_zombie = zombie_load_img(ZOMBIE_ZOMBIE_IMG, 1);
    if(img_zombie == NULL) return 1;

    img_box = zombie_load_img(ZOMBIE_BOX_IMG ,0);
    if(img_box == NULL) return 1;

    img_shot = zombie_load_img(ZOMBIE_SHOT_IMG, 0);
    if(img_shot == NULL) return 1;

    if(DEBUGMODE) printf("Images loaded successfully. Trying to create player.\n");

    //Create player entity
    player = (struct zent*) malloc(sizeof(struct zent));
    if(player == NULL) return 1;
    *player = zent_make(img_player, 150.0, 100.0, ZOMBIE_PLAYER_SIZE, ZOMBIE_PLAYER_SIZE, ZOMBIE_PLAYER_TPF);
    for(i1 = 0 ; i1 < 4 ; i1++) {
        player->qfr[i1] = 1;
//      player->qhit[i1] = 2;
//      free(player->hitbox[i1]);
//      player->hitbox[i1] = (SDL_Rect*) malloc(sizeof(SDL_Rect) * 2);
    }

//  player->hitbox[0][0].x = 0; player->hitbox[0][0].y = 18;
//  player->hitbox[0][0].w = 32; player->hitbox[0][0].h = 14;
//  player->hitbox[0][1].x = 24; player->hitbox[0][1].y = 0;
//  player->hitbox[0][1].w = 8;  player->hitbox[0][1].h = 18;

    if(DEBUGMODE) printf("Player created successfully. Trying to create zombies.\n");

    //Create zombies array (filling done in main loop)
    zombies = (struct zent**) malloc(sizeof(struct zent*) * ZOMBIE_ZOMBIE_QUAN);
    if(zombies == NULL) return 1;
    for(i1 = 0 ; i1 < ZOMBIE_ZOMBIE_QUAN ; i1++) zombies[i1] = NULL;

    if(DEBUGMODE) printf("Zombies created successfully. Trying to create boxes.\n");

    //Create boxes array & fill
    boxes = (struct zent**) malloc(sizeof(struct zent*) * ZOMBIE_BOX_QUAN);
    if(boxes == NULL) return 1;
    if(zombie_boxes_make() != 0) return 1;

    if(DEBUGMODE) printf("Boxes created successfully. Trying to create shots.\n");

    //Create shots array, leave it empty
    shots = (struct zent**) malloc(sizeof(struct zent*) * ZOMBIE_SHOT_QUAN);
    if(shots == NULL) return 1;
    for(i1 = 0 ; i1 < ZOMBIE_SHOT_QUAN ; i1++)
        shots[i1] = NULL;

    if(DEBUGMODE) printf("Shots created successfully. Trying to start IA.\n");

    if(zia_init() != 0) return 1;

    if(DEBUGMODE) printf("IA started successfully. Trying to draw background.\n");

    //Blit background to screen
    if(SDL_BlitSurface(background, NULL, screen, NULL) != 0)
        return 1;

    if(DEBUGMODE) printf("Trying to update screen.\n");

    //Update screen
    if(SDL_Flip(screen) != 0)
        return 1;

    return 0;
}

//Function for creating the background image
int zombie_background_make()
{
    int i1, i2;
    SDL_Rect r1;

    //Create empty image
    background = SDL_CreateRGBSurface(SDL_SWSURFACE, ZOMBIE_SCREEN_X, ZOMBIE_SCREEN_Y, 32,
        0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    if(background == NULL) return 3;

    //Load background tile
    tile = zombie_load_img(ZOMBIE_TILE_IMG, 0);
    if(tile == NULL) return 3;
    r1 = tile->clip_rect;

    //Fill background with tiles
    for(i1 = 0 ; i1 < ZOMBIE_SCREEN_X / ZOMBIE_TILE_SIZE ; i1++)
        for(i2 = 0;i2 < ZOMBIE_SCREEN_Y / ZOMBIE_TILE_SIZE ; i2++)
        {
            //Fill rectangle data
            r1.x = i1 * ZOMBIE_TILE_SIZE;
            r1.y = i2 * ZOMBIE_TILE_SIZE;
            //Blit tile to correspondent position, check error
            if(SDL_BlitSurface(tile, &(tile->clip_rect), background, &r1) != 0)
                return 3;
        }

    return 0;
}

//Place box entities randomly across the screen
int zombie_boxes_make()
{
    int i1,i2;

    //Initialize random seed
    srand(time(NULL));

    //Check if array exists
    if(boxes == NULL) return 1;

    //Loop through array
    for(i1 = 0 ; i1 < ZOMBIE_BOX_QUAN ; i1++) {
        //Allocate memory
        boxes[i1] = (struct zent*) malloc(sizeof(struct zent));
        if(boxes[i1] == NULL) return 1;

        //Try until finding a free position
        while(1) {
            //Create box
            *(boxes[i1]) = zent_make(img_box,
                rand() % (ZOMBIE_SCREEN_X - ZOMBIE_BOX_SIZE - 2 * ZOMBIE_BOX_MARG) + ZOMBIE_BOX_MARG,
                rand() % (ZOMBIE_SCREEN_Y - ZOMBIE_BOX_SIZE - 2 * ZOMBIE_BOX_MARG) + ZOMBIE_BOX_MARG,
                ZOMBIE_BOX_SIZE, ZOMBIE_BOX_SIZE, 1);

            //Check for collisions
            if(zent_collide(player, boxes[i1])) continue;
            for(i2 = 0 ; i2 < i1 ; i2++)
                if(zent_collide(boxes[i1], boxes[i2])) break;
            if(i2 != i1) continue;

            break;
        }
    }

    return 0;
}

//Function for generating zombies, called continuously
int zombie_zombies_make()
{
    int i1,i2,i3;

    //Loop through array, fill empty places
    for(i1 = 0 ; i1 < ZOMBIE_ZOMBIE_QUAN ; i1++) {
        //Only process empty entries
        if(zombies[i1] != NULL) continue;

        //Check probability
        if(rand() % ZOMBIE_ZOMBIE_GEN_PROB != 0) continue;

        //Allocate memory
        zombies[i1] = (struct zent*) malloc(sizeof(struct zent));
        if(zombies[i1] == NULL) return 1;

        //Attempt to create until finding free space
        for(i2 = 0 ; i2 < ZOMBIE_GEN_TRY ; i2++) {
            //Make zombies, place randomly across right edge
            *(zombies[i1]) = zent_make(img_zombie, ZOMBIE_SCREEN_X - ZOMBIE_ZOMBIE_SIZE,
                rand() % (ZOMBIE_SCREEN_Y - ZOMBIE_ZOMBIE_SIZE),
                ZOMBIE_ZOMBIE_SIZE, ZOMBIE_ZOMBIE_SIZE, ZOMBIE_ZOMBIE_TPF);

            //Check collision
            if(zent_collide(player, zombies[i1]) != 0) continue;
            for(i3 = 0 ; i3 < ZOMBIE_ZOMBIE_QUAN ; i3++)
                if(zent_collide(zombies[i3], zombies[i1]) != 0 ) break;
            if(i3 != i1) continue;

            //Add details
            zombies[i1]->st = 0x03;

            break;
        }
    }

    return 0;
}

//Function for processing keyboard & other input
int zombie_event()
{
    SDL_Event e1;

    //Loop through all pending events
    while(SDL_PollEvent(&e1) != 0) {
        //Pressing X button
        if(e1.type == SDL_QUIT) return 1;

        //Pressing a key
        if(e1.type == SDL_KEYDOWN) {
            //Left arrow key
            if(e1.key.keysym.sym == SDLK_LEFT) {
                //Change velocity
                player->vx -= ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vy == 0) {
                    player->st &= 0xfc;
                    player->st |= 0x03;
                }
            }

            //Up arrow key
            if(e1.key.keysym.sym == SDLK_UP) {
                //Change velocity
                player->vy -= ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vx == 0) {
                    player->st &= 0xfc;
                    player->st |= 0x00;
                }
            }

            //Right arrow key
            if(e1.key.keysym.sym == SDLK_RIGHT) {
                //Change velocity
                player->vx += ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vy == 0) {
                    player->st &= 0xfc;
                    player->st |= 0x01;
                }
            }

            //Down arrow key
            if(e1.key.keysym.sym == SDLK_DOWN) {
                //Change velocity
                player->vy += ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vx == 0) {
                    player->st &= 0xfc;
                    player->st |= 0x02;
                }
            }

            if(e1.key.keysym.sym == SDLK_SPACE)
                zombie_shoot();
        }

        //Releasing a key
        if(e1.type == SDL_KEYUP) {
            //Left arrow key
            if(e1.key.keysym.sym == SDLK_LEFT) {
                //Change velocity
                if(player->vx < 0) player->vx += ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vx < 0) {
                    player->st &= 0xfc;
                    player->st |= 0x01;
                }
            }

            //Up arrow key
            if(e1.key.keysym.sym == SDLK_UP) {
                //Change velocity
                if(player->vy < 0) player->vy += ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vy < 0) {
                    player->st &= 0xfc;
                    player->st |= 0x02;
                }
            }

            //Right arrow key
            if(e1.key.keysym.sym == SDLK_RIGHT) {
                //Change velocity
                if(player->vx > 0) player->vx -= ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vx < 0) {
                    player->st &= 0xfc;
                    player->st |= 0x03;
                }
            }

            //Down arrow key
            if(e1.key.keysym.sym == SDLK_DOWN) {
                //Change velocity
                if(player->vy > 0) player->vy -= ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vy < 0) {
                    player->st &= 0xfc;
                    player->st |= 0x00;
                }
            }
        }
    }

    return 0;
}

//Update entity positions and stuff
int zombie_update()
{
    int i1,i2;

    //Move player if he exists
    if(player != NULL)
    {
        player->x += player->vx;
        player->y += player->vy;
    }

    //Move zombies (AI can only alter velocity)
    for(i1 = 0 ; i1 < ZOMBIE_ZOMBIE_QUAN ; i1++) {
        if(zombies[i1] == NULL) continue;

        zombies[i1]->x += zombies[i1]->vx;
        zombies[i1]->y += zombies[i1]->vy;
    }

    //Move shots
    for(i1 = 0 ; i1 < ZOMBIE_SHOT_QUAN ; i1++) {
        if(shots[i1] == NULL) continue;

        //Update position
        shots[i1]->x += shots[i1]->vx;
        shots[i1]->y += shots[i1]->vy;

        //If shot goes out of screen
        if(shots[i1]->x < 0 || shots[i1]->x > ZOMBIE_SCREEN_X - ZOMBIE_SHOT_SIZE ||
            shots[i1]->y < 0 || shots[i1]->y > ZOMBIE_SCREEN_Y - ZOMBIE_SHOT_SIZE) {
            //Delete shot
            zent_clear(shots + i1);
        }
    }

    //Collisions: boxes
    for(i1 = 0 ; i1 < ZOMBIE_BOX_QUAN ; i1++) {
        //Inexistent boxes don't count
        if(boxes[i1] == NULL) continue;

        //If collision with player, stop player
        if(zent_collide(player, boxes[i1])) {
            player->x -= player->vx;
            player->y -= player->vy;
            player->vx = 0;
            player->vy = 0;
        }

        //If collision with zombie, stop zombie
        for(i2 = 0 ; i2 < ZOMBIE_ZOMBIE_QUAN ; i2++)
           if(zent_collide(boxes[i1], zombies[i2]) != 0) {
                zombies[i2]->x -= zombies[i2]->vx;
                zombies[i2]->y -= zombies[i2]->vy;
                zombies[i2]->vx = 0;
                zombies[i2]->vy = 0;
            }

        //If collision with shot, delete shot
        for(i2 = 0 ; i2 < ZOMBIE_SHOT_QUAN ; i2++)
            if(zent_collide(boxes[i1], shots[i2]) != 0) zent_clear(shots + i2);
    }

    //Collisions: shots
    for(i1 = 0 ; i1 < ZOMBIE_SHOT_QUAN ; i1++) {
        //If collision with player, remove shot (& reduce health)
        if(zent_collide(shots[i1], player) != 0)
            zent_clear(shots + i1);

        //If collision with zombie, remove both entities
        for(i2 = 0 ; i2 < ZOMBIE_ZOMBIE_QUAN ; i2++)
            if(zent_collide(shots[i1], zombies[i2]) != 0) {
                zent_clear(shots + i1);
                zent_clear(zombies + i2);
            }
    }

    //Collisions: zombies
    for(i1 = 0 ; i1 < ZOMBIE_ZOMBIE_QUAN ; i1++) {
        //If collision with player, stop both (& reduce health)
        if(zent_collide(zombies[i1], player) != 0) {
            zombies[i1]->x -= zombies[i1]->vx;
            zombies[i1]->y -= zombies[i1]->vy;
            zombies[i1]->vx = 0;
            zombies[i1]->vy = 0;

            player->x -= player->vx;
            player->y -= player->vy;
            player->vx = 0;
            player->vy = 0;
        }
    }

    return 0;
}

int zombie_draw()
{
    int i1;

    //Entity drawing: player
    if(zent_draw(player) != 0) return 1;

    //Entity drawing: zombies
    for(i1 = 0 ; i1 < ZOMBIE_ZOMBIE_QUAN ; i1++) {
        if(zombies[i1] == NULL) continue;

        if(zent_draw(zombies[i1]) != 0) return 1;
    }

    //Entity drawing: boxes
    for(i1 = 0 ; i1 < ZOMBIE_BOX_QUAN ; i1++) {
        if(boxes[i1] == NULL) continue;

        if(zent_draw(boxes[i1]) != 0) return 1;
    }

    //Entity drawing: shots
    for(i1 = 0 ; i1 < ZOMBIE_SHOT_QUAN ; i1++) {
        if(shots[i1] == NULL) continue;

        if(zent_draw(shots[i1]) != 0) return 1;
    }

    //Update screen
    if(SDL_Flip(screen) != 0) return 1;

    return 0;
}

//Function for shooting (when pressing SPACE)
int zombie_shoot()
{
    int i1;
    float x = 0, y = 0, vx = 0, vy = 0;

    //Null checking
    if(shots == NULL) return 1;
    if(player == NULL) return 0;
    //If reloading
    if(reload != 0) return 0;

    //Getting free position in array
    for(i1 = 0 ; i1 < ZOMBIE_SHOT_QUAN ; i1++)
        if(shots[i1] == NULL) break;

    //If array full
    if(i1 == ZOMBIE_SHOT_QUAN) return 0;

    //Calculate position & velocity
    //Facing up
    if((player->st & 0x03) == 0) {
        x = player->x + 27;
        y = player->y - ZOMBIE_SHOT_SIZE;
        vx = 0;
        vy = -ZOMBIE_SHOT_V;
    }

    //Facing right
    if((player->st & 0x03) == 1) {
        x = player->x + ZOMBIE_PLAYER_SIZE;
        y = player->y + 27;
        vx = ZOMBIE_SHOT_V;
        vy = 0;
    }

    //Facing down
    if((player->st & 0x03) == 2) {
        x = player->x + 3;
        y = player->y + ZOMBIE_PLAYER_SIZE;
        vx = 0;
        vy = ZOMBIE_SHOT_V;
    }

    //Facing left
    if((player->st & 0x03) == 3) {
        x = player->x + ZOMBIE_SHOT_SIZE;
        y = player->y + 3;
        vx = -ZOMBIE_SHOT_V;
        vy = 0;
    }

    //Memory allocating
    shots[i1] = (struct zent*) malloc(sizeof(struct zent));
    if(shots[i1] == NULL) return 1;

    //Create the shot
    *(shots[i1]) = zent_make(img_shot, x, y, ZOMBIE_SHOT_SIZE, ZOMBIE_SHOT_SIZE, ZOMBIE_SHOT_TPF);
    shots[i1]->vx = vx;
    shots[i1]->vy = vy;

    //Start reloading
    reload = ZOMBIE_RELOAD_TIME;

    return 0;
}

//Final stuff
int zombie_clear()
{
    int i1;

    //Clear IA
    zia_clear();

    //Free player
    zent_clear(&player);

    //Free each zombie
    for(i1 = 0 ; i1 < ZOMBIE_ZOMBIE_QUAN ; i1++)
        zent_clear(zombies + i1);

    //Free zombie array
    free(zombies);
    zombies = NULL;

    //Free each box
    for(i1 = 0 ; i1 < ZOMBIE_BOX_QUAN ; i1++)
        zent_clear(boxes + i1);

    //Free box array
    free(boxes);
    boxes = NULL;

    //Free each shot
    for(i1 = 0 ; i1 < ZOMBIE_SHOT_QUAN ; i1++)
        zent_clear(shots + i1);

    //Free shot array
    free(shots);
    shots = NULL;

    //Free images
    SDL_FreeSurface(img_shot);
    SDL_FreeSurface(img_player);
    SDL_FreeSurface(img_zombie);
    SDL_FreeSurface(img_box);
    SDL_FreeSurface(tile);
    SDL_FreeSurface(background);

    //Quit SDL
    SDL_Quit();

    return 0;
}

//Main function. Parameters are only for info display.
int main(int argc, char **argv)
{
    uint32_t t1;

    //Print game data
    printf("%s %s\n", ZOMBIE_NAME, ZOMBIE_VERSION);

    //If arguments passed
    if(argc > 1)
    {
        //Show game information
        if(strcmp(argv[1], "info") == 0)
            printf("%s is an action game in 2D about killing zombies.\n", ZOMBIE_NAME);

        return 0;
    }

    //Initialize
    if(zombie_init() != 0)
    {
        printf("Error during initializing.\n");
        goto end;
    }

    //Main loop
    while(1)
    {
        //Store time of tick beginning
        t1=SDL_GetTicks();

        //Event processing
        if(zombie_event() != 0) goto end;

        //Create zombies if necessary
        if(zombie_zombies_make() != 0) goto end;

        if(zia_run() != 0) goto end;

        //Update positions
        zombie_update();

        if(zombie_draw() != 0) {
            printf("Error while drawing screen.\n");
            goto end;
        }

        //Ticks must last at least specified time
        while(SDL_GetTicks() - t1 < ZOMBIE_TICK_MS);
        tick = (tick + 1) % ZOMBIE_MAX_TICK;
        //Reloading time decreased
        if(reload > 0) reload--;
    }

end:
    //Final stuff
    zombie_clear();

    return 0;
}
