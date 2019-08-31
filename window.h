
#include <SDL2/SDL.h>

#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#define TITLE "Game"
#define WIDTH 800
#define HEIGHT 600

static SDL_Window *window = NULL;

void init_window()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
}

#endif // WINDOW_H_INCLUDED
