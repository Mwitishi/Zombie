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
    ze1.st=0;
    ze1.qst=img->h/h;
    ze1.qfr=(int*)malloc(ze1.qst*sizeof(int));
    for(i1=0;i1<ze1.qst;i1++)
        ze1.qfr[i1]=img->w/w;

    return ze1;
}
