#include "headers/window.hpp"
#include "headers/vector2.hpp"
#include "headers/ball.hpp"
#include "headers/player.hpp"
#include "headers/score.hpp"
#include <iostream>
#include <SDL2/SDL.h>
//La police 
#include <SDL2/SDL_ttf.h>
//Le son
#include <SDL2/SDL_mixer.h>
//le timer
#include <chrono>

//CONST
//Fenetre
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
//BALLE
const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15;
const float BALL_SPEED = 1.0f;

//LES 2 players
const int PLAYER_WIDTH = 10;
const int PLAYER_HEIGHT = 100;
//La vitesse du player
const float PLAYER_SPEED = 1.0f;

//Enumeration des 4 boutons
enum Buttons{
    PlayerOneUp = 0,
    PlayerOneDown,
    PlayerTwoUp,
    PlayerTwoDown,
};


SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,0);
bool game_is_running = true;
//Init du Tableau enum des 4 boutons
bool buttons[4] = {};
//FPS
float delta_time = 0.0f;
//Evenement souris + clavier
SDL_Event event;


//La balle + param = classe tableau Vector2 (x et y)
Ball balle(
    Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f),
    Vector2(BALL_SPEED, 0.0f));


//Instance de la classe Player + param tableau x et y pour la position
//Puis second param Vector2 pour la velocité en x et y
Player player_one(Vector2(50.0f, (WINDOW_HEIGHT / 2.0f) - (PLAYER_HEIGHT / 2.0f)),
    Vector2(0.0f,0.0f)
);

Player player_two(Vector2(WINDOW_WIDTH - 50.0f, (WINDOW_HEIGHT / 2.0f) - (PLAYER_HEIGHT / 2.0f)),
    Vector2(0.0f,0.0f)
);

//Dessinner la ligne de separation
void DrawMiddleLine(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,255);
    for(int y = 0; y < 720; ++y){
        if(y % 5){
            SDL_RenderDrawPoint(renderer, 640, y);
        }
    }
}

//Test de collison balle et player
//const& = passage de valeur par reference pour optimisation
Ball::Contact CheckPlayerCollision(Ball const& balle, Player const& player){
    //La balle
    float balle_gauche = balle.position.x;
    float balle_droite = balle.position.x + BALL_WIDTH;
    float balle_haut = balle.position.y;
    float balle_bas = balle.position.y + BALL_HEIGHT;

    //Les players
    float player_gauche = player.position.x;
    float player_droite = player.position.x + PLAYER_WIDTH;
    float player_haut = player.position.y;
    float player_bas = player.position.y + PLAYER_HEIGHT;

    //instance de la classe enum Contact
    Ball::Contact contact{};

    //Les consditions
    if(balle_gauche >= player_droite){
        return contact;
    }

    if(balle_droite <= player_gauche){
        return contact;
    }

    if(balle_haut >= player_bas){
        return contact;
    }

    if(balle_bas <= player_haut){
        return contact;
    }

    float playerRangeUp = player_bas - (2.0f * PLAYER_HEIGHT / 3.0f);
    float playerRangeMiddle = player_bas - (PLAYER_HEIGHT / 3.0f);

    if(balle.velocity.x < 0){
        //player gauche
        contact.penetration = player_droite - balle_gauche;
    }else if(balle.velocity.x > 0){
        //player droite
        contact.penetration = player_gauche - balle_droite;
    }

    if((balle_bas > player_gauche) && (balle_bas < playerRangeUp)){
        contact.type = Ball::CollisionType::Haut;
    }else if((balle_bas > playerRangeUp) && (balle_bas < playerRangeMiddle)){
        contact.type = Ball::CollisionType::Milieu;
    }else{
        contact.type = Ball::CollisionType::Bas;
    }

    return contact;
}

//Collision avec les mur gauche droite haut bas
Ball::Contact CheckWallCollision(Ball const& balle){
    float balle_gauche = balle.position.x;
    float balle_droite = balle.position.x + balle.BALL_WIDTH;
    float balle_haut = balle.position.y;
    float balle_bas = balle.position.y + balle.BALL_HEIGHT;

    Ball::Contact contact{};

    if(balle_gauche < 0.0f){
        contact.type = Ball::CollisionType::Gauche;
    }else if(balle_droite > WINDOW_WIDTH){
        contact.type = Ball::CollisionType::Droite;
    }else if(balle_haut < 0.0f){
        contact.type = Ball::CollisionType::Haut;
        contact.penetration = -balle_haut;
    }else if(balle_bas > WINDOW_HEIGHT){
        contact.type = Ball::CollisionType::Bas;
        contact.penetration = WINDOW_HEIGHT - balle_bas;
    }
    return contact;
}

void Window::CreateWindow(){
    //Init de la lib SDL2
    SDL_Init(SDL_INIT_EVERYTHING);
    //Init de la lib Police TTF
    TTF_Init();
    //Init de la lib SON mixer
    //Frequence + format + chaine + chunkSize = taille du morceau
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    //AJOUTER LE LINKER g++ *.cpp -w -lSDL2 -lSDL2_ttf -o pong-poo POUR LA COMPILATION
    TTF_Font* score_font = TTF_OpenFont("SunnySunday.ttf", 25); 
    int player_one_score = 0;
    int player_two_score = 0;
    Score player_one_score_text(Vector2(WINDOW_WIDTH / 4, 20), renderer, score_font);
    Score player_two_score_text(Vector2(3 * WINDOW_WIDTH / 4 ,20), renderer, score_font);

    //Audio
    //NE PAS OUBLIER LE LINKER POUR LE COMPILATION DE LA LIB MIXER
    //g++ *.cpp -w -lSDL2 -lSDL2_ttf -lSDL2_mixer -o pong-poo
    Mix_Chunk* wallHitSound = Mix_LoadWAV("hit2.wav");
    Mix_Chunk* playerHitSound = Mix_LoadWAV("hit3.wav");
    {
        while(game_is_running){
            //Le timer
            auto start_time = std::chrono::high_resolution_clock::now();
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    game_is_running = false;
                    
                }else if(event.type == SDL_KEYDOWN){
                    if(event.key.keysym.sym == SDLK_ESCAPE){
                        //Quitter le jeux avec ECHAP
                        game_is_running = false;
                    }else if(event.key.keysym.sym == SDLK_z){
                        //Vers le haut touche Z = pour player 1
                        buttons[Buttons::PlayerOneUp] = true;
                    }else if(event.key.keysym.sym == SDLK_s){
                        //Vers le bas touche S = pour player 1
                        buttons[Buttons::PlayerOneDown] = true;
                    }else if(event.key.keysym.sym == SDLK_UP){
                        //Vers le haut touche UP = pour player 2
                        buttons[Buttons::PlayerTwoUp] = true;
                    }else if(event.key.keysym.sym == SDLK_DOWN){
                        //Vers le bas touche DOWN = pour player 2
                        buttons[Buttons::PlayerTwoDown] = true;
                    }
                }else if(event.type == SDL_KEYUP){
                        //Touche relachée player 1 Z
                    if(event.key.keysym.sym == SDLK_z){
                        buttons[Buttons::PlayerOneUp] = false;
                    }else if(event.key.keysym.sym == SDLK_s){
                        //Touche relachée player 1 S
                        buttons[Buttons::PlayerOneDown] = false;
                    }else if(event.key.keysym.sym == SDLK_UP){
                        //Touche relachée player 2 UP
                        buttons[Buttons::PlayerTwoUp] = false;
                    }else if(event.key.keysym.sym == SDLK_DOWN){
                        //Touche relachée player 2 DOWN
                        buttons[Buttons::PlayerTwoDown] = false;
                    }
                }
            }

            //Sortie du PollEvent

            //Si on appuie HAUT = PLAYER 1
            if(buttons[Buttons::PlayerOneUp]){
                player_one.velocity.y = -PLAYER_SPEED;
            }else if(buttons[Buttons::PlayerOneDown]){
                player_one.velocity.y = PLAYER_SPEED;
            }else{
                //Si rien n'est appuyer = on reste statique
                player_one.velocity.y = 0.0f;
            }

            //Sin on appuie HAUT = PLAYER 2
            if(buttons[Buttons::PlayerTwoUp]){
                player_two.velocity.y = -PLAYER_SPEED;
            }else if(buttons[Buttons::PlayerTwoDown]){
                player_two.velocity.y = PLAYER_SPEED;
            }else{
                //Si rien n'est appuyer = on reste statique
                player_two.velocity.y = 0.0f;
            }

            player_one.UpdatePlayer(delta_time);
            player_two.UpdatePlayer(delta_time);

            //Déplacement de la balle
            balle.UpdateBall(delta_time);

            //Check les collision entre balle et les players
            if(Ball::Contact contact = CheckPlayerCollision(balle, player_one);
                contact.type != Ball::CollisionType::Aucune){

                    balle.CollideWithPlayer(contact);
                    //Son
                    Mix_PlayChannel(-1, playerHitSound,0);

                }else if(contact = CheckPlayerCollision(balle, player_two);
                contact.type != Ball::CollisionType::Aucune){

                    balle.CollideWithPlayer(contact);
                    //Son
                    Mix_PlayChannel(-1, playerHitSound,0);

                }else if(contact = CheckWallCollision(balle);
                    contact.type != Ball::CollisionType::Aucune){
                        balle.CollideWithWall(contact);
                        if(contact.type == Ball::CollisionType::Gauche){
                            ++player_two_score;
                            player_two_score_text.SetScore(player_two_score);
                        }else if(contact.type == Ball::CollisionType::Droite){
                            ++player_one_score;
                            player_one_score_text.SetScore(player_one_score);
                        }else{
                            //Chaine + morceau + boucle = 0 = false
                            Mix_PlayChannel(-1, wallHitSound, 0);
                        }
                    }



            SDL_SetRenderDrawColor(renderer, 0x456123, 0x852123, 0x635241, 0xFF);
            SDL_RenderClear(renderer);

            //La ligne de separation
            DrawMiddleLine(renderer);
            //La balle 
            balle.DrawBall(renderer);
            //Les 2 players
            player_one.DrawPlayer(renderer);
            player_two.DrawPlayer(renderer);
            //Les 2 scores
            player_one_score_text.DrawScore();
            player_two_score_text.DrawScore();

            SDL_RenderPresent(renderer);

            //Les frames
            auto stop_time = std::chrono::high_resolution_clock::now();
            delta_time = std::chrono::duration<float, std::chrono::milliseconds::period>(stop_time - start_time).count();
        }
        
    }

    //Liberer la memoire du son
    Mix_FreeChunk(wallHitSound);
    Mix_FreeChunk(playerHitSound);
    //Le rendu
    SDL_DestroyRenderer(renderer);
    //La fenetre
    SDL_DestroyWindow(window);
    //La police
    TTF_CloseFont(score_font);
    //Quitter le mixer
    Mix_Quit();
    //La police
    TTF_Quit();
    //le programme
    SDL_Quit();

}