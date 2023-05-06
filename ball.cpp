#include "headers/window.hpp"
#include "headers/ball.hpp"
#include <SDL2/SDL.h>
#include <iostream>


Ball::Ball(Vector2 position, Vector2 velocity): position(position), velocity(velocity){
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = BALL_WIDTH;
    rect.h = BALL_HEIGHT;
}

void Ball::UpdateBall(float delta_time){
    //position + opérateur de surchage interne +=
    position += velocity * delta_time;
}

void Ball::DrawBall(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 145,50,50,255);
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = BALL_WIDTH;
    rect.h = BALL_HEIGHT;
    SDL_RenderFillRect(renderer, &rect); 
}

void Ball::CollideWithPlayer(Contact const& contact){
    //Operateur interne de surchage pour ajouter un angle a la collison
    position.x += contact.penetration;
    //Inverse la direction au moment de la collision
    velocity.x = -velocity.x;
    if(contact.type == CollisionType::Haut){
        velocity.y = -.75f * BALL_SPEED;
    }else if(contact.type == CollisionType::Bas){
        velocity.y = 0.75f * BALL_SPEED;
    }
}

void Ball::CollideWithWall(Contact const& contact){
    if((contact.type == CollisionType::Haut) || (contact.type == CollisionType::Bas)){
        position.y += contact.penetration;
        velocity.y = -velocity.y;
    }else if(contact.type == CollisionType::Gauche){
        position.x = 640;
        position.y = 360;
        velocity.x = BALL_SPEED;
        velocity.y = 0.75f * BALL_SPEED;
    }else if(contact.type == CollisionType::Droite){
        position.x = 640;
        position.y = 360;
        velocity.x = -BALL_SPEED;
        velocity.y = 0.75f * BALL_SPEED;
    }
}


