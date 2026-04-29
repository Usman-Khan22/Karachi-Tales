#include "game.h"
#include <fstream>
#include <algorithm> 

Game::Game(int screenWidth, int screenHeight) : player(320, 180) {
    target = LoadRenderTexture(vWidth, vHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    
    LoadBestTime();

    // --- ASSETS LOADING ---
    roadTex     = LoadTexture("images/road.png");
    potholeTex  = LoadTexture("images/pothole_24.png");
    manholeTex  = LoadTexture("images/manhole_32.png");
    rickshawTex = LoadTexture("images/rickshaw.png");
    carTex      = LoadTexture("images/car.png");
    heartTex    = LoadTexture("images/heart.png"); 
    chappalTex  = LoadTexture("images/chappal.png");
    bikerTex    = LoadTexture("images/biker.png");

    player.texture = bikerTex; 

    // --- INITIAL STATE ---
    lives = 5;
    gameTimer = 100.0f;      // SET TO 100 SECONDS
    distance = 0;
    targetDistance = 100.0f; // Goal: 100km
    gameOver = false;
    gameWon = false;
    endState = 0;
    chappalRotation = 0.0f;
    spawnInterval = 1.5f;
    scrollSpeed = 140.0f;    // Set base speed
}

Game::~Game() {
    UnloadTexture(roadTex);
    UnloadTexture(potholeTex);
    UnloadTexture(manholeTex);
    UnloadTexture(rickshawTex);
    UnloadTexture(carTex);
    UnloadTexture(heartTex);
    UnloadTexture(chappalTex);
    UnloadTexture(bikerTex); 
    UnloadRenderTexture(target);

    for (auto obs : obstacles) delete obs;
}

void Game::SpawnObstacle() {
    float rx = (float)GetRandomValue(ROAD_LEFT + 5, ROAD_RIGHT - 30); 
    int type = GetRandomValue(0, 3);

    if (type == 0)      obstacles.push_back(new Hazard(potholeTex, rx, -30, 24, scrollSpeed));
    else if (type == 1) obstacles.push_back(new Hazard(manholeTex, rx, -30, 32, scrollSpeed));
    else if (type == 2) obstacles.push_back(new Vehicle(rickshawTex, rx, -60, 32, scrollSpeed, 60.0f));
    else                obstacles.push_back(new Vehicle(carTex, rx, -80, 32, scrollSpeed, -40.0f));
}

void Game::Update() {
    if (gameOver || gameWon) {
        if (gameWon) SaveBestTime();
        if (gameOver) chappalRotation += 200.0f * GetFrameTime(); 
        
        if (IsKeyPressed(KEY_R)) {
            player.resetLives();
            gameTimer = 100.0f; // Reset to 100s
            distance = 0;
            spawnInterval = 1.5f; 
            spawnTimer = 0.0f;
            gameOver = false;
            gameWon = false;
            endState = 0;
            chappalRotation = 0.0f;
            for (auto obs : obstacles) delete obs;
            obstacles.clear();
        }
        return;
    }

    float dt = GetFrameTime();

    // --- 2. MOVEMENT & BOOST LOGIC ---
    float boostMultiplier = 1.0f;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        boostMultiplier = 1.7f; // BOOST VALUE: 1.7
    }
    float currentSpeed = scrollSpeed * boostMultiplier;

    // --- 3. DIFFICULTY & PROGRESSION ---
    distance += (currentSpeed * dt) / 126.0f; // DIVISOR: 126

    // SPAWN MULTIPLIER: 0.015 (Aggressive ramp-up)
    spawnInterval = std::max(0.6f, 1.5f - (distance * 0.015f)); 

    if (distance >= targetDistance) {
        gameWon = true;
        endState = 3;
    }

    gameTimer -= dt;
    if (gameTimer <= 0) {
        gameTimer = 0;
        gameOver = true;
        endState = 2;
    }

    scrollingBack += currentSpeed * dt;
    if (scrollingBack >= vHeight) scrollingBack = 0;

    if (IsKeyDown(KEY_LEFT))  player.moveLeft(dt);
    if (IsKeyDown(KEY_RIGHT)) player.moveRight(dt);

    // --- 4. SPAWNING ---
    spawnTimer += dt;
    if (IsKeyPressed(KEY_C)) {
        ApplyEmergencyRepairs(player);
    }
    if (spawnTimer >= spawnInterval) {
        SpawnObstacle();
        spawnTimer = 0;
    }

    // --- 5. COLLISION & CLEANUP ---
    for (int i = 0; i < (int)obstacles.size(); i++) {
        obstacles[i]->update(dt * boostMultiplier);

        if (CheckCollisionRecs(player.getRect(), obstacles[i]->getRect())) {
            --player; 
            gameTimer -= 10.0f; // Crash penalty
            if (gameTimer < 0) gameTimer = 0;

            delete obstacles[i];
            obstacles.erase(obstacles.begin() + i);
            i--;

            if (player.getLives() <= 0) { // Using Getter
                gameOver = true;
                endState = 1; 
            }
            continue; 
        }


        if (!obstacles[i]->active) {
            delete obstacles[i];
            obstacles.erase(obstacles.begin() + i);
            i--;
        }
    }
}

// ... Keep your Draw and DrawUI as they were, they work with the logic above!


void Game::Draw(int screenWidth, int screenHeight) {
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    // Road Background
    DrawTexture(roadTex, 0, (int)scrollingBack, WHITE);
    DrawTexture(roadTex, 0, (int)scrollingBack - vHeight, WHITE);
    
    // Objects
    for (auto obs : obstacles) obs->draw();
    player.draw();
    
    DrawUI();
    EndTextureMode();
    
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(target.texture, 
        (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height },
        (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight },
        (Vector2){ 0, 0 }, 0.0f, WHITE);
    EndDrawing();
    }
    
    void Game::DrawUI() {
        // --- BEST TIME ---
        const char* bestTimeStr;
        if (bestTime > 900.0f) bestTimeStr = "BEST: --:--";
        else {
            int bMin = (int)bestTime / 60;
            int bSec = (int)bestTime % 60;
            bestTimeStr = TextFormat("BEST: %02d:%02d", bMin, bSec);
        }
        int bestWidth = MeasureText(bestTimeStr, 10);
        DrawText(bestTimeStr, vWidth - bestWidth - 10, 5, 10, GOLD);
        
        // --- PROGRESS ---
        float distLeft = std::max(0.0f, targetDistance - distance);
        DrawText("TO HOME:", 10, (vHeight / 2) - 25, 8, GRAY);
        DrawText(TextFormat("%.1f km", distLeft), 10, (vHeight / 2) - 15, 10, RAYWHITE);
        
        // --- HEARTS ---
        for (int i = 0; i < player.getLives(); i++) {
            DrawTexture(heartTex, 8 + (i * (heartTex.width + 2)), 5, WHITE);
        }
        
        // --- MAGHRIB TIMER ---
        int mins = (int)gameTimer / 60;
        int secs = (int)gameTimer % 60;
        DrawText("MAGHRIB IN:", (vWidth / 2) - 25, 2, 6, ORANGE); 
    
    Color tColor = (gameTimer < 20.0f && (int)(GetTime() * 5) % 2 == 0) ? RED : RAYWHITE;
    DrawText(TextFormat("%02d:%02d", mins, secs), (vWidth / 2) - 15, 10, 10, tColor);

    // --- HUMOR POPUPS ---
    if (gameTimer < 60.0f && !gameWon && !gameOver) {
        DrawRectangle(10, vHeight - 20, 85, 10, Fade(RED, 0.6f));
        DrawText("📞 99 Missed Calls: AMMI", 15, vHeight - 17, 5, WHITE);
    }
    
    if (gameTimer < 30.0f && gameTimer > 0 && !gameWon && !gameOver) {
        const char* msg = "AMMI IS SEARCHING FOR HER CHAPPAL!";
        int msgW = MeasureText(msg, 8);
        if ((int)(GetTime() * 2) % 2 == 0) {
            DrawText(msg, (vWidth / 2) - (msgW / 2), vHeight - 40, 8, YELLOW);
        }
    }
    
    // --- SCREENS ---
    if (gameOver) {
        DrawRectangle(0, 0, vWidth, vHeight, Fade(BLACK, 0.85f));
        Rectangle src = { 0, 0, (float)chappalTex.width, (float)chappalTex.height };
        Rectangle dst = { vWidth / 2.0f, vHeight / 2.0f - 40, 40, 40 }; 
        DrawTexturePro(chappalTex, src, dst, (Vector2){20, 20}, chappalRotation, WHITE);
        
        DrawText("WASTED", (vWidth / 2) - (MeasureText("WASTED", 30) / 2), (vHeight / 2) - 10, 30, RED);
        
        const char* ammiMsg = (endState == 1) ? "BIKE TOOT GAYI!" : "AZAAN HO GAYI! AB DEKHO...";
        DrawText(ammiMsg, (vWidth / 2) - (MeasureText(ammiMsg, 10) / 2), (vHeight / 2) + 20, 10, RAYWHITE);
        DrawText("AMMI IS ANGRY", (vWidth / 2) - 35, (vHeight / 2) + 32, 8, ORANGE);
        DrawText("Press R to Restart", (vWidth / 2) - 40, vHeight - 20, 8, GRAY);
    }
    
    if (gameWon) {
        DrawRectangle(0, 0, vWidth, vHeight, Fade(DARKGREEN, 0.8f));
        DrawText("MERA BETA AA GAYA!", (vWidth/2) - 55, (vHeight/2) - 10, 10, RAYWHITE);
        DrawText("Dinner is ready!", (vWidth/2) - 35, (vHeight/2) + 5, 8, GREEN);
        
        if ((120.0f - gameTimer) <= bestTime) DrawText("NEW RECORD TIME!", (vWidth/2) - 40, (vHeight/2) + 20, 8, GOLD);
        DrawText("Press R to play again", (vWidth/2) - 45, vHeight - 20, 8, GRAY);
    }
}

void Game::SaveBestTime() {
    float timeTaken = 100.0f - gameTimer; // Adjusted for 100s timer
    if (timeTaken < bestTime) {
        bestTime = timeTaken;
        std::ofstream file("besttime.txt");
        if (file.is_open()) {
            file << bestTime;
            file.close();
        }
    }
}
void Game::LoadBestTime() {
    std::ifstream file("besttime.txt");
    if (file.is_open()) {
        if (!(file >> bestTime)) bestTime = 999.0f;
        file.close();
    } else bestTime = 999.0f;
}

