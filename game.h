
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "character.h"
#include "battle_system_skill.h"

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

static SDL_Renderer *renderer = NULL;
static SDL_Surface *surface = NULL;
static SDL_Texture *texture = NULL;
static SDL_Event event;
static int quit = 0;

const Uint32 FPS = 1000 / 24;//限制的帧速
Uint32 FPS_Timer;

#define man_stand_width 80
#define man_stand_height 80
#define man_run_width 235/2
#define man_run_height 175/2

Character* character;
Character* character1;

Object* object;
Object* object1;
Object* object2;
Object* object3;

Skill_Object* skill;
int isSkill = 0;

Background* background;

int n = 0,n1 = 0;

void init_spirit(Man* man);
void game();
void render();
void quit_game();

void stand(Man* man,int* i);
void walk(Man* man,int* i);
void run(Man* man,int* i);
void hurt(int* i);

void init_game()
{
    #ifdef _WIN32
    printf("window\n");
    #endif

    #ifdef __linux
    printf("linux\n");
    #endif
    
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    //surface = IMG_ReadXPMFromArray(icon_xpm);
    
    character = character_new("kakashi",100,100,100,500,man_stand_width,man_stand_height,
                                "data/kakashi.png",renderer,0,0,0,0);
    character1 = character_new("kakashi",100,100,600,500,man_stand_width,man_stand_height,
                                "data/kakashi.png",renderer,0,0,0,0);
    character->man->spirit_rect = init_spirit_rect(man_stand_width,man_stand_height,0,0,10,7);
    character1->man->spirit_rect = init_spirit_rect(man_stand_width,man_stand_height,0,0,10,7);
    character1->man->way = MAN_WAY_LEFT;
    //init_spirit(man);
    //init_spirit(man1);

    object = object_new(-1000,580,2000,50,"data/pane.png",renderer,255,255,255,255);
    
    object1 = object_new(500,400,100,50,"data/pane.png",renderer,255,255,255,255);
    object2 = object_new(300,500,200,50,"data/pane.png",renderer,255,255,255,255);
    object3 = object_new(100,550,200,50,"data/pane.png",renderer,255,255,255,255);
    
    skill = skill_object_new(character->man->x,character->man->y,character->man->width,character->man->height
    ,"data/skill.bmp",renderer,0,0,0,0,init_spirit_rect(328/4,249/3,0,0,4,1));

    background = background_new("data/background.jpg",renderer,0,0,0,0);
}

void init_spirit(Man* man)
{
    man->spirit_rect = init_spirit_rect(25,52,5,10,17,1);

    //stand
    man->spirit_rect[0].x = 0;
    man->spirit_rect[0].y = 1054;
    man->spirit_rect[0].w = 104;
    man->spirit_rect[0].h = 275;

    man->spirit_rect[1].x = 104;
    man->spirit_rect[1].y = 1054;
    man->spirit_rect[1].w = 104;
    man->spirit_rect[1].h = 275;

    man->spirit_rect[2].x = 207;
    man->spirit_rect[2].y = 1054;
    man->spirit_rect[2].w = 104;
    man->spirit_rect[2].h = 275;

    //run
    man->spirit_rect[3].x = 223;
    man->spirit_rect[3].y = 1695;
    man->spirit_rect[3].w = 234;
    man->spirit_rect[3].h = 173;

    man->spirit_rect[4].x = 227;
    man->spirit_rect[4].y = 1877;
    man->spirit_rect[4].w = 234;
    man->spirit_rect[4].h = 171;

    man->spirit_rect[5].x = 0;
    man->spirit_rect[5].y = 1330;
    man->spirit_rect[5].w = 227;
    man->spirit_rect[5].h = 180;

    man->spirit_rect[6].x = 0;
    man->spirit_rect[6].y = 1869;
    man->spirit_rect[6].w = 226;
    man->spirit_rect[6].h = 180;

    man->spirit_rect[7].x = 0;
    man->spirit_rect[7].y = 1695;
    man->spirit_rect[7].w = 221;
    man->spirit_rect[7].h = 172;

    man->spirit_rect[8].x = 0;
    man->spirit_rect[8].y = 1512;
    man->spirit_rect[8].w = 236;
    man->spirit_rect[8].h = 180;

    //
    man->spirit_rect[9].x = 318;
    man->spirit_rect[9].y = 1210;
    man->spirit_rect[9].w = 190;
    man->spirit_rect[9].h = 210;

    man->spirit_rect[10].x = 340;
    man->spirit_rect[10].y = 1488;
    man->spirit_rect[10].w = 252;
    man->spirit_rect[10].h = 206;

    man->spirit_rect[11].x = 507;
    man->spirit_rect[11].y = 1089;
    man->spirit_rect[11].w = 244;
    man->spirit_rect[11].h = 192;

    man->spirit_rect[12].x = 751;
    man->spirit_rect[12].y = 1102;
    man->spirit_rect[12].w = 307;
    man->spirit_rect[12].h = 180;

    man->spirit_rect[13].x = 759;
    man->spirit_rect[13].y = 1471;
    man->spirit_rect[13].w = 258;
    man->spirit_rect[13].h = 194;

    man->spirit_rect[14].x = 596;
    man->spirit_rect[14].y = 1664;
    man->spirit_rect[14].w = 269;
    man->spirit_rect[14].h = 179;

    man->spirit_rect[15].x = 1020;
    man->spirit_rect[15].y = 1476;
    man->spirit_rect[15].w = 238;
    man->spirit_rect[15].h = 199;

    man->spirit_rect[16].x = 1109;
    man->spirit_rect[16].y = 1803;
    man->spirit_rect[16].w = 172;
    man->spirit_rect[16].h = 244;
}

void start_game()
{
    while (!quit)
    {
        game();

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if(state[SDL_SCANCODE_W])
            man_jump(character->man,character->man->way);
        if(state[SDL_SCANCODE_A])
        {
            if(!state[SDL_SCANCODE_D])
            {
                man_run(character->man,MAN_WAY_LEFT);
                run(character->man,&n);
            }
            else
                man_stand(character->man,character->man->way);
        }
        if(state[SDL_SCANCODE_D])
        {
            if(!state[SDL_SCANCODE_A])
            {
                man_run(character->man,MAN_WAY_RIGHT);
                run(character->man,&n);
            }
            else
                man_stand(character->man,character->man->way);
        }

        if(state[SDL_SCANCODE_UP])
            man_jump(character1->man,character1->man->way);
        if(state[SDL_SCANCODE_LEFT])
        {
            if(!state[SDL_SCANCODE_RIGHT])
            {
                man_run(character1->man,MAN_WAY_LEFT);
                run(character1->man,&n1);
            }
            else
                man_stand(character1->man,character1->man->way);
        }
        if(state[SDL_SCANCODE_RIGHT])
        {
            if(!state[SDL_SCANCODE_LEFT])
            {
                man_run(character1->man,MAN_WAY_RIGHT);
                run(character1->man,&n1);
            }
            else
                man_stand(character1->man,character1->man->way);
        }

        if(state[SDL_SCANCODE_J])
        {
            isSkill = 1;
            if(character->man->way == MAN_WAY_LEFT)
                skill->object->dx = -5;
            else
                skill->object->dx = 5;
        }
        
        if(character->man->state[MAN_STATE_STAND])
            stand(character->man,&n);
        if(character1->man->state[MAN_STATE_STAND])
            stand(character1->man,&n1);
        
        if(character->man->state[MAN_STATE_HURT])
            hurt(&n);
        if(character1->man->state[MAN_STATE_HURT])
            hurt(&n1);

        character_updata(character);
        character_updata(character1);
        object_updata(object);
        object_updata(object1);
        object_updata(object2);
        object_updata(object3);
        skill_object_updata(skill);
        render();
    }
}

void game()
{
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:quit = 1;
            break;
        case SDL_KEYDOWN:
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
            case SDLK_w:
                character->man->jumpM = 0;
                break;
            case SDLK_a:
                man_stand(character->man,character->man->way);
                stand(character->man,&n);
                break;
            case SDLK_d:
                man_stand(character->man,character->man->way);
                stand(character->man,&n);
                break;
            case SDLK_j:
                man_stand(character1->man,character1->man->way);
                break;
            case SDLK_UP:
                character1->man->jumpM = 0;
                break;
            case SDLK_LEFT:
                man_stand(character1->man,character1->man->way);
                stand(character1->man,&n1);
                break;
            case SDLK_RIGHT:
                man_stand(character1->man,character1->man->way);
                stand(character1->man,&n1);
                break;
            }
            break;
        }
    }
}

void render()
{
    if(SDL_GetTicks() - FPS_Timer < FPS)
        SDL_Delay(FPS - SDL_GetTicks() + FPS_Timer);
    FPS_Timer = SDL_GetTicks();
    
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer,background->texture,NULL,NULL);
    
    object_render(object,WAY_RIGHT,renderer);
    object_render(object1,WAY_RIGHT,renderer);
    object_render(object2,WAY_RIGHT,renderer);
    object_render(object3,WAY_RIGHT,renderer);

    if(character->man->state[MAN_STATE_STAND])
        man_render(character->man,character->man->way,renderer,character->texture,character->man->spirit_rect,0,3);
    else if(character->man->state[MAN_STATE_RUN])
        man_render(character->man,character->man->way,renderer,character->texture,character->man->spirit_rect,4,7);
    else
        man_render(character->man,character->man->way,renderer,character->texture,character->man->spirit_rect,0,3);
    if(character1->man->state[MAN_STATE_STAND])
        man_render(character1->man,character1->man->way,renderer,character1->texture,character1->man->spirit_rect,0,3);
    else if(character1->man->state[MAN_STATE_RUN])
        man_render(character1->man,character1->man->way,renderer,character1->texture,character1->man->spirit_rect,4,7);
    else
        man_render(character1->man,character1->man->way,renderer,character1->texture,character1->man->spirit_rect,0,3);
        
    //skill_object_render(skill,character->man->way,renderer);

    if(isSkill)
        skill_object_render(skill,character->man->way,renderer);

    SDL_RenderPresent(renderer);
}

void quit_game()
{
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void stand(Man* man,int* i)
{
    if(man->state[MAN_STATE_STAND])
    {
        if(*i < 3)
            *i += 1;
        else
            *i = 0;
    }

    /*
    if(man->height != man_stand_height)
                is = 1;
    man->width = man_stand_width;
    man->height = man_stand_height;
    if(is)
    {
        is = 0;
        man->y -= man_stand_height - man_run_height;
    }
    */
}

void walk(Man* man,int* i)
{
    if(man->state[MAN_STATE_WALK])
    {
        if(*i < 7)
            *i += 1;
        else
            *i = 4;
    }
}

void run(Man* man,int* i)
{
    if(man->state[MAN_STATE_RUN])
    {
        if(*i < 22 && *i > 19)
            *i += 1;
        else
            *i = 20;
    }
    
    /*
    if(man->height != man_run_height)
        is = 1;
    man->width = man_run_width;
    man->height = man_run_height;
    if(is)
    {
        is = 0;
        man->y += man_stand_height - man_run_height;
    }
    */
}

void hurt(int* i)
{
    if(character->man->state[MAN_STATE_HURT])
        *i = 30;
}

#endif // GAME_H_INCLUDED
