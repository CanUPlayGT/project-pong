#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

extern const int WINSIZE_WIDTH;
extern const int WINSIZE_HEIGHT;

enum class GAMESTATE{PLAY, EXIT, PAUSE};

class Game{
    public:
        Game(SDL_Window **window, SDL_Renderer **renderer, int x, int y, int w, int h, Uint32 flags);
        ~Game();
        void initialize();
        
        GAMESTATE GetState();
        void SetState(GAMESTATE state);
        
    private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    GAMESTATE _state;
};

