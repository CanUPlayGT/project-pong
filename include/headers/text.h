#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

class LoadText{
    public:
        int width, height;
        SDL_Rect rect;
        SDL_Renderer *renderer;
        const char *text = "default";

        LoadText(SDL_Renderer *renderer, const char *text, const char *path, int size, SDL_Color color);
        LoadText(SDL_Renderer *renderer, TTF_Font* font);
        ~LoadText();

        void rendcopy();
        void UpdateTexture(const char* new_text, SDL_Color color);
        void set_pos(int x, int y);


    private:
        TTF_Font *font;
        SDL_Surface *surface;
        SDL_Texture *texture;
};