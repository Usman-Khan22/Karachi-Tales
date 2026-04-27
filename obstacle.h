#pragma once
#include "raylib.h"

class Obstacle {
public:
    Rectangle rect;
    float speed;
    bool active;
    Texture2D texture;

    // We pass the texture and starting X position when we create it
    Obstacle(Texture2D tex, float startX, float startY, float size);
    
    void update(float dt);
    void draw();
};