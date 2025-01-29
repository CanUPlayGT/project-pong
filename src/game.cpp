#include <game.h>

Game::Game(SDL_Window **window, SDL_Renderer **renderer, int x, int y, int w, int h, Uint32 flags){
	*window = SDL_CreateWindow("Window",  x,  y, w, h, flags);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if(window == nullptr || renderer == nullptr){
		std::cout << "Window/Rendereer Creations Failed: " << SDL_GetError() << std::endl;
		SDL_DestroyRenderer(*renderer);
		SDL_DestroyWindow(*window);
	}
}

void Game::initialize(){
	_state = GAMESTATE::PAUSE;

	if(SDL_Init(SDL_INIT_VIDEO||SDL_INIT_AUDIO) != 0){
		std::cout << "SDL Initializations Failed: " << SDL_GetError() << std::endl;
		_state = GAMESTATE::EXIT;
	}

	if(TTF_Init() != 0){
		std::cout << "TTF Initializations Failed: " << TTF_GetError() << std::endl;
		_state = GAMESTATE::EXIT;
	}
}

GAMESTATE Game::GetState(){
	return _state;
}

void Game::SetState(GAMESTATE state){
	_state = state;
}

Game::~Game(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    TTF_Quit();
}

// returns 1 on collision
int CheckRectCollision(SDL_Rect *Rect1, SDL_Rect *Rect2){
    //i moved this function here so that i have reusable code
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
        return 0;
    }
    if (rect2_left > rect1_right) {
        return 0;
    }
    if (rect2_top > rect1_down) {
        return 0;
    }
    if (rect2_down < rect1_top) {
        return 0;
    }

    return 1;

}