#pragma once

#include <SDL.h>
#include <iostream>
#include "game.h"

class paddle{
    public: 
        SDL_Rect rect;
        bool move_up = false;
        bool move_down = false;
        int speed = 6;
        int reset_position_y;
        SDL_Renderer* renderer;

        paddle(SDL_Renderer* renderer);

        void UpdatePos();

        void reset();

        void CheckBorderCollision();

        void rend();
};
