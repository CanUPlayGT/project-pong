#include <game.h>
#include <SDL.h>


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
		std::cout << "TTF Initializations Failed: " << SDL_GetError() << std::endl;
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
}