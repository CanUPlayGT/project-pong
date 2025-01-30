#pragma once

#include <iostream>
#include <SDL.h>
#include <game.h>

class ball_class{
    public:
        SDL_Rect rect;
        float velocity_x = 0;
        float velocity_y = 0;
        int speed = 6;
        SDL_Renderer *renderer;
        int ResetPosition_x = WINSIZE_WIDTH/2 - rect.w/2;
        int ResetPosition_y = WINSIZE_HEIGHT/2 - rect.h/2;

        ball_class(SDL_Renderer* renderer);

        int CheckBorderCollision();

        void rend();
    };