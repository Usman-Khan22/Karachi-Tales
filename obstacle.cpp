#include "obstacle.h"

// Base Obstacle Constructor
Obstacle::Obstacle(Texture2D tex, float x, float y, float size, float s) 
    : texture(tex), speed(s), active(true) {
    rect = { x, y, size, size };
}

void Obstacle::draw() {
    DrawTexture(texture, (int)rect.x, (int)rect.y, WHITE);
}

// Hazard Update
void Hazard::update(float dt) {
    rect.y += speed * dt;
    if (rect.y > 180) active = false;
}

// Vehicle Update
void Vehicle::update(float dt) {
    rect.y += speed * dt;
    // Vehicles can go off the bottom OR the top if they are slow
    if (rect.y > 200 || rect.y < -100) active = false;
}