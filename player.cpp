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
    return { x - 8, y + 5, 16, 40 }; 
}