#pragma once

#include <SDL.h>
#include <iostream>

extern const int WINSIZE_WIDTH; 
extern const int WINSIZE_HEIGHT; //this mean the variable is defined somewhere else 

enum class GAMESTATE{PLAY, EXIT, PAUSE, GAMEOVER};

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

//for code reusability
namespace gm{
    //pretty self-explanatory
    // \return 1 on collision
    int CheckRectCollision(SDL_Rect *Rect1, SDL_Rect *Rect2);
    //converts an integer to const char*
    //i made this so i can pass converted int to TTF_RenderText()
    const char* itoc(int i);
}