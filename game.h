
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ui.h"

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
Object* object4;
Object* object5;

Skill_Object* skill;
int attack_way = 0;

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

void init_game(SDL_Window* window)
{
    #ifdef _WIN32
    printf("window\n");
    #endif

    #ifdef __linux
    printf("linux\n");
    #endif
    
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    //surface = IMG_ReadXPMFromArray(icon_xpm);
    
    character = character_new("kakashi",100,100,1,0,100,500,man_stand_width,man_stand_height,
                                "data/kakashi.png",renderer,0,0,0,0);
    character1 = character_new("kakashi",100,100,1,0,600,500,man_stand_width,man_stand_height,
                                "data/kakashi.png",renderer,0,0,0,0);
    character->man->spirit_rect = init_spirit_rect(man_stand_width,man_stand_height,0,0,10,5);
    character1->man->spirit_rect = init_spirit_rect(man_stand_width,man_stand_height,0,0,10,5);
    character1->man->way = MAN_WAY_LEFT;
    //init_spirit(man);
    //init_spirit(man1);

    object = object_new(-1000,580,2000,50,"data/pane.png",renderer,255,255,255,255);
    
    /*
    object1 = object_new(500,200,100,50,"data/pane.png",renderer,255,255,255,255);
    object2 = object_new(300,300,200,50,"data/pane.png",renderer,255,255,255,255);
    object3 = object_new(100,100,200,50,"data/pane.png",renderer,255,255,255,255);
    object4 = object_new(790,0,10,600,"data/pane.png",renderer,255,255,255,255);
    object5 = object_new(0,0,10,600,"data/pane.png",renderer,255,255,255,255);
    */
    
    skill = skill_object_new(200,500,character->man->width,character->man->height
    ,5,5,"data/skill.bmp",renderer,0,0,0,0,init_spirit_rect(328/4,249/3,0,0,4,1));

    background = background_new("data/background.jpg",renderer,0,0,0,0);
    
    init_ui(renderer);
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
                if(!character->state[CHARACTER_STATE_ATTACK])
                    man_walk(character->man,MAN_WAY_LEFT);
            }
            else
                man_stand(character->man,character->man->way);
        }
        if(state[SDL_SCANCODE_D])
        {
            if(!state[SDL_SCANCODE_A])
            {
                if(!character->state[CHARACTER_STATE_ATTACK])
                    man_walk(character->man,MAN_WAY_RIGHT);
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
                if(!character1->state[CHARACTER_STATE_ATTACK])
                    man_walk(character1->man,MAN_WAY_LEFT);
            }
            else
                man_stand(character1->man,character1->man->way);
        }
        if(state[SDL_SCANCODE_RIGHT])
        {
            if(!state[SDL_SCANCODE_LEFT])
            {
                if(!character1->state[CHARACTER_STATE_ATTACK])
                    man_walk(character1->man,MAN_WAY_RIGHT);
            }
            else
                man_stand(character1->man,character1->man->way);
        }

        if(state[SDL_SCANCODE_J])
        {
            if(!character->state[CHARACTER_STATE_ATTACK])
            {
                character_attack(character,character->man->way,character->ATK);
            }
        }
        if(state[SDL_SCANCODE_KP_1])
        {
            if(!character1->state[CHARACTER_STATE_ATTACK])
            {
                character_attack(character1,character1->man->way,character1->ATK);
            }
        }
        
        if(state[SDL_SCANCODE_U])
        {
            if(!character->state[CHARACTER_STATE_ATTACK_SKILL_DISTANCE])
            {
                character_attack_skill_distance(character,skill,character->man->way);
            }
        }
        if(state[SDL_SCANCODE_KP_4])
        {
            if(!character1->state[CHARACTER_STATE_ATTACK_SKILL_DISTANCE])
            {
                character_attack_skill_distance(character1,skill,character1->man->way);
            }
        }
        
        character_updata(character,renderer);
        character_updata(character1,renderer);
        object_updata(object);
        /*
        object_updata(object1);
        object_updata(object2);
        object_updata(object3);
        object_updata(object4);
        object_updata(object5);
        */
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
                break;
            case SDLK_d:
                man_stand(character->man,character->man->way);
                break;
            case SDLK_j:
                character->state[CHARACTER_STATE_ATTACK] = 0;
                man_stand(character->man,character->man->way);
                break;
            case SDLK_UP:
                character1->man->jumpM = 0;
                break;
            case SDLK_LEFT:
                man_stand(character1->man,character1->man->way);
                break;
            case SDLK_RIGHT:
                man_stand(character1->man,character1->man->way);
                break;
            case SDLK_KP_1:
                character1->state[CHARACTER_STATE_ATTACK] = 0;
                man_stand(character1->man,character1->man->way);
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
    draw_hp(0,10,character->HP * 3.5,25);
    draw_mp(0,40,character->MP * 3.5,25);
    draw_hp(450,10,character1->HP * 3.5,25);
    draw_mp(450,40,character1->MP * 3.5,25);
    
    object_render(object,WAY_RIGHT,renderer);
    /*
    object_render(object1,WAY_RIGHT,renderer);
    object_render(object2,WAY_RIGHT,renderer);
    object_render(object3,WAY_RIGHT,renderer);
    object_render(object4,WAY_RIGHT,renderer);
    object_render(object5,WAY_RIGHT,renderer);
    */
    
    if(character->man->state[MAN_STATE_HURT])
        character_render(character,character->man->way,renderer,character->texture,character->man->spirit_rect,30,31);
    else if(character->state[CHARACTER_STATE_ATTACK])
        character_render(character,character->man->way,renderer,character->texture,character->man->spirit_rect,10,17);
    else if(character->man->state[MAN_STATE_STAND])
        character_render(character,character->man->way,renderer,character->texture,character->man->spirit_rect,0,3);
    else if(character->man->state[MAN_STATE_WALK])
        character_render(character,character->man->way,renderer,character->texture,character->man->spirit_rect,4,7);
    else if(character->man->state[MAN_STATE_RUN])
        character_render(character,character->man->way,renderer,character->texture,character->man->spirit_rect,20,22);
    else
        character_render(character,character->man->way,renderer,character->texture,character->man->spirit_rect,0,3);
    
    if(character1->man->state[MAN_STATE_HURT])
        character_render(character1,character1->man->way,renderer,character1->texture,character1->man->spirit_rect,30,31);
    else if(character1->state[CHARACTER_STATE_ATTACK])
        character_render(character1,character1->man->way,renderer,character1->texture,character1->man->spirit_rect,10,17);
    else if(character1->man->state[MAN_STATE_STAND])
        character_render(character1,character1->man->way,renderer,character1->texture,character1->man->spirit_rect,0,3);
    else if(character1->man->state[MAN_STATE_WALK])
        character_render(character1,character1->man->way,renderer,character1->texture,character1->man->spirit_rect,4,7);
    else if(character1->man->state[MAN_STATE_RUN])
        character_render(character1,character1->man->way,renderer,character1->texture,character1->man->spirit_rect,20,22);
    else
        character_render(character1,character1->man->way,renderer,character1->texture,character1->man->spirit_rect,0,3);
    
    if(character->state[CHARACTER_STATE_ATTACK_SKILL_DISTANCE])
        skill_object_render(skill,skill->way,renderer);
    if(character1->state[CHARACTER_STATE_ATTACK_SKILL_DISTANCE])
        skill_object_render(skill,skill->way,renderer);

    SDL_RenderPresent(renderer);
}

void quit_game()
{
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

#endif // GAME_H_INCLUDED
