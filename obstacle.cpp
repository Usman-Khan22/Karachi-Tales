#pragma once
#include "raylib.h"

class Obstacle {
protected:
    Texture2D texture;
    Rectangle rect;
    float speed;

public:
    bool active;

    Obstacle(Texture2D tex, float x, float y, float size, float s) 
        : texture(tex), speed(s), active(true) {
        rect = { x, y, size, size };
    }

    // Virtual Destructor is essential for inheritance
    virtual ~Obstacle() {}

    // Pure virtual function (making the class abstract)
    virtual void update(float dt) = 0; 

    virtual void draw() {
        DrawTexture(texture, (int)rect.x, (int)rect.y, WHITE);
    }

    // Getter for collision detection later
    Rectangle getRect() { return rect; }
};

class Hazard : public Obstacle {
public:
    using Obstacle::Obstacle; // Inherit the constructor

    void update(float dt) override {
        rect.y += speed * dt; // Just moves with the road
        if (rect.y > 180) active = false;
    }
};

class Vehicle : public Obstacle {
private:
    float relativeSpeed;
public:
    Vehicle(Texture2D tex, float x, float y, float size, float roadSpeed, float extraSpeed) 
        : Obstacle(tex, x, y, size, roadSpeed + extraSpeed) {}

    void update(float dt) override {
        rect.y += speed * dt; 
        if (rect.y > 180 || rect.y < -100) active = false;
    }
};