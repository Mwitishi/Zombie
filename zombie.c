//This definition avoids declaring extern variables
#define ZOMBIE_EXTERN

#include "zombie.h"

//Global variables
SDL_Surface *screen=NULL;
SDL_Surface *background=NULL;
SDL_Surface *tile=NULL;

int main(int argc,char **argv)
{
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
        printf("Error during SDL initializing");
        return 1;
    }

    //SDL closing
    SDL_Quit();

    return 0;
}
