#pragma once
#include "raylib.h"

class Obstacle {
protected:
    Texture2D texture;
    Rectangle rect;
    float speed;

public:
    bool active;
    Obstacle(Texture2D tex, float x, float y, float size, float s);
    virtual ~Obstacle() {} 

    virtual void update(float dt) = 0;
    virtual void draw();
    virtual Rectangle getRect();
    
};

class Hazard : public Obstacle {
public:
    Hazard(Texture2D tex, float x, float y, float size, float s) : Obstacle(tex, x, y, size, s) {}
    
    void update(float dt) override;
};

class Vehicle : public Obstacle {
public:
    Vehicle(Texture2D tex, float x, float y, float size, float roadSpeed, float extraSpeed) 
        : Obstacle(tex, x, y, size, roadSpeed + extraSpeed) {}
    
    void update(float dt) override;
    Rectangle getRect() override;
};