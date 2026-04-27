#pragma once
#include "raylib.h"

class Player {
public:
    float x, y;
    float speed;
    Texture2D texture;

    Player(float virtualWidth, float virtualHeight);
    ~Player();
    
    void draw();
    
    // Updated movement functions
    void moveLeft(float dt);
    void moveRight(float dt);
};