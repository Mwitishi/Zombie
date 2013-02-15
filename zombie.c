#include "zombie.h"

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
