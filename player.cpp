#include "player.h"

Player::Player(float virtualWidth, float virtualHeight) {
    x = virtualWidth / 2.0f;
    y = virtualHeight - 60;
    speed = 150.0f;
}

Player::~Player() {}

void Player::moveLeft(float dt) {
    x -= speed * dt;

    const float LEFT_BOUND = 61; // ✅ clean boundary
    if (x < LEFT_BOUND) x = LEFT_BOUND;
}

void Player::moveRight(float dt) {
    x += speed * dt;

    const float RIGHT_BOUND = 259; // ✅ clean boundary
    if (x > RIGHT_BOUND) x = RIGHT_BOUND;
}

void Player::draw() {
    DrawTexture(texture, (int)x - 16, (int)y, WHITE);
}

Rectangle Player::getRect() {
    // x - 12 (instead of 8) makes the biker "wider" to the physics engine
    // width 24 (instead of 16) matches the visual width better
    return { x - 12, y + 5, 24, 40 }; 
}

void Player::operator--() {
    if (lives > 0) lives--;
    // You could even trigger a sound effect or flash red here
}

void ApplyEmergencyRepairs(Player& p) {
    // Because it's a friend, it can access 'p.lives' directly
    // even though 'lives' is private!
    p.lives = 5; 
}