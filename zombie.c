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
//Entities
struct zent *player = NULL;
struct zent **boxes = NULL;
//Counts current amount of ticks (for animations)
uint32_t tick = 0;

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

    if(DEBUGMODE) printf("Images loaded successfully. Trying to create player.\n");

    //Create player entity
    player = (struct zent*) malloc(sizeof(struct zent));
    if(player == NULL) return 1;
    *player = zent_make(img_player, 150.0, 100.0, ZOMBIE_PLAYER_SIZE, ZOMBIE_PLAYER_SIZE, 2);
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

    if(DEBUGMODE) printf("Player created successfully. Trying to create boxes.\n");

    //Create boxes array & fill
    boxes = (struct zent**) malloc(sizeof(struct zent*) * ZOMBIE_BOX_QUAN);
    if(boxes == NULL) return 1;
    if(zombie_boxes_make() != 0) return 1;

    if(DEBUGMODE) printf("Boxes created successfully. Trying to draw background.\n");

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
        //Allocate memory & create box
        boxes[i1] = (struct zent*) malloc(sizeof(struct zent));
        if(boxes[i1] == NULL) return 1;

        while(1) {
            *(boxes[i1]) = zent_make(img_box, rand() % (ZOMBIE_SCREEN_X-ZOMBIE_BOX_SIZE),
                rand() % (ZOMBIE_SCREEN_Y-ZOMBIE_BOX_SIZE), ZOMBIE_BOX_SIZE, ZOMBIE_BOX_SIZE, 1);

            if(zent_collide(player, boxes[i1])) continue;
            for(i2 = 0 ; i2 < i1 ; i2++)
                if(zent_collide(boxes[i1], boxes[i2])) break;
            if(i2 != i1) continue;

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
                if(player->vy == 0 && player->vx != 0) {
                    player->st &= 0xfc;
                    player->st |= 0x03;
                }
            }

            //Up arrow key
            if(e1.key.keysym.sym == SDLK_UP) {
                //Change velocity
                player->vy -= ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vx == 0 && player->vy != 0) {
                    player->st &= 0xfc;
                    player->st |= 0x00;
                }
            }

            //Right arrow key
            if(e1.key.keysym.sym == SDLK_RIGHT) {
                //Change velocity
                player->vx += ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vy == 0 && player->vx != 0) {
                    player->st &= 0xfc;
                    player->st |= 0x01;
                }
            }

            //Down arrow key
            if(e1.key.keysym.sym == SDLK_DOWN) {
                //Change velocity
                player->vy += ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vx == 0 && player->vy != 0) {
                    player->st &= 0xfc;
                    player->st |= 0x02;
                }
            }
        }

        //Releasing a key
        if(e1.type == SDL_KEYUP) {
            //Left arrow key
            if(e1.key.keysym.sym == SDLK_LEFT) {
                //Change velocity
                if(player->vx < 0) player->vx += ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vx > 0) {
                    player->st &= 0xfc;
                    player->st |= 0x01;
                }
            }

            //Up arrow key
            if(e1.key.keysym.sym == SDLK_UP) {
                //Change velocity
                if(player->vy < 0) player->vy += ZOMBIE_PLAYER_V;

                //Change facing direction
                if(player->vy > 0) {
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
    int i1;

    //Move player if he exists
    if(player != NULL)
    {
        player->x += player->vx;
        player->y += player->vy;
    }

    //Collisions: player-box
    for(i1 = 0 ; i1 < ZOMBIE_BOX_QUAN ; i1++) {
        //Inexistent boxes don't count
        if(boxes[i1] == NULL) continue;

        //If collision, stop player
        if(zent_collide(player, boxes[i1])) {
            player->x -= player->vx;
            player->y -= player->vy;
            player->vx = 0;
            player->vy = 0;
        }
    }

    return 0;
}

//Final stuff
int zombie_clear()
{
    int i1;

    //Free entity memory
    zent_clear(&player);
    for(i1 = 0 ; i1 < ZOMBIE_BOX_QUAN ; i1++)
        zent_clear(boxes + i1);
    free(boxes);
    boxes = NULL;

    //Free images
    SDL_FreeSurface(img_player);
    SDL_FreeSurface(img_zombie);
    SDL_FreeSurface(img_box);
    SDL_FreeSurface(tile);
    SDL_FreeSurface(background);
    SDL_Quit();

    return 0;
}

int main(int argc, char **argv)
{
    int i1;
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
        return 1;
    }

    //Main loop
    while(1)
    {
        //Store time of tick beginning
        t1=SDL_GetTicks();

        //Event processing
        if(zombie_event() != 0) goto end;

        //Update positions
        zombie_update();

        //Entity drawing: player
        if(zent_draw(player) != 0)
        {
            printf("Error while drawing entity.\n");
            return 1;
        }

        //Entity drawing: boxes
        for(i1 = 0 ; i1 < ZOMBIE_BOX_QUAN ; i1++) {
            if(boxes[i1] == NULL) continue;

            if(zent_draw(boxes[i1]) != 0) {
                printf("Error while drawing entity.\n");
                return 1;
            }
        }

        //Update screen
        if(SDL_Flip(screen) != 0)
        {
            printf("Error while updating screen.\n");
            return 3;
        }

        //Ticks must last at least specified time
        while(SDL_GetTicks() - t1 < ZOMBIE_TICK_MS);
        tick = (tick + 1) % ZOMBIE_MAX_TICK;
    }

end:
    //Final stuff
    zombie_clear();

    return 0;
}
