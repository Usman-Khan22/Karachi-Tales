#include "game.h"
#include <fstream>

Game::Game(int screenWidth, int screenHeight) : player(320, 180) {
    target = LoadRenderTexture(vWidth, vHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    
    LoadBestTime();

    // --- ASSETS ---
    roadTex = LoadTexture("images/road.png");
    potholeTex = LoadTexture("images/pothole_24.png");
    manholeTex = LoadTexture("images/manhole_32.png");
    rickshawTex = LoadTexture("images/rickshaw.png");
    carTex = LoadTexture("images/car.png");
    heartTex = LoadTexture("images/heart.png"); 
    chappalTex = LoadTexture("images/chappal.png");
    bikerTex = LoadTexture("images/biker.png"); // ✅ moved here

    player.texture = bikerTex; // ✅ pass to player

    lives = 5;
    gameTimer = 120.0f; 
    distance = 0;
    targetDistance = 50.0f;
    gameOver = false;
    gameWon = false;
    endState = 0;
    chappalRotation = 0.0f;
}

Game::~Game() {
    UnloadTexture(roadTex);
    UnloadTexture(potholeTex);
    UnloadTexture(manholeTex);
    UnloadTexture(rickshawTex);
    UnloadTexture(carTex);
    UnloadTexture(heartTex);
    UnloadTexture(chappalTex);
    UnloadTexture(bikerTex); // ✅ added
    UnloadRenderTexture(target);

    for (auto obs : obstacles) delete obs;
}

void Game::SpawnObstacle() {
    float rx = (float)GetRandomValue(ROAD_LEFT + 5, ROAD_RIGHT - 30); // ✅ fixed
    int type = GetRandomValue(0, 3);

    if (type == 0) obstacles.push_back(new Hazard(potholeTex, rx, -30, 24, scrollSpeed));
    else if (type == 1) obstacles.push_back(new Hazard(manholeTex, rx, -30, 32, scrollSpeed));
    else if (type == 2) obstacles.push_back(new Vehicle(rickshawTex, rx, -60, 32, scrollSpeed, 60.0f));
    else obstacles.push_back(new Vehicle(carTex, rx, -80, 32, scrollSpeed, -40.0f));
}

void Game::Update() {
    if (gameOver || gameWon) {
        if (gameWon) SaveBestTime();

        if (gameOver) chappalRotation += 200.0f * GetFrameTime(); 
        
        if (IsKeyPressed(KEY_R)) {
            lives = 5;
            gameTimer = 120.0f;
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


    spawnInterval = 1.5f - (distance * 0.02f); 
    if (spawnInterval < 0.6f) spawnInterval = 0.6f;

    distance += (scrollSpeed * dt) / 100.0f; 
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

    scrollingBack += scrollSpeed * dt;
    if (scrollingBack >= vHeight) scrollingBack = 0;

    if (IsKeyDown(KEY_LEFT))  player.moveLeft(dt);
    if (IsKeyDown(KEY_RIGHT)) player.moveRight(dt);

    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        SpawnObstacle();
        spawnTimer = 0;
    }

    for (int i = 0; i < (int)obstacles.size(); i++) {
        obstacles[i]->update(dt);

        if (CheckCollisionRecs(player.getRect(), obstacles[i]->getRect())) {
            lives--; 
            gameTimer -= 10.0f;
            if (gameTimer < 0) gameTimer = 0;

            delete obstacles[i];
            obstacles.erase(obstacles.begin() + i);
            i--;

            if (lives <= 0) {
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

void Game::Draw(int screenWidth, int screenHeight) {
    BeginTextureMode(target);
        ClearBackground(BLACK);
        
        DrawTexture(roadTex, 0, (int)scrollingBack, WHITE);
        DrawTexture(roadTex, 0, (int)scrollingBack - vHeight, WHITE);

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
    // --- DRAW BEST TIME ---
    const char* bestTimeStr;
    if (bestTime > 900.0f) {
        bestTimeStr = "BEST: --:--";
    } else {
        int bMin = (int)bestTime / 60;
        int bSec = (int)bestTime % 60;
        bestTimeStr = TextFormat("BEST: %02d:%02d", bMin, bSec);
    }
    int bestWidth = MeasureText(bestTimeStr, 10);
    DrawText(bestTimeStr, vWidth - bestWidth - 10, 5, 10, GOLD);

    // --- DISTANCE ---
    float distLeft = targetDistance - distance;
    if (distLeft < 0) distLeft = 0;
    DrawText("TO HOME:", 10, (vHeight / 2) - 25, 8, GRAY);
    DrawText(TextFormat("%.1f km", distLeft), 10, (vHeight / 2) - 15, 10, RAYWHITE);

    // --- TIMER ---
    int minutes = (int)gameTimer / 60;
    int seconds = (int)gameTimer % 60;
    const char* timeStr = TextFormat("%02d:%02d", minutes, seconds);
    int textWidth = MeasureText(timeStr, 10);
    DrawText(timeStr, (vWidth / 2) - (textWidth / 2), 5, 10, RAYWHITE);

    // --- HEARTS ---
    for (int i = 0; i < lives; i++) {
        DrawTexture(heartTex, 10 + (i * (heartTex.width + 4)), 5, WHITE);
    }

    // --- LOSS SCREEN ---
    if (gameOver) {
        DrawRectangle(0, 0, vWidth, vHeight, Fade(BLACK, 0.85f));

        Rectangle source = { 0, 0, (float)chappalTex.width, (float)chappalTex.height };
        Rectangle dest = { vWidth / 2.0f, vHeight / 2.0f - 40, 40, 40 }; 
        Vector2 origin = { 20, 20 }; 
        DrawTexturePro(chappalTex, source, dest, origin, chappalRotation, WHITE);

        int wastedW = MeasureText("WASTED", 30);
        DrawText("WASTED", (vWidth / 2) - (wastedW / 2), (vHeight / 2) - 10, 30, RED);

        const char* ammiMsg = (endState == 1) ? "BIKE TOOT GAYI!" : "LATE HO GAYE!";
        int ammiW = MeasureText(ammiMsg, 10);
        DrawText(ammiMsg, (vWidth / 2) - (ammiW / 2), (vHeight / 2) + 20, 10, RAYWHITE);
        DrawText("AMMI IS ANGRY", (vWidth / 2) - 35, (vHeight / 2) + 32, 8, ORANGE);
        DrawText("Press R to Restart", (vWidth / 2) - 40, vHeight - 20, 8, GRAY);
    }

    // --- WIN SCREEN ---
    if (gameWon) {
        DrawRectangle(0, 0, vWidth, vHeight, Fade(DARKGREEN, 0.8f));
        DrawText("MERA BETA AA GAYA!", vWidth/2 - 55, vHeight/2 - 10, 10, RAYWHITE);
        DrawText("Dinner is ready!", vWidth/2 - 35, vHeight/2 + 5, 8, GREEN);
        
        float timeTaken = 120.0f - gameTimer;
        if (timeTaken <= bestTime) {
            DrawText("NEW RECORD TIME!", vWidth/2 - 40, vHeight/2 + 20, 8, GOLD);
        }
        
        DrawText("Press R to play again", vWidth/2 - 45, vHeight - 20, 8, GRAY);
    }
}

void Game::LoadBestTime() {
    std::ifstream file("besttime.txt");
    if (file.is_open()) {
        if (!(file >> bestTime)) { // ✅ safe read
            bestTime = 999.0f;
        }
        file.close();
    } else {
        bestTime = 999.0f;
    }
}

void Game::SaveBestTime() {
    float timeTaken = 120.0f - gameTimer;
    if (timeTaken < bestTime) {
        bestTime = timeTaken;
        std::ofstream file("besttime.txt");
        if (file.is_open()) {
            file << bestTime;
            file.close();
        }
    }
}