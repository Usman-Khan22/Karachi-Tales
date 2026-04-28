#pragma once
#include "raylib.h"

class Player {
public:
    float x, y;
    float speed;
    Texture2D texture; // now assigned externally

    Player(float virtualWidth, float virtualHeight);
    ~Player();
    
    void draw();
    void moveLeft(float dt);
    void moveRight(float dt);

    Rectangle getRect();
};