#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#include "game.h"
#include "paddle.h"
#include "ball.h"
#include "text.h"

using std::cout, std::endl;

const int WINSIZE_WIDTH = 800;
const int WINSIZE_HEIGHT = 600;
const int FPS = 60;
const int FRAME_TIME = 1000/FPS;

const SDL_Color WHITE = {255, 255, 255, 255};

int main(int argc, char *args[]){
    SDL_Window *window;
	SDL_Renderer *renderer;

    // cout << "point 1" << endl;

	Game game(
		&window, &renderer, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINSIZE_WIDTH, WINSIZE_HEIGHT, SDL_WINDOW_SHOWN
	);

	// cout << "point 2" << endl;

	game.initialize();

    // cout << "point 3" << endl;

    paddle paddle[2] = {renderer, renderer};

    //just explicitly set paddle position
    paddle[0].rect.x = 20;
    paddle[1].rect.x = (WINSIZE_WIDTH - paddle[1].rect.w) - 20;

    ball_class ball(renderer);

    // cout << "point 4" << endl;

    TTF_Font *arial32 = TTF_OpenFont("ariblk.ttf", 32);
    if(arial32 == NULL){
        cout << "Error: " << SDL_GetError() << endl;
        TTF_CloseFont(arial32);
        return -1;
    }
    TTF_Font *arial64 = TTF_OpenFont("ariblk.ttf", 64);
    if(arial32 == NULL){
        cout << "Error: " << SDL_GetError() << endl;
        TTF_CloseFont(arial64);
        return -1;
    }

    // cout << "point 5" << endl;

    //this is for displaying message on game over scene
    LoadText win_message(renderer, arial64);
    LoadText restart_message(renderer, arial32);

    // cout << "point 6" << endl;

    //create sdl_texture for displaying score
    //this will be updated if the score changes

    const int score_x_position[] = {300, 500};

    LoadText score[2] = {
        {renderer, arial32},
        {renderer, arial32}
    };

    for (int i = 0; i < 2; i++){
        score[i].UpdateTexture("0", WHITE);
        score[i].set_pos(score_x_position[i] - score[i].rect.w / 2, 0);
    }

    // cout << "point 7" << endl;

    srand(time(NULL));
    int winner = 0;
    int hitcount = 0;
    bool playerhitball = false;
    bool player2 = false;

    //if player 2 ever control the paddle 2 this will set to true until game over scene
    //meanwhile player2 variable will get resetted after every game pause
    bool player2_flag = false;

    bool running = true;

    //cout << "point 4" << endl;

    SDL_Event event;
    
    //inside the main while loop there are multiple while loop for each game state, every scene should listen to input.
    while(running){

        // cout << "point 8" << endl;
        
        if (game.GetState() == GAMESTATE::EXIT){
            running = false;
        }

        while(game.GetState() == GAMESTATE::GAMEOVER){

            //update the text based on the winner
            if(player2_flag){
                if (winner == 1)
                {
                    win_message.UpdateTexture("PLAYER 1 WIN!", WHITE);
                    win_message.set_pos(WINSIZE_WIDTH/2 - win_message.rect.w / 2, 200);
                }
                else if (winner == 2)
                {
                    win_message.UpdateTexture("PLAYER 2 WIN!", WHITE);
                    win_message.set_pos(WINSIZE_WIDTH/2 - win_message.rect.w / 2, 200);
                }
            }

            else{
                if (winner == 1)
                {
                    win_message.UpdateTexture("PLAYER 1 WIN!", WHITE);
                    win_message.set_pos(WINSIZE_WIDTH/2 - win_message.rect.w / 2, 200);
                }
                else if (winner == 2)
                {
                    win_message.UpdateTexture("YOU LOSE!", WHITE);
                    win_message.set_pos(WINSIZE_WIDTH/2 - win_message.rect.w / 2, 200);
                }
            }

            restart_message.UpdateTexture("Press Space to Restart...", WHITE);
            restart_message.set_pos(WINSIZE_WIDTH/2 - restart_message.rect.w / 2, 400);

            //only render it 1 time

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            win_message.rendcopy();
            restart_message.rendcopy();

            SDL_RenderPresent(renderer);

            //keep listening for input
            while(game.GetState() == GAMESTATE::GAMEOVER){
                //SDL_WaitEvent helps to make cpu usage 0% when waiting for input
                SDL_WaitEvent(&event);
                if(event.type == SDL_QUIT){
                    std:: cout << "\ngame loop set false\n";
                    game.SetState(GAMESTATE::EXIT);
                }
                else if (event.type == SDL_KEYDOWN){
                    switch(event.key.keysym.sym){
                        case SDLK_SPACE:

                            game.SetState(GAMESTATE::PAUSE);

                            //reset some things
                            winner = 0;
                            player2_flag = false;
                            paddle[0].score = 0;
                            paddle[1].score = 0;

                            //reset score texture
                            for (int i = 0; i < 2; i++){
                            score[i].UpdateTexture("0", WHITE);
                            score[i].set_pos(score_x_position[i] - score[i].rect.w / 2, 0);
                            }

                            break;
                    }
                }
            }

            //gameover end loop
        }
        
        //draw game state pause
        while(game.GetState() == GAMESTATE::PAUSE){
            // cout << "point 9" << endl;

            //reset player 2
            player2 = false;
            //computer should move if the ball goes right first
            playerhitball = true;
            hitcount = 0;

            //render things once
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            for (int i = 0; i < 2; i++){
                paddle[i].rend();
                score[i].rendcopy();

                paddle[i].move_down = false;
                paddle[i].move_up = false;
            }

            ball.rend();

            // cout << ball.rect.x << " " << ball.rect.y << endl;
            SDL_RenderPresent(renderer);

            SDL_Delay(500); //prevent accidentally continuing after entering game pause

            while(game.GetState() == GAMESTATE::PAUSE){
                SDL_WaitEvent(&event);
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

                //the end loop of game state pause 
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
                player2_flag = true;
            }

            for (int i = 0; i < 2; i++){
                paddle[i].UpdatePos();
            }

            ball.rect.x += ball.velocity_x;
            ball.rect.y += ball.velocity_y;

            //check if score should be added
            int add_score = ball.CheckBorderCollision();
            switch(add_score){
                case 1:
                    paddle[0].score++;
                    break;
                case 2:
                    paddle[1].score++;
                    break;
            }

            //reset paddle position, change game state, and update score texture if either win
            if(add_score){
                for (int i = 0; i < 2; i++){
                    if(paddle[0].score == 5){
                        winner = 1;
                        game.SetState(GAMESTATE::GAMEOVER);
                    }
                    else if(paddle[1].score == 5){
                        winner = 2;
                        game.SetState(GAMESTATE::GAMEOVER);
                    }
                    else{
                        game.SetState(GAMESTATE::PAUSE);
                    }

                    paddle[i].reset();
                    cout << "paddle" << i + 1 << " score: " << paddle[i].score << "\n";

                    //update the score texture
                    score[i].UpdateTexture(gm::itoccp(paddle[i].score), WHITE);
                }
            }

            //paddle collision
            if(gm::CheckRectCollision(&paddle[0].rect, &ball.rect)){
                ball.rect.x = paddle[0].rect.x + paddle[0].rect.w;
                ball.velocity_x *= -1;
                playerhitball = true;
                hitcount++;
                cout << "hitcount = " << hitcount << "\n";
            }
            if(gm::CheckRectCollision(&paddle[1].rect, &ball.rect)){
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


            //computer movement logic
            if(!player2){

                //stop if computer successfully hit the ball
                if(playerhitball){

                    //only move if the ball is on the right side of the screen
                    if (ball.rect.x >= WINSIZE_HEIGHT/2){

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

            for (int i = 0; i < 2; i++){
                score[i].rendcopy();
            }

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
3. track and display scores (DONE)
4. add game over scene (DONE)
4. improve ball bouncing logic 
5. fix assertion failure error when exiting program (i have no idea what it is)
*/