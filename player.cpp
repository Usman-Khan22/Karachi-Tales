#include "player.h"

Player::Player(float virtualWidth, float virtualHeight) {
    x = virtualWidth / 2.0f; // Start in the middle
    y = virtualHeight - 60;
    speed = 150.0f; // How fast the bike moves side-to-side
    texture = LoadTexture("images/biker.png");
}

Player::~Player() {
    UnloadTexture(texture);
}

void Player::moveLeft(float dt) {
    x -= speed * dt;
    
    // Boundary Check: Left Footpath (50px + half of bike width)
    if (x < 16+45) x = 16 + 45; 
}

void Player::moveRight(float dt) {
    x += speed * dt;
    
    // Boundary Check: Right Footpath (320px - 50px - half of bike width)
    if (x > 320-45-16) x = 320-45-16;
}

void Player::draw() {
    // Center the 32px texture on the X coordinate
    DrawTexture(texture, (int)x - 16, (int)y, WHITE);
}

Rectangle Player::getRect() {
        // x-8 and width 16 makes the hitbox perfectly centered on the bike's body, 
        // ignoring the wide handles.
        return { x - 8, y + 5, 16, 40 }; 
    }