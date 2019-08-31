
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <time.h>

#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#define WAY_LEFT 1
#define WAY_RIGHT 2

#define OBJECT_INFORMATION_N 7
#define OBJECT_INFORMATION_RENDER 0
#define OBJECT_INFORMATION_X 1
#define OBJECT_INFORMATION_Y 2
#define OBJECT_INFORMATION_DX 3
#define OBJECT_INFORMATION_DY 4
#define OBJECT_INFORMATION_WIDTH 5
#define OBJECT_INFORMATION_HEIGHT 6

int objectN = 0;
int** object_information;

typedef struct
{
    int objectN;

    int x,y;
    int dx,dy;
    int width,height;
    SDL_Rect object_rect;
    SDL_Texture* texture;

    int isRender,renderN;
}Object;

typedef struct
{
    SDL_Texture* texture;
}Background;

int collision(int x1,int y1,int width1,int height1,int x2,int y2,int width2,int height2)
{
    if((y1 + height1 > y2 && y1 + height1 < y2 + height2 && x1 + width1 > x2 && x1 < x2 + width2) || //stand
       (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 > y2)) //
    {
        return 1;
    }
    
    if((y1 < y2 + height2 && y1 + height1 > y2 && x1 < x2 + width2 && x1 > x2) || //left
        (y1 < y2 + height2 && y1 + height1 > y2 && x1 + width1 > x2 && x1 + width1 < x2 + width2))//right
    {
        return 1;
    }

    return 0;
}

Object* object_new(int x,int y,int width,int height,const char* texture_path,SDL_Renderer* renderer,
                   Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    if(objectN == 0)
    {
        object_information = (int**)calloc(objectN + 1,sizeof(int*));
    }
    else
    {
        object_information = (int**)realloc(object_information,sizeof(int*) * (objectN + 1));
    }
    object_information[objectN] = (int*)calloc(OBJECT_INFORMATION_N,sizeof(int));

    Object* object = (Object*)calloc(1,sizeof(Object));

    //SDL_RWops* src = SDL_RWFromFile(texture_path,"r");
    SDL_Surface* surface= IMG_Load(texture_path);
    SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGBA(surface->format,r,g,b,a));
    object->texture = SDL_CreateTextureFromSurface(renderer,surface);

    object->objectN = objectN;

    object->x = x;
    object->y = y;
    object->dx = 0;
    object->dy = 0;
    object->width = width;
    object->height = height;
    object->isRender = 0;

    object->object_rect.x = x;
    object->object_rect.y = y;
    object->object_rect.w = width;
    object->object_rect.h = height;

    object_information[objectN][OBJECT_INFORMATION_RENDER] = object->isRender;
    object_information[objectN][OBJECT_INFORMATION_X] = object->x;
    object_information[objectN][OBJECT_INFORMATION_Y] = object->y;
    object_information[objectN][OBJECT_INFORMATION_DX] = object->dx;
    object_information[objectN][OBJECT_INFORMATION_DY] = object->dy;
    object_information[objectN][OBJECT_INFORMATION_WIDTH] = object->width;
    object_information[objectN][OBJECT_INFORMATION_HEIGHT] = object->height;
    objectN++;

    SDL_FreeSurface(surface);

    return object;
}

void object_render(Object* object,int way,SDL_Renderer* renderer)
{
    if(renderer == NULL)
    {
        object->isRender = 0;
    }
    else
    {
        if(way == WAY_LEFT)
            SDL_RenderCopyEx(renderer,object->texture,NULL,&object->object_rect,0,NULL,SDL_FLIP_HORIZONTAL);
        else if(way == WAY_RIGHT)
            SDL_RenderCopy(renderer,object->texture,NULL,&object->object_rect);
        object->isRender = 1;
    }
}

void object_updata(Object* object)
{
    object_information[object->objectN][OBJECT_INFORMATION_RENDER] = object->isRender;
    
    if(!object->isRender)
        return;
    
    object_information[object->objectN][OBJECT_INFORMATION_X] = object->x;
    object_information[object->objectN][OBJECT_INFORMATION_Y] = object->y;
    object_information[object->objectN][OBJECT_INFORMATION_DX] = object->dx;
    object_information[object->objectN][OBJECT_INFORMATION_DY] = object->dy;
    object_information[object->objectN][OBJECT_INFORMATION_WIDTH] = object->width;
    object_information[object->objectN][OBJECT_INFORMATION_HEIGHT] = object->height;
    
    object->x += object->dx;
    object->y += object->dy;
    object->dx = 0;
    object->dy = 0;
    
    object->object_rect.x = object->x;
    object->object_rect.y = object->y;
    object->object_rect.w = object->width;
    object->object_rect.h = object->height;
}

Background* background_new(const char* texture_path,SDL_Renderer* renderer,Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    Background* background = (Background*)calloc(1,sizeof(Background));

    SDL_Surface* surface= IMG_Load(texture_path);
    SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGBA(surface->format,r,g,b,a));
    background->texture = SDL_CreateTextureFromSurface(renderer,surface);

    SDL_FreeSurface(surface);

    return background;
}

#endif // OBJECT_H_INCLUDED
