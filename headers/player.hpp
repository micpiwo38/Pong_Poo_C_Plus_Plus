#pragma once

#include "vector2.hpp"
#include <SDL2/SDL.h>


class Player
{

public:
    Player(Vector2 position, Vector2 velocity);
    void DrawPlayer(SDL_Renderer* renderer);

    void UpdatePlayer(float delta_time);

    Vector2 position;
    Vector2 velocity;
    SDL_Rect rect{};
    const int PLAYER_WIDTH = 10;
    const int PLAYER_HEIGHT = 100;
    
    
};

