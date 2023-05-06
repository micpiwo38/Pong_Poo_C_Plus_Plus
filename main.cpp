#include "headers/window.hpp"
#include "headers/vector2.hpp"
#include "headers/ball.hpp"

//g++ *.cpp -w -lSDL2 -lSDL2_ttf -lSDL2_mixer -o pong-poo

int main(){
    //Instance de la classe stockée dans la variable window
    Window window;
    //Appel de la methode CreateWindow()
    window.CreateWindow();

    return 0;
}