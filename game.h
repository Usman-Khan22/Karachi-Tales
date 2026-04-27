#pragma once
#include "raylib.h"
#include "player.h"
#include "obstacle.h"
#include <vector>

class Game {
private:
    // Virtual Canvas & Resolution
    RenderTexture2D target;
    const int vWidth = 320;
    const int vHeight = 180;

    // Assets
    Texture2D roadTex;
    Texture2D potholeTex;
    Texture2D manholeTex;
    Texture2D rickshawTex;
    Texture2D carTex;

    // Game Objects
    Player player;
    std::vector<Obstacle*> obstacles;

    // State Variables
    float scrollingBack = 0;
    float scrollSpeed = 120.0f;
    float spawnTimer = 0;
    float spawnInterval = 1.5f;

    // Private helper methods
    void SpawnObstacle();
    void HandleInput(float dt);

public:
    Game(int screenWidth, int screenHeight);
    ~Game(); // Handles all UnloadTexture calls

    bool IsRunning();
    void Update();
    void Draw(int screenWidth, int screenHeight);
};