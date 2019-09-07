
#include <SDL2/SDL.h>

#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#define TITLE "Game"
#define WIDTH 800
#define HEIGHT 600

static SDL_Window* window = NULL;

SDL_Window* init_window()
{
    SDL_Init(SDL_INIT_TIMER || SDL_INIT_GAMECONTROLLER || SDL_INIT_EVENTS);
    window = SDL_CreateWindow(TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
    
    return window;
}

#endif // WINDOW_H_INCLUDED
