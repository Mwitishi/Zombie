#include "zombie.h"

//Function for creating a zent structure with specified values
struct zent zent_make(SDL_Surface *img,float x,float y,int w,int h,int tpf)
{
    struct zent ze1;
    int i1;

    ze1.img=img;
    ze1.x=x;
    ze1.y=y;
    ze1.w=w;
    ze1.h=h;
    ze1.lastbox.x=(int)x;
    ze1.lastbox.y=(int)y;
    ze1.lastbox.w=w;
    ze1.lastbox.h=h;
    ze1.tpf=tpf;
    ze1.st=0;
    ze1.qst=img->h/h;
    ze1.qfr=(int*)malloc(ze1.qst*sizeof(int));
    for(i1=0;i1<ze1.qst;i1++)
        ze1.qfr[i1]=img->w/w;

    return ze1;
}

//Function for drawing an entity on the screen
int zent_draw(struct zent *ze1)
{
    SDL_Rect r1,r2;

    //Null checking
    if(screen==NULL) return 1;
    if(background==NULL) return 1;
    if(ze1==NULL) return 1;
    if(ze1->img==NULL) return 1;

    //Initialize rectangles
    r1.w=r2.w=ze1->w;
    r1.h=r2.h=ze1->h;
    r1.x=(tick/(ze1->tpf))%(ze1->qfr[ze1->st]);
    r1.y=(ze1->st)*(ze1->h);
    r2.x=(int)(ze1->x);
    r2.y=(int)(ze1->y);

    //Empty previous position, draw at new position
    if(SDL_BlitSurface(background,&(ze1->lastbox),screen,&(ze1->lastbox))!=0
     ||SDL_BlitSurface(ze1->img,&r1,screen,&r2)!=0) return 1;

    //Update lastbox, next drawing will erase this one
    ze1->lastbox=r2;

    return 0;
}
