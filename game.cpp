#include "game.h"

Game::Game(int screenWidth, int screenHeight) : player(320, 180) {
    target = LoadRenderTexture(vWidth, vHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    // --- ASSET LOADING ---
    roadTex = LoadTexture("images/road.png");
    potholeTex = LoadTexture("images/pothole_24.png");
    manholeTex = LoadTexture("images/manhole_32.png");
    rickshawTex = LoadTexture("images/rickshaw.png");
    carTex = LoadTexture("images/car.png");
    heartTex = LoadTexture("images/heart.png"); 
    chappalTex = LoadTexture("images/chappal.png");

    // --- INITIAL STATE ---
    lives = 5;
    gameTimer = 120.0f; 
    distance = 0;
    targetDistance = 50.0f; // Goal in km
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
    UnloadRenderTexture(target);

    for (auto obs : obstacles) delete obs;
}

void Game::SpawnObstacle() {
    float rx = (float)GetRandomValue(61, 230);
    int type = GetRandomValue(0, 3);

    if (type == 0)      obstacles.push_back(new Hazard(potholeTex, rx, -30, 24, scrollSpeed));
    else if (type == 1) obstacles.push_back(new Hazard(manholeTex, rx, -30, 32, scrollSpeed));
    else if (type == 2) obstacles.push_back(new Vehicle(rickshawTex, rx, -60, 32, scrollSpeed, 60.0f));
    else                obstacles.push_back(new Vehicle(carTex, rx, -80, 32, scrollSpeed, -40.0f));
}

void Game::Update() {
    // 1. RESTART & FAILURE LOGIC
    if (gameOver || gameWon) {
        if (gameOver) chappalRotation += 200.0f * GetFrameTime(); 
        
        if (IsKeyPressed(KEY_R)) {
            lives = 5;
            gameTimer = 120.0f;
            distance = 0;
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

    // 2. DISTANCE & WIN CHECK
    distance += (scrollSpeed * dt) / 100.0f; 
    if (distance >= targetDistance) {
        gameWon = true;
        endState = 3; // WIN
    }

    // 3. TIMER CHECK
    gameTimer -= dt;
    if (gameTimer <= 0) {
        gameTimer = 0;
        gameOver = true;
        endState = 2; // TIME OUT
    }

    // 4. WORLD SCROLLING
    scrollingBack += scrollSpeed * dt;
    if (scrollingBack >= vHeight) scrollingBack = 0;

    // 5. PLAYER INPUT
    if (IsKeyDown(KEY_LEFT))  player.moveLeft(dt);
    if (IsKeyDown(KEY_RIGHT)) player.moveRight(dt);

    // 6. SPAWNING
    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        SpawnObstacle();
        spawnTimer = 0;
    }

    // 7. OBSTACLE & COLLISION LOOP
    for (int i = 0; i < (int)obstacles.size(); i++) {
        obstacles[i]->update(dt);

        if (CheckCollisionRecs(player.getRect(), obstacles[i]->getRect())) {
            lives--; 
            delete obstacles[i];
            obstacles.erase(obstacles.begin() + i);
            i--;

            if (lives <= 0) {
                gameOver = true;
                endState = 1; // CRASHED
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

void Game::DrawUI() {
    // --- DISTANCE LEFT (Left side, above center) ---
    float distLeft = targetDistance - distance;
    if (distLeft < 0) distLeft = 0;
    DrawText("TO HOME:", 10, (vHeight / 2) - 25, 8, GRAY);
    DrawText(TextFormat("%.1f km", distLeft), 10, (vHeight / 2) - 15, 10, RAYWHITE);

    // --- DRAW TIMER (Top Center) ---
    int minutes = (int)gameTimer / 60;
    int seconds = (int)gameTimer % 60;
    const char* timeStr = TextFormat("%02d:%02d", minutes, seconds);
    int textWidth = MeasureText(timeStr, 10);
    DrawText(timeStr, (vWidth / 2) - (textWidth / 2), 5, 10, RAYWHITE);

    // --- DRAW HEARTS (Top Left) ---
    for (int i = 0; i < lives; i++) {
        DrawTexture(heartTex, 10 + (i * (heartTex.width + 4)), 5, WHITE);
    }

    // --- LOSS SCREEN (WASTED) ---
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
        DrawText("Press R to play again", vWidth/2 - 45, vHeight - 20, 8, GRAY);
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