#include "headers/score.hpp"


Score::Score(Vector2 position, SDL_Renderer* renderer, TTF_Font* font): renderer(renderer), font(font){
    surface = TTF_RenderText_Solid(font, "0", {255,165,0});
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width, text_height;
    SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);

    //Position
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = text_width;
    rect.h = text_height;
}

//Desctucteur
Score::~Score(){
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Score::DrawScore(){
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Score::SetScore(int score){
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    surface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), {255,165,0});
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width, text_height;
    SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);
    rect.w = text_width;
    rect.h = text_height;
}