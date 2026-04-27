#pragma once
#include "raylib.h"

// --- BASE CLASS ---
class Obstacle {
protected:
    Texture2D texture;
    Rectangle rect;
    float speed;

public:
    bool active;
    Obstacle(Texture2D tex, float x, float y, float size, float s);
    virtual ~Obstacle() {} // Required for OOP inheritance

    virtual void update(float dt) = 0; // Pure virtual = Abstract class
    virtual void draw();
};

// --- CHILD CLASS 1: Static Hazards ---
class Hazard : public Obstacle {
public:
    // This tells Hazard to use the Obstacle constructor
    Hazard(Texture2D tex, float x, float y, float size, float s) : Obstacle(tex, x, y, size, s) {}
    
    void update(float dt) override;
};

// --- CHILD CLASS 2: Moving Vehicles ---
class Vehicle : public Obstacle {
public:
    Vehicle(Texture2D tex, float x, float y, float size, float roadSpeed, float extraSpeed) 
        : Obstacle(tex, x, y, size, roadSpeed + extraSpeed) {}
    
    void update(float dt) override;
};