#pragma once

#include "vector2.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

class Score{
    public:
        Score(Vector2 position, SDL_Renderer* renderer, TTF_Font* font);
        ~Score();

        void DrawScore();
        void SetScore(int score);
        SDL_Renderer* renderer;
        TTF_Font* font;
        SDL_Surface* surface{};
        SDL_Texture* texture{};
        SDL_Rect rect{};
};