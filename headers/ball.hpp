#ifndef __BALL__
#define __BALL__

#include <SDL2/SDL.h>
#include "vector2.hpp"

class Ball{  
    public:
        //On passe en paramètre du constructeur une intance de la classe Vector2 = tableau a 2 entrées (x, y)
        Ball(Vector2 position, Vector2 velocity);
        void DrawBall(SDL_Renderer* renderer);
        void UpdateBall(float delta_time);
        Vector2 position;
        Vector2 velocity;

        //Enumeration des type de collison
        enum class CollisionType{
            Aucune,
            Haut,
            Milieu,
            Bas,
            Gauche,
            Droite,
        };

        struct Contact
        {
            //Appel de l'enum
            CollisionType type;
            float penetration;

        };
        void CollideWithPlayer(Contact const& contact);
        void CollideWithWall(Contact const& contact);
        SDL_Rect rect{};
        const int BALL_WIDTH = 15;
        const int BALL_HEIGHT = 15;
        const int BALL_SPEED = 1.0f;
};

#endif