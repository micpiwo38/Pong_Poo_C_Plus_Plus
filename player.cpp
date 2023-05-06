#include "headers/player.hpp"

Player::Player(Vector2 position, Vector2 velocity):position(position), velocity(velocity){
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = PLAYER_WIDTH;
    rect.h = PLAYER_HEIGHT;
}

//Mise a jour pour le deplacement de haut en bas
void Player::UpdatePlayer(float delta_time){
    position += velocity * delta_time;
    if(position.y < 0){
        //Restriction pour le haut de l'ecran
        position.y = 0;
        //Idem en bas
    }else if(position.y > 705){
        position.y = 705;
    }
}

void Player::DrawPlayer(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 95,140,100,255);
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = PLAYER_WIDTH;
    rect.h = PLAYER_HEIGHT;
    SDL_RenderFillRect(renderer, &rect);
}

