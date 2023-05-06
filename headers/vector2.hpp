#ifndef __VECTOR2__
#define __VECTOR2__


class Vector2
{

   
public:
    
    //Constructeur position initiale retourne x et y a chaque instance
    Vector2(): x(0.0f), y(0.0f){};
    //Constructeur avec 2 paramètres
    Vector2(float x, float y): x(x), y(y){};
    //Surchage a l'aide de operator pour realisé des additions et des multiplications
    //le parametre = postion += velocité * time = pvt
    Vector2 operator+(Vector2 const &pvt){
        return Vector2(x + pvt.x, y + pvt.y);
    }
    //60fps
    Vector2& operator+=(Vector2 const& pvt){
        x += pvt.x;
        y += pvt.y;
        return *this;
    }

    //Mulitplication
    Vector2 operator*(float pvt){
        return Vector2(x * pvt, y * pvt);
    }

    float x, y;

};




#endif