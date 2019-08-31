
#include "object.h"

#ifndef MAN_H_INCLUDED
#define MAN_H_INCLUDED

#define MAN_STATE_N 7
#define MAN_STATE_STAND 0
#define MAN_STATE_JUMP 1
#define MAN_STATE_DROP 2
#define MAN_STATE_WALK 3
#define MAN_STATE_RUN 4
#define MAN_STATE_HURT 5
#define MAN_STATE_DIE 6

#define MAN_WAY_LEFT 1
#define MAN_WAY_RIGHT 2

#define MAN_JUMP_SPEED 10
#define MAN_JUMP_N 30

#define MAN_WALK_SPEED 1
#define MAN_RUN_SPEED 2

#define MAN_INFORMATION_N 8
#define MAN_INFORMATION_X 0
#define MAN_INFORMATION_Y 1
#define MAN_INFORMATION_DX 2
#define MAN_INFORMATION_DY 3
#define MAN_INFORMATION_WIDTH 4
#define MAN_INFORMATION_HEIGHT 5
#define MAN_INFORMATION_STATE_HURT 6
#define MAN_INFORMATION_RENDER 7

int manN = 0;
int** man_information;

typedef struct
{
    int N;
    int isRender,renderN;
    
    float x,y;
    float dx,dy;
    int width,height;
    int *state;
    int way;
    int jumpN,jumpM;
    int dropN;
    SDL_Rect man_rect;
    SDL_Rect* spirit_rect;
}Man;

void man_jump(Man *man,int way);
void man_run(Man* man,int way);

Man* man_new(int x,int y,int width,int height)
{
    if(manN == 0)
        man_information = (int**)malloc(sizeof(int*) * (manN + 1));
    else
        man_information = (int**)realloc(man_information,sizeof(int*) * (manN + 1));
    
    man_information[manN] = (int*)malloc(sizeof(int) * MAN_INFORMATION_N);
    
    Man* man = (Man*)calloc(1,sizeof(Man));
    man->state = (int*)calloc(MAN_STATE_N,sizeof(int));

    man->N = manN;
    man->isRender = 0;
    
    man->x = x;
    man->y = y;
    man->dx = 0;
    man->dy = 0;
    man->width = width;
    man->height = height;
    man->state[MAN_STATE_STAND] = MAN_WAY_RIGHT;
    man->state[MAN_STATE_JUMP] = 0;
    man->state[MAN_STATE_DROP] = 1;
    man->state[MAN_STATE_RUN] = 0;
    man->state[MAN_STATE_HURT] = 0;
    man->state[MAN_STATE_DIE] = 0;
    man->way = MAN_WAY_RIGHT;
    man->jumpN = 0;
    man->jumpM = MAN_JUMP_N;
    man->dropN = 1;
    man->man_rect.x = x;
    man->man_rect.y = y;
    man->man_rect.w = width;
    man->man_rect.h = height;
    
    man_information[manN][MAN_INFORMATION_X] = man->x;
    man_information[manN][MAN_INFORMATION_Y] = man->y;
    man_information[manN][MAN_INFORMATION_DX] = man->dx;
    man_information[manN][MAN_INFORMATION_DY] = man->dy;
    man_information[manN][MAN_INFORMATION_WIDTH] = man->width;
    man_information[manN][MAN_INFORMATION_HEIGHT] = man->height;
    man_information[manN][MAN_INFORMATION_STATE_HURT] = man->state[MAN_STATE_HURT];
    man_information[manN][MAN_INFORMATION_RENDER] = man->isRender;
    manN++;

    return man;
}

void man_stand(Man* man,int way)
{
    int i;
    
    if(man->state[MAN_STATE_DIE])
    {
        return;
    }
    
    man->state[MAN_STATE_HURT] = 0;
    man->state[MAN_STATE_STAND] = 1;
    man->state[MAN_STATE_RUN] = 0;

    man->way = way;

    for(i = 0;i < objectN;i++)
    {
        if(object_information[i][OBJECT_INFORMATION_RENDER])
        {
            int object_x = object_information[i][OBJECT_INFORMATION_X];
            int object_y = object_information[i][OBJECT_INFORMATION_Y];
            int object_dx = object_information[i][OBJECT_INFORMATION_DX];
            int object_dy = object_information[i][OBJECT_INFORMATION_DY];
            int object_width = object_information[i][OBJECT_INFORMATION_WIDTH];
            int object_height = object_information[i][OBJECT_INFORMATION_HEIGHT];
            if(man->y + man->height >= object_y + object_dy && man->y + man->height <= object_y + object_dy + object_height)
            {
                if(man->x + man->width < object_x + object_dx || man->x > object_x + object_dx + object_width)
                {
                    man->state[MAN_STATE_STAND] = 0;
                    man->state[MAN_STATE_DROP] = 1;
                    return;
                }
            }
        }
    }

    return;
}

void man_drop(Man* man,int way)
{
    int i;

    man->state[MAN_STATE_JUMP] = 0;
    man->state[MAN_STATE_DROP] = 1;

    man->way = way;

    for(i = 0;i < objectN;i++)
    {
        if(object_information[i][OBJECT_INFORMATION_RENDER])
        {
            int object_x = object_information[i][OBJECT_INFORMATION_X];
            int object_y = object_information[i][OBJECT_INFORMATION_Y];
            int object_dx = object_information[i][OBJECT_INFORMATION_DX];
            int object_dy = object_information[i][OBJECT_INFORMATION_DY];
            int object_width = object_information[i][OBJECT_INFORMATION_WIDTH];
            int object_height = object_information[i][OBJECT_INFORMATION_HEIGHT];
            
            if(man->y + man->height >= object_y + object_dy && man->y + man->height <= object_y + object_dy + object_height)
            {
                /*
                if(man->y >= object_y + object_dy + object_height && man->y <= object_y + object_dy)
                {
                    man->dy = 1;
                }
                */

                if(man->x + man->width >= object_x + object_dx && man->x <= object_x + object_dx + object_width)
                {
                    man->dy = -(man->y - (object_y + object_dy - man->height));
                    
                    man->state[MAN_STATE_DROP] = 0;
                    man->dropN = 1;
                    
                    man->state[MAN_STATE_JUMP] = 0;
                    man->jumpN = 0;
                    man->jumpM = MAN_JUMP_N;
                    
                    return;
                }
            }
        }
    }

    man->dy = 9.8 * man->dropN / 30;
    man->dropN++;

    //if(man->state[MAN_STATE_RUN])
    //    man_run(man,man->way);

    return;
}

void man_walk(Man* man,int way)
{
    int i;
    
    if(man->state[MAN_STATE_DIE])
    {
        return;
    }

    man->state[MAN_STATE_HURT] = 0;
    man->state[MAN_STATE_STAND] = 0;
    man->state[MAN_STATE_RUN] = 0;
    man->state[MAN_STATE_DROP] = 1;
    man->state[MAN_STATE_WALK] = 1;

    man->way = way;

    if(man->way == MAN_WAY_LEFT)
    {
        for(i = 0;i < objectN;i++)
        {
            if(object_information[i][OBJECT_INFORMATION_RENDER])
            {
                int object_x = object_information[i][OBJECT_INFORMATION_X];
                int object_y = object_information[i][OBJECT_INFORMATION_Y];
                int object_dx = object_information[i][OBJECT_INFORMATION_DX];
                int object_dy = object_information[i][OBJECT_INFORMATION_DY];
                int object_width = object_information[i][OBJECT_INFORMATION_WIDTH];
                int object_height = object_information[i][OBJECT_INFORMATION_HEIGHT];
                
                if(man->y < object_y + object_dy + object_height && man->y + man->height > object_y + object_dy
                && man->x <= object_x + object_dx + object_width && man->x >= object_x + object_dx)
                {
                    man->state[MAN_STATE_WALK] = 0;
                    return;
                }
            }
        }

        man->dx = -(5 * MAN_WALK_SPEED);
        return;
    }
    else if(man->way == MAN_WAY_RIGHT)
    {
        for(i = 0;i < objectN;i++)
        {
            if(object_information[i][OBJECT_INFORMATION_RENDER])
            {
                int object_x = object_information[i][OBJECT_INFORMATION_X];
                int object_y = object_information[i][OBJECT_INFORMATION_Y];
                int object_dx = object_information[i][OBJECT_INFORMATION_DX];
                int object_dy = object_information[i][OBJECT_INFORMATION_DY];
                int object_width = object_information[i][OBJECT_INFORMATION_WIDTH];
                int object_height = object_information[i][OBJECT_INFORMATION_HEIGHT];
                
                if(man->y < object_y + object_dy + object_height && man->y + man->height > object_y + object_dy
                && man->x + man->width >= object_x + object_dx && man->x + man->width <= object_x + object_dx + object_width)
                {
                    man->state[MAN_STATE_WALK] = 0;
                    return;
                }
            }
        }

        man->dx = 5 * MAN_WALK_SPEED;
        return;
    }
}

void man_run(Man* man,int way)
{
    int i;
    
    if(man->state[MAN_STATE_DIE])
    {
        return;
    }

    man->state[MAN_STATE_HURT] = 0;
    man->state[MAN_STATE_STAND] = 0;
    man->state[MAN_STATE_DROP] = 1;
    man->state[MAN_STATE_RUN] = 1;

    man->way = way;

    if(man->way == MAN_WAY_LEFT)
    {
        for(i = 0;i < objectN;i++)
        {
            if(object_information[i][OBJECT_INFORMATION_RENDER])
            {
                int object_x = object_information[i][OBJECT_INFORMATION_X];
                int object_y = object_information[i][OBJECT_INFORMATION_Y];
                int object_dx = object_information[i][OBJECT_INFORMATION_DX];
                int object_dy = object_information[i][OBJECT_INFORMATION_DY];
                int object_width = object_information[i][OBJECT_INFORMATION_WIDTH];
                int object_height = object_information[i][OBJECT_INFORMATION_HEIGHT];
                
                if(man->y < object_y + object_dy + object_height && man->y + man->height > object_y + object_dy
                && man->x <= object_x + object_dx + object_width && man->x > object_x + object_dx)
                {
                    printf("run left collision\n");
                    return;
                }
            }
        }

        man->dx = -(5 * MAN_RUN_SPEED);
        return;
    }

    if(man->way == MAN_WAY_RIGHT)
    {
        for(i = 0;i < objectN;i++)
        {
            if(object_information[i][OBJECT_INFORMATION_RENDER])
            {
                int object_x = object_information[i][OBJECT_INFORMATION_X];
                int object_y = object_information[i][OBJECT_INFORMATION_Y];
                int object_dx = object_information[i][OBJECT_INFORMATION_DX];
                int object_dy = object_information[i][OBJECT_INFORMATION_DY];
                int object_width = object_information[i][OBJECT_INFORMATION_WIDTH];
                int object_height = object_information[i][OBJECT_INFORMATION_HEIGHT];
                
                if(man->y < object_y + object_dy + object_height && man->y + man->height > object_y + object_dy
                && man->x + man->width >= object_x + object_dx && man->x + man->width < object_x + object_dx + object_width)
                {
                    printf("run right collision\n");
                    return;
                }
            }
        }

        man->dx = 5 * MAN_RUN_SPEED;
        return;
    }
}

void man_jump(Man *man,int way)
{
    int i;
    
    if(man->state[MAN_STATE_DIE])
    {
        return;
    }

    man->state[MAN_STATE_HURT] = 0;
    man->state[MAN_STATE_STAND] = 0;
    man->state[MAN_STATE_JUMP] = 1;

    man->way = way;

    if(man->jumpN < man->jumpM)
    {
        man->state[MAN_STATE_DROP] = 0;

        for(i = 0;i < objectN;i++)
        {
            if(object_information[i][OBJECT_INFORMATION_RENDER])
            {
                int object_x = object_information[i][OBJECT_INFORMATION_X];
                int object_y = object_information[i][OBJECT_INFORMATION_Y];
                int object_dx = object_information[i][OBJECT_INFORMATION_DX];
                int object_dy = object_information[i][OBJECT_INFORMATION_DY];
                int object_width = object_information[i][OBJECT_INFORMATION_WIDTH];
                int object_height = object_information[i][OBJECT_INFORMATION_HEIGHT];
                
                if(man->y >= object_y + object_dy + object_height && man->y <= object_y + object_dy
                && man->x + man->width >= object_x + object_dx && man->x <= object_x + object_dx + object_width)
                {
                    man->dy = - 1;
                    man->state[MAN_STATE_JUMP] = 0;
                    man->state[MAN_STATE_DROP] = 1;
                    man->dropN = 1;
                    man_drop(man,man->way);
                    return;
                }
            }
        }

        man->dy = -(MAN_JUMP_SPEED * man->jumpM / 30);
        man->jumpM--;
    }
    else
    {
        man->state[MAN_STATE_JUMP] = 0;
        man->state[MAN_STATE_DROP] = 1;
        man_drop(man,man->way);
    }

    return;
}

void man_hurt(Man* man)
{
    if(man->state[MAN_STATE_DIE])
    {
        return;
    }
    
    man->state[MAN_STATE_STAND] = 0;
    man->state[MAN_STATE_JUMP] = 0;
    man->state[MAN_STATE_DROP] = 1;
    man->state[MAN_STATE_WALK] = 0;
    man->state[MAN_STATE_RUN] = 0;
    man->state[MAN_STATE_HURT] = 1;
    
    man_information[man->N][MAN_INFORMATION_STATE_HURT] = 0;
}

void man_die(Man* man)
{
    int i;
    
    for(i = 0; i < MAN_STATE_N - 1; i++)
    {
        man->state[i] = 0;
    }
    
    man->state[MAN_STATE_DROP] = 1;
    man->state[MAN_STATE_DIE] = 1;
}

void man_render(Man* man,int way,SDL_Renderer* renderer,SDL_Texture* texture,SDL_Rect* spirit_rect,int begin_n,int end_n)
{
    if(renderer == NULL)
    {
        man->isRender = 0;
    }
    else
    {
        man->way = way;
        man->spirit_rect = spirit_rect;
        
        if(man->renderN < end_n)
            man->renderN++;
        else
            man->renderN = begin_n;
        
        if(way == WAY_LEFT)
            SDL_RenderCopyEx(renderer,texture,&man->spirit_rect[man->renderN],&man->man_rect,0,NULL,SDL_FLIP_HORIZONTAL);
        else if(way == WAY_RIGHT)
            SDL_RenderCopy(renderer,texture,&man->spirit_rect[man->renderN],&man->man_rect);
        man->isRender = 1;
    }
}

void man_updata(Man* man)
{
    if(!man->isRender)
        return;
    
    if(man_information[man->N][MAN_INFORMATION_STATE_HURT])
    {
        man_hurt(man);
    }
    
    else if(man->state[MAN_STATE_STAND])
    {
        man_stand(man,man->way);
        man_drop(man,man->way);
    }
    else if(man->state[MAN_STATE_JUMP])
    {
        man_jump(man,man->way);
    }
    else if(man->state[MAN_STATE_DROP])
    {
        man_drop(man,man->way);
    }
    else if(man->state[MAN_STATE_WALK])
    {
        man_walk(man,man->way);
        man_drop(man,man->way);
    }
    else if(man->state[MAN_STATE_RUN])
    {
        man_run(man,man->way);
        man_drop(man,man->way);
    }
    
    man_information[man->N][MAN_INFORMATION_X] = man->x;
    man_information[man->N][MAN_INFORMATION_Y] = man->y;
    man_information[man->N][MAN_INFORMATION_DX] = man->dx;
    man_information[man->N][MAN_INFORMATION_DY] = man->dy;
    man_information[man->N][MAN_INFORMATION_WIDTH] = man->width;
    man_information[man->N][MAN_INFORMATION_HEIGHT] = man->height;
    man_information[man->N][MAN_INFORMATION_RENDER] = man->isRender;

    man->x += man->dx;
    man->y += man->dy;
    man->dx = 0;
    man->dy = 0;

    man->man_rect.x = man->x;
    man->man_rect.y = man->y;
    man->man_rect.w = man->width;
    man->man_rect.h = man->height;
}

SDL_Rect* init_spirit_rect(int width,int height,int begin_x,int begin_y,unsigned int n,unsigned int m)
{
    int a = 0;
    int i,j;

    if(m == 0){m = 1;}

    SDL_Rect* spirit_rect = (SDL_Rect*)calloc(n * m + 1,sizeof(SDL_Rect));

    if(n == 0)
    {
        spirit_rect[0].x = 0;
        spirit_rect[0].y = 0;
        spirit_rect[0].w = width;
        spirit_rect[0].h = height;
        return spirit_rect;
    }

    for(i = 0;i < m;i++)
    {
        for(j = 0;j < n;j++)
        {
            spirit_rect[a].x = width * j + begin_x;
            spirit_rect[a].y = height * i + begin_y;
            spirit_rect[a].w = width;
            spirit_rect[a].h = height;
            a++;
        }
    }

    return spirit_rect;
}

#endif // MAN_H_INCLUDED












