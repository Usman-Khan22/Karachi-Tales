#include "game.h"

Game::Game(int screenWidth, int screenHeight) : player(320, 180) {
    target = LoadRenderTexture(vWidth, vHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    // Load all textures here
    roadTex = LoadTexture("images/road.png");
    potholeTex = LoadTexture("images/pothole_24.png");
    manholeTex = LoadTexture("images/manhole_32.png");
    rickshawTex = LoadTexture("images/rickshaw_wrongway.png");
    carTex = LoadTexture("images/car.png");
}

Game::~Game() {
    UnloadTexture(roadTex);
    UnloadTexture(potholeTex);
    UnloadTexture(manholeTex);
    UnloadTexture(rickshawTex);
    UnloadTexture(carTex);
    UnloadRenderTexture(target);

    for (auto obs : obstacles) delete obs; // Clean up memory
}

void Game::SpawnObstacle() {
    float rx = (float)GetRandomValue(61, 230);
    int type = GetRandomValue(0, 3);

    if (type == 0)      obstacles.push_back(new Hazard(potholeTex, rx, -30, 24, scrollSpeed));
    else if (type == 1) obstacles.push_back(new Hazard(manholeTex, rx, -30, 32, scrollSpeed));
    else if (type == 2) obstacles.push_back(new Vehicle(rickshawTex, rx, -60, 32, scrollSpeed, 60.0f)); // Fast!
    else                obstacles.push_back(new Vehicle(carTex, rx, -80, 32, scrollSpeed, -40.0f)); // Slow traffic
}

void Game::Update() {
    float dt = GetFrameTime();

    // 1. Scroll Road
    scrollingBack += scrollSpeed * dt;
    if (scrollingBack >= vHeight) scrollingBack = 0;

    // 2. Handle Player
    if (IsKeyDown(KEY_LEFT))  player.moveLeft(dt);
    if (IsKeyDown(KEY_RIGHT)) player.moveRight(dt);

    // 3. Spawning Logic
    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        SpawnObstacle();
        spawnTimer = 0;
    }

    // 4. Update Obstacles & Cleanup
    for (int i = 0; i < (int)obstacles.size(); i++) {
        obstacles[i]->update(dt);
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
        // Draw Road
        DrawTexture(roadTex, 0, (int)scrollingBack, WHITE);
        DrawTexture(roadTex, 0, (int)scrollingBack - vHeight, WHITE);
        
        for (auto obs : obstacles) obs->draw();
        player.draw();
    EndTextureMode();

    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(target.texture, 
            (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height },
            (Rectangle){ 0, 0, (float)screenWidth, (float)screenHeight },
            (Vector2){ 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}