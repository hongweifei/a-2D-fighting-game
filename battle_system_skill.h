
#include "character.h"

#ifndef BATTLE_SYSTEM_SKILL_H_INCLUDE
#define BATTLE_SYSTEM_SKILL_H_INCLUDE

#define WAY_LEFT 1
#define WAY_RIGHT 2

typedef struct 
{
    Object* object;
    SDL_Rect* skill_object_spirit_rect;
    int renderN;
    int characterN;
}Skill_Object;

Skill_Object* skill_object_new(int x,int y,int width,int height,const char* texture_path,SDL_Renderer* renderer,
                                Uint8 r,Uint8 g,Uint8 b,Uint8 a,SDL_Rect* skill_object_spirit_rect)
{
    Skill_Object* skill_object = (Skill_Object*)malloc(sizeof(Skill_Object));

    skill_object->object = object_new(x,y,width,height,texture_path,renderer,r,g,b,a);

    if(skill_object_spirit_rect != NULL)
        skill_object->skill_object_spirit_rect = skill_object_spirit_rect;

    skill_object->renderN = 0;

    return skill_object;
}

void skill_object_render(Skill_Object* skill_object,int way,SDL_Renderer* renderer)
{
    int length = sizeof(skill_object->skill_object_spirit_rect)/sizeof(skill_object->skill_object_spirit_rect[0]);
    int renderN = skill_object->renderN;

    if(skill_object->renderN < length)
        skill_object->renderN++;
    else    
        skill_object->renderN = 0;

    if(renderer == NULL)
    {
        skill_object->object->isRender = 0;
    }
    else
    {
        if(way == WAY_LEFT)
            SDL_RenderCopyEx(renderer,skill_object->object->texture,&skill_object->skill_object_spirit_rect[renderN],
                            &skill_object->object->object_rect,0,NULL,SDL_FLIP_HORIZONTAL);
        else if(way == WAY_RIGHT)
            SDL_RenderCopy(renderer,skill_object->object->texture,&skill_object->skill_object_spirit_rect[renderN],
                            &skill_object->object->object_rect);
            
        skill_object->object->isRender = 1;
    }
    
    
                
    printf("%d\n",skill_object->renderN);
    printf("%d\n",length);
}

int skill_object_collision(Skill_Object* skill_object)
{
    int i;
    int isTrue = 0;

    for (i = 0; i < characterN; i++)
    {
        int character_man_n = character_information[i][CHARACTER_INFORMATION_MAN_N];

        int character_x = man_information[character_man_n][MAN_INFORMATION_X];
        int character_y = man_information[character_man_n][MAN_INFORMATION_Y];
        int character_width = man_information[character_man_n][MAN_INFORMATION_WIDTH];
        int character_height = man_information[character_man_n][MAN_INFORMATION_HEIGHT];

        isTrue = collision(skill_object->object->x,skill_object->object->y,
                    skill_object->object->width,skill_object->object->height
                    ,character_x,character_y,character_width,character_height);

        if(isTrue)
        {
            skill_object->characterN = i;
            return isTrue;
        }
    }
    
}

void skill_object_updata(Skill_Object* skill_object)
{
    object_updata(skill_object->object);
    skill_object->renderN++;

    if(skill_object->object->isRender)
    {
        if(skill_object_collision(skill_object))
        {
            printf("%d is hurt.\n",skill_object->characterN);
            
        }
    }
    
}

#endif //BATTLE_SYSTEM_SKILL_H_INCLUDE
