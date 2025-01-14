#include <SDL.h>
#include <iostream>
#include <unordered_set>

#include "game.h"
#include "paddle.h"

const int WINSIZE_WIDTH = 800;
const int WINSIZE_HEIGHT = 600;
const int FPS = 60;
const int FRAME_TIME = 1000/FPS;

using std::cout, std::endl;

class ball_class{
    public:
        SDL_Rect rect;
        int speed_x = 0;
        int speed_y = 0;
        SDL_Renderer *renderer;
        int ResetPosition_x = WINSIZE_WIDTH/2 - rect.w/2;
        int ResetPosition_y = WINSIZE_HEIGHT/2 - rect.h/2;

        ball_class(SDL_Renderer* renderer) : renderer(renderer){
            rect.w = 10;
            rect.h = 10;
            rect.y = ResetPosition_y;
            rect.x = ResetPosition_x;
        }

        void CheckBorderCollision(){
            if(rect.y + rect.h > WINSIZE_HEIGHT){
                speed_y *= -1;
            }
            if(rect.y < 0){
                speed_y *= -1;
            }
            if(rect.x + rect.h > WINSIZE_WIDTH){
                //speed_x *= -1;
                rect.x = ResetPosition_x;
                rect.y = ResetPosition_y;

            }
            if(rect.x < 0){
                //speed_x *= -1;
                rect.x = ResetPosition_x;
                rect.y = ResetPosition_y;
            }
        }

        void rend(){
            if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0){
                std::cout << SDL_GetError() << std::endl;
            }
            
            if(SDL_RenderFillRect(renderer, &rect) != 0){
                std::cout << SDL_GetError() << std::endl;
            }
        }
        

};

int CheckCollision(SDL_Rect *Rect1, SDL_Rect *Rect2);

int main(int argc, char *args[]){
    SDL_Window *window;
	SDL_Renderer *renderer;

    //cout << "point 1" << endl;

	Game game(
		&window, &renderer, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINSIZE_WIDTH, WINSIZE_HEIGHT, SDL_WINDOW_SHOWN
	);

	//cout << "point 2" << endl;

	game.initialize();

    //cout << "point 3" << endl;

    paddle paddle1(renderer);
    paddle1.rect.x = 20;

    paddle paddle2(renderer);
    paddle2.rect.x = (WINSIZE_WIDTH - paddle2.rect.w) - 20;

    ball_class ball(renderer);
    ball.speed_x = -8;
    ball.speed_y = 8;

    //cout << "point 4" << endl;

    while(game.GetState() == GAMESTATE::PLAY){
        int startframe = SDL_GetTicks();

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                std::cout << "\ngame loop set false";
                game.SetState(GAMESTATE::EXIT);
            }
            else if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_a :
                        paddle1.move_up = true;
                        cout << "paddle1 move up set true";
                        break;
                    case SDLK_d :
                        paddle1.move_down = true;
                        cout << "paddle1 move down set true";
                        break;
                    case SDLK_j :
                        paddle2.move_up = true;
                        cout << "paddle1 move up set true";
                        break;
                    case SDLK_l :
                        paddle2.move_down = true;
                        cout << "paddle1 move down set true";
                        break;
                } 
            }
            else if(event.type == SDL_KEYUP){
                switch(event.key.keysym.sym){
                    case SDLK_a :
                        paddle1.move_up = false;
                        cout << "paddle1 move up set false";
                        break;
                    case SDLK_d :
                        paddle1.move_down = false;
                        cout << "paddle1 move down set false";
                        break;
                    case SDLK_j :
                        paddle2.move_up = false;
                        cout << "paddle1 move up set false";
                        break;
                    case SDLK_l :
                        paddle2.move_down = false;
                        cout << "paddle1 move down set false";
                        break;
                }
            }
        }

        //cout << "point 5" << endl;

        paddle1.UpdatePos();
        paddle2.UpdatePos();
        ball.rect.x += ball.speed_x;
        ball.rect.y += ball.speed_y;

        ball.CheckBorderCollision();

        if(CheckCollision(&paddle1.rect, &ball.rect)){
            ball.speed_x *= -1;
        }
        if(CheckCollision(&paddle2.rect, &ball.rect)){
            ball.speed_x *= -1;
        }

        //cout << "point 6" << endl;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        paddle1.rend();
        paddle2.rend();
        ball.rend();

        //cout << "point 7" << endl;

        int frame_delay = SDL_GetTicks() - startframe;
        if(frame_delay < FRAME_TIME){
            SDL_Delay(FRAME_TIME - frame_delay);
        } 

        //cout << "point 8" << endl;

        SDL_RenderPresent(renderer);

        //cout << "point 9" << endl;
        }
    //outside game loop

    SDL_Quit();
    //cout << "point 10" << endl;
    return 0;
}

int CheckCollision(SDL_Rect *Rect1, SDL_Rect *Rect2){
    //rect1
    int rect1_top = Rect1->y;
    int rect1_down = Rect1->y + Rect1->h;
    int rect1_right = Rect1->x + Rect1->w;
    int rect1_left = Rect1->x;

    //rect2
    int rect2_top = Rect2->y;
    int rect2_down = Rect2->y + Rect2->h;
    int rect2_right = Rect2->x + Rect2->w;
    int rect2_left = Rect2->x;

    if (rect2_right < rect1_left) {
        return false;
    }
    if (rect2_left > rect1_right) {
        return false;
    }
    if (rect2_top > rect1_down) {
        return false;
    }
    if (rect2_down < rect1_top) {
        return false;
    }

    cout << "collision set true" << endl;
    return true;

}

