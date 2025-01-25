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

            ResetPosition_x = WINSIZE_WIDTH/2 - rect.w/2;
            ResetPosition_y = WINSIZE_HEIGHT/2 - rect.h/2;

            rect.y = ResetPosition_y;
            rect.x = ResetPosition_x;
        }

        int CheckBorderCollision(){
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
                return 1;
            }
            if(rect.x < 0){
                //speed_x *= -1;
                rect.x = ResetPosition_x;
                rect.y = ResetPosition_y;
                return 1;
            }

            return 0;
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

    srand(time(NULL));

    bool running = true;
    //cout << "point 4" << endl;

    SDL_Event event;
    while(running){
        
        if (game.GetState() == GAMESTATE::EXIT){
            running = false;
        }

        while(game.GetState() == GAMESTATE::PAUSE){
            // cout << "gamestate = pause" << endl;
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
                            game.SetState(GAMESTATE::PLAY);
                            break;
                        case SDLK_d :
                             paddle1.move_down = true;
                            cout << "paddle1 move down set true";
                            game.SetState(GAMESTATE::PLAY);
                            break;
                    }
                }
                else if(event.type == SDL_KEYUP){
                    switch(event.key.keysym.sym){
                        case SDLK_a :
                            paddle1.move_up = false;
                            cout << "paddle1 move up set true";
                            game.SetState(GAMESTATE::PLAY);
                            break;
                        case SDLK_d :
                             paddle1.move_down = false;
                            cout << "paddle1 move down set true";
                            game.SetState(GAMESTATE::PLAY);
                            break;
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            paddle1.rend();
            paddle2.rend();
            ball.rend();
            // cout << ball.rect.x << " " << ball.rect.y << endl;
            SDL_RenderPresent(renderer);
            
        }

        //cout << "point 5" << endl;
        ball.speed_x = 6 * (rand() % 2 == 0 ? 1 : -1);
        ball.speed_y = 6 * (rand() % 2 == 0 ? 1 : -1);
        
        while(game.GetState() == GAMESTATE::PLAY){


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
            // cout << "gamestate = play" << endl;
            int startframe = SDL_GetTicks();

            paddle1.UpdatePos();
            paddle2.UpdatePos();

            ball.rect.x += ball.speed_x;
            ball.rect.y += ball.speed_y;

            if(ball.CheckBorderCollision()){
                game.SetState(GAMESTATE::PAUSE);
                paddle1.reset_position_y();
                paddle2.reset_position_y();

            }

            if(CheckRectCollision(&paddle1.rect, &ball.rect)){
                ball.rect.x -= ball.speed_x;
                ball.speed_x *= -1;
            }
            if(CheckRectCollision(&paddle2.rect, &ball.rect)){
                ball.rect.x -= ball.speed_x;
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
        
    }
    //outside game loop

    SDL_Quit();
    //cout << "point 10" << endl;
    return 0;
}

/*
TODO:
1. fix paddle 2 moving bug in play game state
2. add computer AI
3. make ball faster as time passes 
4. fix ball colliding with paddle edge bug
5. track and display scores
6. improve ball's bouncing logic (hard)
*/