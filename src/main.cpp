#include <SDL.h>
#include <iostream>
#include <unordered_set>

#include <game.h>

const int WINSIZE_WIDTH = 800;
const int WINSIZE_HEIGHT = 600;
const int FPS = 60;
const int FRAME_TIME = 1000/FPS;

using std::cout, std::endl;

class paddle{
    public: 
        SDL_Rect rect;
        bool move_up = false;
        bool move_down = false;
        int speed = 6;
        SDL_Renderer* renderer;

        paddle(SDL_Renderer* renderer) : renderer(renderer){
            rect.w = 8;
            rect.h = 100;
            rect.y = WINSIZE_HEIGHT/2 - rect.h/2;
            rect.x = 0;
        }

        void UpdatePos(){
            if ( move_up == true ) {
                rect.y -= speed;
            }
            if ( move_down == true ) {
                rect.y += speed;
            }

            CheckBorderCollision();
        }

        void CheckBorderCollision(){
            if(rect.y < 0){
                rect.y = 0;
            }
            if(rect.y + rect.h > WINSIZE_HEIGHT){
                rect.y -= speed;
            }
        }

        void rend(){

            cout << "paddle rendering " << endl;

            if(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0){
                cout << SDL_GetError() << endl;
            }
            
            if(SDL_RenderFillRect(renderer, &rect) != 0){
                cout << SDL_GetError() << endl;
            }

            cout << "rendered successfully" << endl;
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

    //cout << "point 4" << endl;

    while(game.GetState() == GAMESTATE::PLAY){
        int startframe = SDL_GetTicks();

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                std::cout << "game loop set false";
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

        //cout << "point 6" << endl;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        paddle1.rend();
        paddle2.rend();

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


