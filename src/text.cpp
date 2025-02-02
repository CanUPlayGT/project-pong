#include <text.h>

#include <iostream>

//this will create a dummy text texture, you'll have to call UpdateTexture(), and set_pos() to change it
LoadText::LoadText(SDL_Renderer *renderer, TTF_Font* font)
: renderer(renderer), font(font){
    rect.x = 0;
    rect.y = 0;
    UpdateTexture(text, {255, 255, 255, 255});
}

void LoadText::set_pos(int x, int y){
    rect.x = x;
    rect.y = y;
}

void LoadText::UpdateTexture(const char* new_text, SDL_Color color){
    surface = TTF_RenderUTF8_Solid(font, new_text, color);
    if(surface == NULL){
        std::cout << " Error: " << SDL_GetError() << std::endl;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect.h = surface->h;
    rect.w = surface->w;
}

LoadText::~LoadText(){
    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void LoadText::rendcopy(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}