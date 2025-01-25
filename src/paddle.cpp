#include <paddle.h>
    
    
paddle::paddle(SDL_Renderer* renderer) : renderer(renderer){
    rect.w = 8;
    rect.h = 100;
    res_y_pos = WINSIZE_HEIGHT/2 - rect.h/2;
    reset_position_y();
    rect.x = 0;
}

void paddle::UpdatePos(){
    if ( move_up == true ) {
        rect.y -= speed;
    }
    if ( move_down == true ) {
        rect.y += speed;
    }

    CheckBorderCollision();
}

void paddle::CheckBorderCollision(){
    if(rect.y < 0){
        rect.y = 0;
    }
    if(rect.y + rect.h > WINSIZE_HEIGHT){
        rect.y -= speed;
    }
}

void paddle::reset_position_y(){
    rect.y = res_y_pos;
}

void paddle::rend(){

    if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0){
        std::cout << SDL_GetError() << std::endl;
    }
    
    if(SDL_RenderFillRect(renderer, &rect) != 0){
        std::cout << SDL_GetError() << std::endl;
    }
}