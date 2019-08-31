
#include "man.h"

#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#define CHARACTER_INFORMATION_N 3
#define CHARACTER_INFORMATION_MAN_N 0
#define CHARACTER_INFORMATION_HP 1
#define CHARACTER_INFORMATION_MP 2

int characterN = 0;
int** character_information;

typedef struct
{
    Man* man;
    char* name;
    int HP;
    int MP;
    SDL_Texture* texture;
}Character;

Character* character_new(char* name,int HP,int MP,int x,int y,int width,int height,
                        const char* texture_path,SDL_Renderer* renderer,Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    if(!characterN)
        character_information = (int**)calloc(1,sizeof(int*));
    else
        character_information = (int**)realloc(character_information,sizeof(int*));

    character_information[characterN] = (int*)calloc(CHARACTER_INFORMATION_N,sizeof(int));

    Character* character = (Character*)calloc(1,sizeof(Character));
    
    character->man = man_new(x,y,width,height);
    
    character->name = name;
    character->HP = HP;
    character->MP = MP;
    
    SDL_Surface* surface= IMG_Load(texture_path);
    SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGBA(surface->format,r,g,b,a));
    character->texture = SDL_CreateTextureFromSurface(renderer,surface);

    SDL_FreeSurface(surface);

    character_information[characterN][CHARACTER_INFORMATION_MAN_N] = character->man->N;
    character_information[characterN][CHARACTER_INFORMATION_HP] = character->HP;
    character_information[characterN][CHARACTER_INFORMATION_MP] = character->MP;
    characterN++;
    
    return character;
}

int Character_attack(Character* character,int way)
{
    int i;
    
    if(character->man->state[MAN_STATE_DIE])
    {
        return 0;
    }
    
    character->man->state[MAN_STATE_WALK] = 0;
    character->man->state[MAN_STATE_HURT] = 0;

    character->man->way = way;

    if(character->man->state[MAN_STATE_RUN])
    {
        character->man->state[MAN_STATE_RUN] = 0;
    }
    
    if(character->man->way == MAN_WAY_LEFT)
    {
        for(i = 0;i < manN;i++)
        {
            if(i != character->man->N)
            {
                if(man_information[i][MAN_INFORMATION_RENDER])
                {
                    if(character->man->y < man_information[i][MAN_INFORMATION_DY] && character->man->dy > man_information[i][MAN_INFORMATION_Y]
                    && character->man->x <= man_information[i][MAN_INFORMATION_DX] && character->man->x >= man_information[i][MAN_INFORMATION_X])
                    {
                        man_information[i][MAN_INFORMATION_STATE_HURT] = 1;
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
    else if(character->man->way == MAN_WAY_RIGHT)
    {
        for(i = 0;i < manN;i++)
        {
            if(i != character->man->N)
            {
                if(man_information[i][MAN_INFORMATION_RENDER])
                {
                    if(character->man->y < man_information[i][MAN_INFORMATION_DY] && character->man->dy > man_information[i][MAN_INFORMATION_Y]
                    && character->man->dx >= man_information[i][MAN_INFORMATION_X] && character->man->dx <= man_information[i][MAN_INFORMATION_DX])
                    {
                        man_information[i][MAN_INFORMATION_STATE_HURT] = 1;
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
}

void character_updata(Character* character)
{
    man_updata(character->man);
}

#endif // CHARACTER_H_INCLUDED
























