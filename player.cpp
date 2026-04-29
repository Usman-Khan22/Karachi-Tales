#include "player.h"

Player::Player(float virtualWidth, float virtualHeight) {
    x = virtualWidth / 2.0f;
    y = virtualHeight - 60;
    speed = 150.0f;
}

Player::~Player() {}

void Player::moveLeft(float dt) {
    x -= speed * dt;

    const float LEFT_BOUND = 61; 
    if (x < LEFT_BOUND) x = LEFT_BOUND;
}

void Player::moveRight(float dt) {
    x += speed * dt;

    const float RIGHT_BOUND = 259; 
    if (x > RIGHT_BOUND) x = RIGHT_BOUND;
}

void Player::draw() {
    DrawTexture(texture, (int)x - 16, (int)y, WHITE);
}

Rectangle Player::getRect() {
    return { x - 12, y + 5, 24, 40 }; 
}

void Player::operator--() {
    if (lives > 0) lives--;
}

void ApplyEmergencyRepairs(Player& p) {
    p.lives = 5; 
}