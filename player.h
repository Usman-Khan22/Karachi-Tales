#pragma once
#include "raylib.h"

class Player {
    int lives = 5;
public:
    float x, y;
    float speed;
    Texture2D texture; // now assigned externally
    
    friend void ApplyEmergencyRepairs(Player& p);

    Player(float virtualWidth, float virtualHeight);
    ~Player();
    
    void draw();
    void moveLeft(float dt);
    void moveRight(float dt);

    void operator--(); 
    int getLives() { return lives; }
    void resetLives() { lives = 5; }

    Rectangle getRect();
};