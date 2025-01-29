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
        float velocity_x = 0;
        float velocity_y = 0;
        int speed = 6;
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
                velocity_y *= -1;
            }
            if(rect.y < 0){
                velocity_y *= -1;
            }
            if(rect.x + rect.h > WINSIZE_WIDTH){
                //speed_x *= -1;
                rect.x = ResetPosition_x;
                rect.y = ResetPosition_y;
                return 2;
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

    paddle paddle[2] = {renderer, renderer};
    paddle[0].rect.x = 20;
    paddle[1].rect.x = (WINSIZE_WIDTH - paddle[2].rect.w) - 20;

    ball_class ball(renderer);

    srand(time(NULL));
    bool playerhitball = false;
    bool player2 = false;
    bool running = true;
    int hitcount = 0;
    //cout << "point 4" << endl;

    SDL_Event event;

    /*
    inside the main while loop there's 2 game state while loop, one for drawing pause scene and 
    the other one is for gameplay scene, both scene must listen to input.
    */
    while(running){
        
        if (game.GetState() == GAMESTATE::EXIT){
            running = false;
        }


        //draw game state pause
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
                            paddle[0].move_up = true;
                            // cout << "paddle1 move up set true";
                            game.SetState(GAMESTATE::PLAY);
                            break;
                        case SDLK_d :
                             paddle[0].move_down = true;
                            // cout << "paddle1 move down set true";
                            game.SetState(GAMESTATE::PLAY);
                            break;
                    }
                }
                else if(event.type == SDL_KEYUP){
                    switch(event.key.keysym.sym){
                        case SDLK_a :
                            paddle[0].move_up = false;
                            // cout << "paddle1 move up set true";
                            game.SetState(GAMESTATE::PLAY);
                            break;
                        case SDLK_d :
                             paddle[0].move_down = false;
                            // cout << "paddle1 move down set true";
                            game.SetState(GAMESTATE::PLAY);
                            break;
                    }
                }
            }
            //reset player 2
            player2 = false;
            //computer should move if the ball goes right first
            playerhitball = true;
            hitcount = 0;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            for (int i = 0; i < 2; i++){
                paddle[i].rend();
            }

            ball.rend();
            // cout << ball.rect.x << " " << ball.rect.y << endl;
            SDL_RenderPresent(renderer);
            
        }

        //cout << "point 5" << endl;
        ball.velocity_x = ball.speed * (rand() % 2 == 0 ? 1 : -1);
        ball.velocity_y = ball.speed * (rand() % 2 == 0 ? 1 : -1);
        
        //draw gamestate play
        while(game.GetState() == GAMESTATE::PLAY){

            //input handling
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    std::cout << "\ngame loop set false";
                    game.SetState(GAMESTATE::EXIT);
                }
                else if(event.type == SDL_KEYDOWN){
                    switch(event.key.keysym.sym){
                        case SDLK_a :
                            paddle[0].move_up = true;
                            // cout << "paddle[1] move up set true";
                            break;
                        case SDLK_d :
                            paddle[0].move_down = true;
                            // cout << "paddle1 move down set true";
                            break;
                        case SDLK_j :
                            paddle[1].move_up = true;
                            // cout << "paddle2 move up set true";
                            break;
                        case SDLK_l :
                            paddle[1].move_down = true;
                            // cout << "paddle2 move down set true";
                            break;
                    } 
                }
                else if(event.type == SDL_KEYUP){
                    switch(event.key.keysym.sym){
                        case SDLK_a :
                            paddle[0].move_up = false;
                            // cout << "paddle1 move up set false";
                            break;
                        case SDLK_d :
                            paddle[0].move_down = false;
                            // cout << "paddle1 move down set false";
                            break;
                        case SDLK_j :
                            paddle[1].move_up = false;
                            // cout << "paddle2 move up set false";
                            break;
                        case SDLK_l :
                            paddle[1].move_down = false;
                            // cout << "paddle2 move down set false";
                            break;
                    }
                }
            }
            // cout << "gamestate = play" << endl;
            int startframe = SDL_GetTicks();

            if (paddle[1].move_down == true || paddle[1].move_down == true){
                player2 = true;
            }

            for (int i = 0; i < 2; i++){
                paddle[i].UpdatePos();
            }

            ball.rect.x += ball.velocity_x;
            ball.rect.y += ball.velocity_y;

            //check if score should be added
            int add_score = ball.CheckBorderCollision();
            if(add_score){
                if (add_score == 2){
                    paddle[0].score++;
                }
                else if (add_score == 1){
                    paddle[1].score++;
                }

                for (int i = 0; i < 2; i++){
                    paddle[i].reset();
                    cout << "paddle" << i << " score: " << paddle[i].score << "\n";
                }

                game.SetState(GAMESTATE::PAUSE);
            }

            //paddle collision
            if(CheckRectCollision(&paddle[0].rect, &ball.rect)){
                ball.rect.x = paddle[0].rect.x + paddle[0].rect.w;
                ball.velocity_x *= -1;
                playerhitball = true;
                hitcount++;
                cout << "hitcount = " << hitcount << "\n";
            }
            if(CheckRectCollision(&paddle[1].rect, &ball.rect)){
                ball.rect.x = paddle[1].rect.x - ball.rect.w;
                ball.velocity_x *= -1;
                playerhitball = false;
                hitcount++;
                cout << "hitcount = " << hitcount << "\n";
            }

            //make the ball goes faster
            if(ball.velocity_x < 10){
                if(hitcount > 0 && hitcount % 5 == 0){
                    //i wanted to continue counting the ball hit but then the if statement would
                    //just trigger more than one time
                    hitcount = 0;

                    /*because we were working with velocity so i can't just add it by a number because the 
                    velocity could be a negative so it could be more slower e.g (-6 + 2) so i made a workaround*/
                    ball.velocity_x += ball.velocity_x * 0.05;
                    ball.velocity_y += ball.velocity_x * 0.05;
                    cout << "ball velocity: " << ball.velocity_x << endl;
                 }
            }


            //lord forgive me for the nested if
            if(!player2){

                //stop if computer successfully hit the ball
                if(playerhitball){

                    //only move if the ball is on the right side of the screen
                    if (ball.rect.x + ball.rect.w/2 > WINSIZE_HEIGHT/2){

                        //check if the ball is higher or lower
                        if (ball.rect.y > paddle[1].rect.y + paddle[1].rect.h/2){
                            paddle[1].rect.y += paddle[1].speed;
                        }
                        if (ball.rect.y < paddle[1].rect.y + paddle[1].rect.h/2){
                            paddle[1].rect.y -= paddle[1].speed;
                        }
                    }
                }
            }

            //cout << "point 6" << endl;
            
            //draw things
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            for (int i = 0; i < 2; i++){
                paddle[i].rend();
            }

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
1. add computer AI (DONE)
2. make ball faster as time passes (DONE)
3. track and display scores
4. improve ball's bouncing logic (hard)
*/