
#include "man.h"
//#include "battle_system_skill.h"

#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#define CHARACTER_INFORMATION_N 3
#define CHARACTER_INFORMATION_MAN_N 0
#define CHARACTER_INFORMATION_HP 1
#define CHARACTER_INFORMATION_MP 2

#define CHARACTER_STATE_N 3
#define CHARACTER_STATE_ATTACK 0
#define CHARACTER_STATE_ATTACK_SKILL 1
#define CHARACTER_STATE_ATTACK_DISTANCE_SKILL 2

int characterN = 0;
int** character_information;

typedef struct
{
    int N;
    
    Man* man;
    char* name;
    int HP;
    int MP;
    int* state;
    SDL_Texture* texture;
}Character;

typedef struct 
{
    Object* object;
    SDL_Rect* skill_object_spirit_rect;
    int characterN,characterN_change;
    int objectN,objectN_change;
}Skill_Object;

Skill_Object* skill_object_new(int x,int y,int width,int height,const char* texture_path,SDL_Renderer* renderer,
                                Uint8 r,Uint8 g,Uint8 b,Uint8 a,SDL_Rect* skill_object_spirit_rect)
{
    Skill_Object* skill_object = (Skill_Object*)calloc(1,sizeof(Skill_Object));

    skill_object->object = object_new(x,y,width,height,texture_path,renderer,r,g,b,a);

    if(skill_object_spirit_rect != NULL)
        skill_object->skill_object_spirit_rect = skill_object_spirit_rect;

    skill_object->object->renderN = 0;

    return skill_object;
}

void skill_object_render(Skill_Object* skill_object,int way,SDL_Renderer* renderer)
{
    int length = sizeof(skill_object->skill_object_spirit_rect)/sizeof(skill_object->skill_object_spirit_rect[0]);
    int renderN = skill_object->object->renderN;

    if(skill_object->object->renderN < length)
        skill_object->object->renderN++;
    else    
        skill_object->object->renderN = 0;

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
}

int skill_object_collision(Skill_Object* skill_object)
{
    int i;

    if(skill_object->object->isRender)
    {
        for (i = 0; i < characterN; i++)
        {
            int character_man_n = character_information[i][CHARACTER_INFORMATION_MAN_N];

            int character_x = man_information[character_man_n][MAN_INFORMATION_X] + man_information[character_man_n][MAN_INFORMATION_DX];
            int character_y = man_information[character_man_n][MAN_INFORMATION_Y] + man_information[character_man_n][MAN_INFORMATION_DY];
            int character_width = man_information[character_man_n][MAN_INFORMATION_WIDTH];
            int character_height = man_information[character_man_n][MAN_INFORMATION_HEIGHT];
            
            if(collision(skill_object->object->x,skill_object->object->y,skill_object->object->width,skill_object->object->height,
                character_x,character_y,character_width,character_height))
            {
                skill_object->characterN = i;
                skill_object->characterN_change = 1;
                return 1;
            }
            
            /*
            if(skill_object->object->y < character_y + character_height && 
                skill_object->object->y + skill_object->object->height > character_y &&
                skill_object->object->x + skill_object->object->width >= character_x &&
                skill_object->object->x + skill_object->object->width < character_x + character_width)
            {
                printf("right\n");
                skill_object->characterN = i;
                return 1;
            }
            
            if(skill_object->object->y < character_y + character_height && 
                skill_object->object->y + skill_object->object->height > character_y &&
                skill_object->object->x <= character_x + character_width &&
                skill_object->object->x > character_x )
            {
                printf("left\n");
                skill_object->characterN = i;
                return 1;
            }
            */
        }
        
        for (i = 0; i < objectN; i++)
        {
            int object_x = object_information[i][OBJECT_INFORMATION_X] + object_information[i][OBJECT_INFORMATION_DX];
            int object_y = object_information[i][OBJECT_INFORMATION_Y] + object_information[i][OBJECT_INFORMATION_DY];
            int object_width = object_information[i][OBJECT_INFORMATION_WIDTH];
            int object_height = object_information[i][OBJECT_INFORMATION_HEIGHT];
            
            if(collision(skill_object->object->x,skill_object->object->y,skill_object->object->width,skill_object->object->height,
                object_x,object_y,object_width,object_height))
            {
                skill_object->objectN = i;
                skill_object->objectN_change = 1;
                return 1;
            }
        }
    }
    
    return 0;
}

void skill_object_updata(Skill_Object* skill_object)
{
    object_updata(skill_object->object);
    
    if(skill_object->object->isRender)
    {
        skill_object->object->renderN++;
    }
}

Character* character_new(char* name,int HP,int MP,int x,int y,int width,int height,
                        const char* texture_path,SDL_Renderer* renderer,Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    if(!characterN)
        character_information = (int**)calloc(1,sizeof(int*));
    else
        character_information = (int**)realloc(character_information,sizeof(int*));

    character_information[characterN] = (int*)calloc(CHARACTER_INFORMATION_N,sizeof(int));

    Character* character = (Character*)calloc(1,sizeof(Character));
    character->state = (int*)malloc(sizeof(int) * CHARACTER_STATE_N);
    
    character->N = characterN;
    
    character->man = man_new(x,y,width,height);
    
    character->name = name;
    character->HP = HP;
    character->MP = MP;
    character->state[CHARACTER_STATE_ATTACK] = 0;
    character->state[CHARACTER_STATE_ATTACK_SKILL] = 0;
    character->state[CHARACTER_STATE_ATTACK_DISTANCE_SKILL] = 0;
    
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
    }
    
    return 0;
}

int Character_attack_skill(Character* character,int way)
{
    character->man->way = way;
    
    //collision(character->man->x,character->man->y,character->man->width,character->man->height,
    //         int x2,int y2,int width2,int height2)
}

int Character_attack_distance_skill(Character* character,int way,int hurt,int speed,Skill_Object* skill,SDL_Renderer* renderer)
{
    if(skill == NULL)
        return 0;

    if(skill_object_collision(skill))
    {
        if(skill->characterN_change)
        {
            int hurt_man_N = character_information[skill->characterN][CHARACTER_INFORMATION_MAN_N];
            
            character_information[skill->characterN][CHARACTER_INFORMATION_HP] -= hurt;
            man_information[hurt_man_N][MAN_INFORMATION_STATE_HURT] = 1;
            character->state[CHARACTER_STATE_ATTACK_DISTANCE_SKILL] = 0;
            skill->characterN_change = 0;
            skill->object->isRender = 0;
        }
        if(skill->objectN_change)
        {
            character->state[CHARACTER_STATE_ATTACK_DISTANCE_SKILL] = 0;
            skill->objectN_change = 0;
            skill->object->isRender = 0;
        }
        return 1;
    }
    else
    {
        skill_object_render(skill,way,renderer);
        
        if(way == WAY_LEFT)
        {
            skill->object->dx = -speed;
            /*
            if(skill->object->x <= )
            {
                character->state[CHARACTER_STATE_ATTACK_DISTANCE_SKILL] = 0;
                skill->object->isRender = 0;
                return 1;
            }
            */
        }
        
        else if(way == WAY_RIGHT)
        {
            skill->object->dx = speed;
            /*
            if(skill->object->x >= )
            {
                character->state[CHARACTER_STATE_ATTACK_DISTANCE_SKILL] = 0;
                skill->object->isRender = 0;
                return 1;
            }
            */
        }
        
        skill_object_updata(skill);
        
        /*
        if(skill->object->y <= end_y)
        {
            character->state[CHARACTER_STATE_ATTACK_DISTANCE_SKILL] = 0;
            skill->object->isRender = 0;
            return 1;
        }
        */
    }
    
    return 0;
}

void character_updata(Character* character)
{
    man_updata(character->man);
    character->HP = character_information[character->N][CHARACTER_INFORMATION_HP];
    character->MP = character_information[character->N][CHARACTER_INFORMATION_MP];
    
    if(character->HP <= 0)
        man_die(character->man);
}

#endif // CHARACTER_H_INCLUDED
























