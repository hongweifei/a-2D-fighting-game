
#include "man.h"
//#include "battle_system_skill.h"

#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#define CHARACTER_INFORMATION_N 4
#define CHARACTER_INFORMATION_MAN_N 0
#define CHARACTER_INFORMATION_HP 1
#define CHARACTER_INFORMATION_MP 2
#define CHARACTER_INFORMATION_STATE_ATTACK_SKILL_DISTANCE 3

#define CHARACTER_STATE_N 3
#define CHARACTER_STATE_ATTACK 0
#define CHARACTER_STATE_ATTACK_SKILL 1
#define CHARACTER_STATE_ATTACK_SKILL_DISTANCE 2

#define CHARACTER_SKILL_ONE 0
#define CHARACTER_SKILL_TWO 1
#define CHARACTER_SKILL_THREE 2
#define CHARACTER_SKILL_FOUR 3
#define CHARACTER_SKILL_FIVE 4

int characterN = 0;
int** character_information;

typedef struct 
{
    Object* object;
    SDL_Rect* skill_object_spirit_rect;
    
    int way;
    int speed;
    int ATK;
    int userN;
    
    int characterN,characterN_change;
    int objectN,objectN_change;
}Skill_Object;

typedef struct
{
    int N;
    
    Man* man;
    char* name;
    int HP;
    int MP;
    int ATK,DEF;
    int* state;
    SDL_Texture* texture;
    Skill_Object** skill;
    int* skill_speed;
}Character;

Skill_Object* skill_object_new(int x,int y,int width,int height,int speed,int ATK,const char* texture_path,SDL_Renderer* renderer,
                                Uint8 r,Uint8 g,Uint8 b,Uint8 a,SDL_Rect* skill_object_spirit_rect)
{
    Skill_Object* skill_object = (Skill_Object*)calloc(1,sizeof(Skill_Object));

    skill_object->object = object_new(x,y,width,height,texture_path,renderer,r,g,b,a);

    if(skill_object_spirit_rect != NULL)
        skill_object->skill_object_spirit_rect = skill_object_spirit_rect;

    skill_object->way = WAY_RIGHT;
    skill_object->speed = speed;
    skill_object->ATK = ATK;
    skill_object->userN = 9999;
    
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
    if(!skill_object->object->isRender)
        return;
    
    if(skill_object_collision(skill_object))
    {
        if(skill_object->characterN_change)
        {
            int hurt_man_N = character_information[skill_object->characterN][CHARACTER_INFORMATION_MAN_N];
            
            character_information[skill_object->characterN][CHARACTER_INFORMATION_HP] -= skill_object->ATK;
            man_information[hurt_man_N][MAN_INFORMATION_STATE_HURT] = 1;
            skill_object->characterN_change = 0;
            skill_object->object->isRender = 0;
        }
        if(skill_object->objectN_change)
        {
            skill_object->objectN_change = 0;
            skill_object->object->isRender = 0;
        }
        
        character_information[skill_object->userN][CHARACTER_INFORMATION_STATE_ATTACK_SKILL_DISTANCE] = 0;
    }
    else
    {
        if(skill_object->way == WAY_LEFT)
        {
            skill_object->object->dx = -skill_object->speed;
        }
        else if(skill_object->way == WAY_RIGHT)
        {
            skill_object->object->dx = skill_object->speed;
        }
    }
    
    object_updata(skill_object->object);
}

Character* character_new(char* name,int HP,int MP,int ATK,int DEF,int x,int y,int width,int height,
                        const char* texture_path,SDL_Renderer* renderer,Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    if(!characterN)
        character_information = (int**)calloc(characterN + 1,sizeof(int*));
    else
        character_information = (int**)realloc(character_information,sizeof(int*));

    character_information[characterN] = (int*)calloc(CHARACTER_INFORMATION_N,sizeof(int));

    Character* character = (Character*)calloc(1,sizeof(Character));
    character->state = (int*)calloc(CHARACTER_STATE_N,sizeof(int));
    
    character->N = characterN;
    
    character->man = man_new(x,y,width,height);
    
    character->name = name;
    character->HP = HP;
    character->MP = MP;
    character->ATK = ATK;
    character->DEF = DEF;
    character->state[CHARACTER_STATE_ATTACK] = 0;
    character->state[CHARACTER_STATE_ATTACK_SKILL] = 0;
    character->state[CHARACTER_STATE_ATTACK_SKILL_DISTANCE] = 0;
    
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

int character_attack(Character* character,int way,int hurt)
{
    if(character->man->state[MAN_STATE_DIE])
        return 0;
    if(character->man->state[MAN_STATE_HURT])
        return 0;
    
    int i,j;
    
    character->man->state[MAN_STATE_WALK] = 0;
    character->man->state[MAN_STATE_RUN] = 0;
    character->man->state[MAN_STATE_HURT] = 0;
    
    character->state[CHARACTER_STATE_ATTACK] = 1;

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
                    int man_x = man_information[i][MAN_INFORMATION_X] + man_information[i][MAN_INFORMATION_DX];
                    int man_y = man_information[i][MAN_INFORMATION_Y] + man_information[i][MAN_INFORMATION_DY];
                    int man_width = man_information[i][MAN_INFORMATION_WIDTH];
                    int man_height = man_information[i][MAN_INFORMATION_HEIGHT];
                    
                    if(character->man->y < man_y + man_height && character->man->y + character->man->height > man_y
                    && character->man->x <= man_x + man_width && character->man->x >= man_x)
                    {
                        man_information[i][MAN_INFORMATION_STATE_HURT] = 1;
                        for(j = 0;j < characterN;j++)
                        {
                            if(man_information[i][MAN_INFORMATION_MAN_N] == character_information[j][CHARACTER_INFORMATION_MAN_N])
                            {
                                character_information[j][CHARACTER_INFORMATION_HP] -= hurt;
                            }
                        }
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
                    int man_x = man_information[i][MAN_INFORMATION_X] + man_information[i][MAN_INFORMATION_DX];
                    int man_y = man_information[i][MAN_INFORMATION_Y] + man_information[i][MAN_INFORMATION_DY];
                    int man_width = man_information[i][MAN_INFORMATION_WIDTH];
                    int man_height = man_information[i][MAN_INFORMATION_HEIGHT];
                    
                    if(character->man->y < man_y + man_height && character->man->y + character->man->height> man_y
                    && character->man->x + character->man->width >= man_x && character->man->x + character->man->width <= man_x + man_width)
                    {
                        man_information[i][MAN_INFORMATION_STATE_HURT] = 1;
                        for(j = 0;j < characterN;j++)
                        {
                            if(man_information[i][MAN_INFORMATION_MAN_N] == character_information[j][CHARACTER_INFORMATION_MAN_N])
                            {
                                character_information[j][CHARACTER_INFORMATION_HP] -= hurt;
                            }
                        }
                        return 1;
                    }
                }
            }
        }
    }
    
    return 0;
}

int character_attack_skill(Character* character,int way)
{
    character->state[CHARACTER_STATE_ATTACK_SKILL] = 1;
    
    character->man->way = way;
    
    //collision(character->man->x,character->man->y,character->man->width,character->man->height,
    //         int x2,int y2,int width2,int height2)
}

int character_attack_skill_distance(Character* character,Skill_Object* skill,int way)
{
    if(character->man->state[MAN_STATE_DIE])
        return 0;
    if(character->man->state[MAN_STATE_HURT])
        return 0;
    
    if(skill == NULL)
        return 0;
    
    skill->way = way;

    character_information[character->N][CHARACTER_INFORMATION_STATE_ATTACK_SKILL_DISTANCE] = 1;
    skill->userN = character->N;
    
    return 0;
}

void character_render(Character* character,int way,SDL_Renderer* renderer,SDL_Texture* texture,SDL_Rect* spirit_rect,int begin_n,int end_n)
{
    man_render(character->man,way,renderer,texture,spirit_rect,begin_n,end_n);
}

void character_updata(Character* character,SDL_Renderer* renderer)
{
    man_updata(character->man);
    character->HP = character_information[character->N][CHARACTER_INFORMATION_HP];
    character->MP = character_information[character->N][CHARACTER_INFORMATION_MP];
    
    character->state[CHARACTER_STATE_ATTACK_SKILL_DISTANCE] = 
    character_information[character->N][CHARACTER_INFORMATION_STATE_ATTACK_SKILL_DISTANCE];
    
    if(character->HP <= 0)
        man_die(character->man);
    else if(character->state[CHARACTER_STATE_ATTACK])
        character_attack(character,character->man->way,character->ATK);
}

#endif // CHARACTER_H_INCLUDED
























