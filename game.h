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

    // ✅ NEW: Layout constants (replacing magic numbers)
    const int ROAD_LEFT = 45;
    const int ROAD_RIGHT = 320 - 45;

    // Assets
    Texture2D roadTex;
    Texture2D potholeTex;
    Texture2D manholeTex;
    Texture2D rickshawTex;
    Texture2D carTex;
    Texture2D heartTex;
    Texture2D chappalTex;
    Texture2D bikerTex; // ✅ moved from Player

    // Game Objects
    Player player;
    std::vector<Obstacle*> obstacles;

    // State Variables
    float scrollingBack = 0;
    float scrollSpeed = 140.0f;
    float spawnTimer = 0;
    float spawnInterval = 1.5f;
    
    // Game Logic Variables
    bool gameOver = false;
    bool gameWon = false;
    float gameTimer = 100.0f; 
    int lives = 5;
    float distance = 0;
    float targetDistance = 100.0f; 
    int endState = 0;
    bool showAmmiWarning = false;
    float warningTimer = 0.0f; // To make the warning flash or disappear
    
    // UI & Persistence
    float chappalRotation = 0.0f;
    float bestTime = 999.0f;

    void SpawnObstacle();
    void DrawUI();
    void LoadBestTime();
    void SaveBestTime();

public:
    Game(int screenWidth, int screenHeight);
    ~Game(); 

    void Update();
    void Draw(int screenWidth, int screenHeight);
};