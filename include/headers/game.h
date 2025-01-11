#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

enum class GAMESTATE{PLAY, EXIT};

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

