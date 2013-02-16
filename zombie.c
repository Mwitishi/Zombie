//This definition avoids declaring extern variables
#define ZOMBIE_EXTERN

#include "zombie.h"

//Global variables
SDL_Surface *screen=NULL;
SDL_Surface *background=NULL;
SDL_Surface *tile=NULL;
SDL_Surface *img_player=NULL;
SDL_Surface *img_zombie=NULL;
SDL_Surface *img_box=NULL;
struct zent *player=NULL;
struct zent **boxes=NULL;
//Counts current amount of ticks (for animations)
uint32_t tick=0;

//Function for loading an image and preparing it
SDL_Surface *zombie_load_img(char *name,char alpha)
{
    char *str1=NULL;
    SDL_Surface *img1,*img2;

    if(name==NULL) return NULL;

    //Allocate memory for image path
    str1=(char*)malloc(strlen(ZOMBIE_IMG_FOLDER)+strlen(name)+1);
    if(str1==NULL) return NULL;

    //Make image path
    sprintf(str1,"%s%s",ZOMBIE_IMG_FOLDER,name);

    //Load image, free memory
    img1=IMG_Load(str1);
    free(str1);
    str1=NULL;
    if(img1==NULL) return NULL;

    //Convert to display format, free old image
    if(alpha) img2=SDL_DisplayFormatAlpha(img1);
    else img2=SDL_DisplayFormat(img1);
    SDL_FreeSurface(img1);
    img1=NULL;

    //Return converted image
    return img2;
}

//Initializing stuff
int zombie_init()
{
    int i1;

    //SDL initializing,check for error
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
        return 1;

    //Create screen, check for error
    screen=SDL_SetVideoMode(ZOMBIE_SCREEN_X,ZOMBIE_SCREEN_Y,ZOMBIE_BPP,SDL_SWSURFACE);
    if(screen==NULL)
        return 1;

    SDL_WM_SetCaption(ZOMBIE_NAME,NULL);

    //Load background
    if(zombie_background_make()!=0)
        return 1;

    //Loading images
    img_player=zombie_load_img(ZOMBIE_PLAYER_IMG,1);
    if(img_player==NULL) return 1;

    img_zombie=zombie_load_img(ZOMBIE_ZOMBIE_IMG,1);
    if(img_zombie==NULL) return 1;

    img_box=zombie_load_img(ZOMBIE_BOX_IMG,0);
    if(img_box==NULL) return 1;

    //Create player entity
    player=(struct zent*)malloc(sizeof(struct zent));
    if(player==NULL) return 1;
    *player=zent_make(img_player,150.0,100.0,ZOMBIE_PLAYER_SIZE,ZOMBIE_PLAYER_SIZE,2);
    for(i1=0;i1<4;i1++) player->qfr[i1]=1;

    boxes=(struct zent**)malloc(sizeof(struct zent*)*ZOMBIE_BOX_QUAN);
    if(boxes==NULL) return 1;
    if(zombie_boxes_make()!=0) return 1;

    //Blit background to screen
    if(SDL_BlitSurface(background,NULL,screen,NULL)!=0)
        return 1;

    //Update screen
    if(SDL_Flip(screen)!=0)
        return 1;

    return 0;
}

//Final stuff
int zombie_clear()
{
    int i1;

    zent_clear(&player);
    for(i1=0;i1<ZOMBIE_BOX_QUAN;i1++)
        zent_clear(boxes+i1);
    free(boxes);
    boxes=NULL;

    SDL_FreeSurface(img_player);
    SDL_FreeSurface(img_zombie);
    SDL_FreeSurface(img_box);
    SDL_FreeSurface(tile);
    SDL_FreeSurface(background);
    SDL_Quit();

    return 0;
}

//Function for creating the background image
int zombie_background_make()
{
    int i1,i2;
    SDL_Rect r1;

    //Create empty image
    background=SDL_CreateRGBSurface(SDL_SWSURFACE,ZOMBIE_SCREEN_X,ZOMBIE_SCREEN_Y,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
    if(background==NULL) return 3;

    //Load background tile
    tile=zombie_load_img(ZOMBIE_TILE_IMG,0);
    if(tile==NULL) return 3;
    r1=tile->clip_rect;

    //Fill background with tiles
    for(i1=0;i1<ZOMBIE_SCREEN_X/ZOMBIE_TILE_SIZE;i1++)
        for(i2=0;i2<ZOMBIE_SCREEN_Y/ZOMBIE_TILE_SIZE;i2++)
        {
            r1.x=i1*ZOMBIE_TILE_SIZE;
            r1.y=i2*ZOMBIE_TILE_SIZE;
            if(SDL_BlitSurface(tile,&(tile->clip_rect),background,&r1)!=0)
                return 3;
        }

    return 0;
}

int zombie_boxes_make()
{
    int i1;

    srand(time(NULL));

    if(boxes==NULL) return 1;
    for(i1=0;i1<ZOMBIE_BOX_QUAN;i1++)
    {
        boxes[i1]=(struct zent*)malloc(sizeof(struct zent));
        if(boxes[i1]==NULL) return 1;
        *(boxes[i1])=zent_make(img_box,rand()%(ZOMBIE_SCREEN_X-ZOMBIE_BOX_SIZE),rand()%(ZOMBIE_SCREEN_Y-ZOMBIE_BOX_SIZE),ZOMBIE_BOX_SIZE,ZOMBIE_BOX_SIZE,1);
    }

    return 0;
}

int zombie_event()
{
    SDL_Event e1;

    //Event processing
    while(SDL_PollEvent(&e1)!=0) {
        //Pressing X button
        if(e1.type==SDL_QUIT) return 1;

        //Pressing a key
        if(e1.type==SDL_KEYDOWN) {
            //Left arrow key
            if(e1.key.keysym.sym==SDLK_LEFT) {
                //Change velocity
                player->vx-=ZOMBIE_PLAYER_V;
                //Change facing direction
                if(player->vy==0&&player->vx!=0) {
                    player->st&=0xfc;
                    player->st|=0x03;
                }
            }

            //Up arrow key
            if(e1.key.keysym.sym==SDLK_UP) {
                //Change velocity
                player->vy-=ZOMBIE_PLAYER_V;
                //Change facing direction
                if(player->vx==0&&player->vy!=0) {
                    player->st&=0xfc;
                    player->st|=0x00;
                }
            }

            //Right arrow key
            if(e1.key.keysym.sym==SDLK_RIGHT) {
                //Change velocity
                player->vx+=ZOMBIE_PLAYER_V;
                //Change facing direction
                if(player->vy==0&&player->vx!=0) {
                    player->st&=0xfc;
                    player->st|=0x01;
                }
            }

            //Down arrow key
            if(e1.key.keysym.sym==SDLK_DOWN) {
                //Change velocity
                player->vy+=ZOMBIE_PLAYER_V;
                //Change facing direction
                if(player->vx==0&&player->vy!=0) {
                    player->st&=0xfc;
                    player->st|=0x02;
                }
            }
        }

        //Releasing a key
        if(e1.type==SDL_KEYUP) {
            //Left arrow key
            if(e1.key.keysym.sym==SDLK_LEFT) {
                //Change velocity
                player->vx+=ZOMBIE_PLAYER_V;
                //Change facing direction
                if(player->vx>0) {
                    player->st&=0xfc;
                    player->st|=0x01;
                }
            }

            //Up arrow key
            if(e1.key.keysym.sym==SDLK_UP) {
                //Change velocity
                player->vy+=ZOMBIE_PLAYER_V;
                //Change facing direction
                if(player->vy>0) {
                    player->st&=0xfc;
                    player->st|=0x02;
                }
            }

            //Right arrow key
            if(e1.key.keysym.sym==SDLK_RIGHT) {
                //Change velocity
                player->vx-=ZOMBIE_PLAYER_V;
                //Change facing direction
                if(player->vx<0) {
                    player->st&=0xfc;
                    player->st|=0x03;
                }
            }

            //Down arrow key
            if(e1.key.keysym.sym==SDLK_DOWN) {
                //Change velocity
                player->vy-=ZOMBIE_PLAYER_V;
                //Change facing direction
                if(player->vy<0) {
                    player->st&=0xfc;
                    player->st|=0x00;
                }
            }
        }
    }

    return 0;
}

int zombie_update()
{
    if(player!=NULL)
    {
        player->x+=player->vx;
        player->y+=player->vy;
    }

    return 0;
}

int main(int argc,char **argv)
{
    int i1;
    uint32_t t1;
    SDL_Event e1;

    //Print game data
    printf("%s %s\n",ZOMBIE_NAME,ZOMBIE_VERSION);

    //If arguments passed
    if(argc>1)
    {
        //Show game information
        if(strcmp(argv[1],"info")==0)
            printf("%s is an action game in 2D about killing zombies.\n",ZOMBIE_NAME);

        return 0;
    }

    //Initialize
    if(zombie_init()!=0)
    {
        printf("Error during initializing.\n");
        return 1;
    }

    //Main loop
    while(1)
    {
        //Store time of tick beginning
        t1=SDL_GetTicks();

        if(zombie_event()!=0) goto end;

        zombie_update();

        //Entity drawing
        if(zent_draw(player)!=0)
        {
            printf("Error while drawing entity.\n");
            return 1;
        }
        for(i1=0;i1<ZOMBIE_BOX_QUAN;i1++)
        {
            if(boxes[i1]==NULL) continue;

            if(zent_draw(boxes[i1])!=0)
            {
                printf("Error while drawing entity.\n");
                return 1;
            }
        }

        //Update screen
        if(SDL_Flip(screen)!=0)
        {
            printf("Error while updating screen.\n");
            return 3;
        }

        //Ticks must last at least specified time
        while(SDL_GetTicks()-t1<ZOMBIE_TICK_MS);
        tick=(tick+1)%ZOMBIE_MAX_TICK;
    }

end:
    //Final stuff
    zombie_clear();

    return 0;
}
