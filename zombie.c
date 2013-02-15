//This definition avoids declaring extern variables
#define ZOMBIE_EXTERN

#include "zombie.h"

//Global variables
SDL_Surface *screen=NULL;
SDL_Surface *background=NULL;
SDL_Surface *tile=NULL;

SDL_Surface *zombie_load_img(char *name)
{
    char *str1=NULL;
    SDL_Surface *img1,*img2;

    //Allocate memory for image path
    str1=(char*)malloc(strlen(ZOMBIE_IMG_FOLDER)+strlen(name)+1);
    if(str1==NULL) return NULL;

    //Make image path
    sprintf(str1,"%s%s",ZOMBIE_IMG_FOLDER,name);

    //Load image, free memory
    img1=IMG_Load(str1);
    free(str1);
    if(img1==NULL) return NULL;

    //Convert to display format, free old image
    img2=SDL_DisplayFormat(img1);
    SDL_FreeSurface(img1);

    //Return converted image
    return img2;
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
    tile=zombie_load_img(ZOMBIE_TILE_IMG);
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

int main(int argc,char **argv)
{
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

    //SDL initializing,check for error
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        printf("Error during SDL initializing.\n");
        return 1;
    }

    //Create screen, check for error
    screen=SDL_SetVideoMode(ZOMBIE_SCREEN_X,ZOMBIE_SCREEN_Y,ZOMBIE_BPP,SDL_SWSURFACE);
    if(screen==NULL)
    {
        printf("Error while creating window.\n");
        return 2;
    }

    //Load background
    if(zombie_background_make()!=0)
    {
        printf("Error while creating background.\n");
        return 3;
    }

    //Blit background to screen
    if(SDL_BlitSurface(background,NULL,screen,NULL)!=0)
    {
        printf("Error while blitting background to screen.\n");
        return 4;
    }

    //Update screen
    if(SDL_Flip(screen)!=0)
    {
        printf("Error while flipping screen.\n");
        return 5;
    }

    while(1)
    {
        //Store time of tick beginning
        t1=SDL_GetTicks();

        while(SDL_PollEvent(&e1)!=0)
        {
            if(e1.type==SDL_QUIT) goto end;
        }

        //Ticks must last at least specified time
        while(SDL_GetTicks()-t1<ZOMBIE_TICK_MS);
    }

end:
    //Free memory
    SDL_FreeSurface(tile);
    SDL_FreeSurface(background);

    //SDL closing
    SDL_Quit();

    return 0;
}
