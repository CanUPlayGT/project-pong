#include "ball.h"

ball_class::ball_class(SDL_Renderer* renderer) : renderer(renderer){
    rect.w = 10;
    rect.h = 10;

    ResetPosition_x = WINSIZE_WIDTH/2 - rect.w/2;
    ResetPosition_y = WINSIZE_HEIGHT/2 - rect.h/2;

    rect.y = ResetPosition_y;
    rect.x = ResetPosition_x;
}

// \return 1 on paddle1 winning, 2 on paddle2 winning, 0 on nothing
int ball_class::CheckBorderCollision(){
    if(rect.y + rect.h > WINSIZE_HEIGHT){
        velocity_y *= -1;
    }
    if(rect.y < 0){
        velocity_y *= -1;
    }
    if(rect.x + rect.h > WINSIZE_WIDTH){
        //speed_x *= -1;
        rect.x = ResetPosition_x;
        rect.y = ResetPosition_y;
        return 1;
    }
    if(rect.x < 0){
        //speed_x *= -1;
        rect.x = ResetPosition_x;
        rect.y = ResetPosition_y;
        return 2;
    }

    return 0;
}

void ball_class::rend(){
    if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0){
        std::cout << SDL_GetError() << std::endl;
    }
    
    if(SDL_RenderFillRect(renderer, &rect) != 0){
        std::cout << SDL_GetError() << std::endl;
    } 
}

