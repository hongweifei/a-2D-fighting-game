
#include "object.h"

SDL_Renderer* ui_renderer;

Background* ui_hp;
Background* ui_mp;
int ui_hp_ture = 0;
int ui_mp_ture = 0;

SDL_Rect src_rect_0 = {1,9,120,10};
SDL_Rect src_rect_1 = {1,21,120,10};

void init_ui(SDL_Renderer* renderer)
{
    ui_renderer = renderer;
    ui_hp = background_new("data/hp.png",renderer,0,0,0,0);
    ui_mp = background_new("data/mp.png",renderer,0,0,0,0);
}

void draw_hp(int x,int y,int width,int height)
{
    SDL_Rect rect = {x,y,width,height};
    
    SDL_RenderCopy(ui_renderer,ui_hp->texture,&src_rect_0,&rect);
    
    ui_hp_ture = 1;
}

void draw_mp(int x,int y,int width,int height)
{
    SDL_Rect rect = {x,y,width,height};
    
    SDL_RenderCopy(ui_renderer,ui_mp->texture,&src_rect_0,&rect);
    
    ui_mp_ture = 1;
}








