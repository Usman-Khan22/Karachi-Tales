#include "raylib.h"
#include "player.h"
#include "obstacle.h"
#include <vector>

int main() {
    // --- 1. CONFIGURATION ---
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const int virtualWidth = 320;
    const int virtualHeight = 180;

    InitWindow(screenWidth, screenHeight, "Karachi Tales: Rush Hour");
    SetTargetFPS(60);

    // --- 2. ASSET LOADING ---
    RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    Texture2D roadTex = LoadTexture("images/road.png");
    Texture2D potholeTex = LoadTexture("images/pothole_24.png");
    Texture2D manholeTex = LoadTexture("images/manhole_32.png"); 
    
    Player player(virtualWidth, virtualHeight); 

    // --- 3. GAME STATE VARIABLES ---
    float scrollingBack = 0.0f;
    float scrollSpeed = 120.0f;
    float spawnTimer = 0.0f;
    float spawnInterval = 1.5f;
    std::vector<Obstacle> obstacles;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // --- 4. UPDATE LOGIC ---
        
        // Scroll Road
        scrollingBack += scrollSpeed * dt;
        if (scrollingBack >= virtualHeight) scrollingBack = 0;

        // Player Movement
        if (IsKeyDown(KEY_LEFT))  player.moveLeft(dt);
        if (IsKeyDown(KEY_RIGHT)) player.moveRight(dt);

        // Obstacle Spawning
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            float randomX = (float)GetRandomValue(61, 230); 

            // Randomly choose which type of obstacle to spawn
            int type = GetRandomValue(0, 1);
            if (type == 0) {
                // Spawn Pothole (24x24)
                obstacles.push_back(Obstacle(potholeTex, randomX, -30, 24));
            }
            else {
                // Spawn Manhole (32x32)
                // Note: Manholes are slightly bigger, so they'll be harder to dodge!
                obstacles.push_back(Obstacle(manholeTex, randomX, -30, 32));
            }
            spawnTimer = 0;
        }

        // Obstacle Movement & Cleanup
        for (int i = 0; i < obstacles.size(); i++) {
            obstacles[i].update(dt);
            if (!obstacles[i].active) {
                obstacles.erase(obstacles.begin() + i);
                i--; 
            }
        }

        // --- 5. DRAWING (VIRTUAL) ---
        BeginTextureMode(target);
            ClearBackground(BLACK);

            // Draw Road First (Background)
            DrawTexture(roadTex, 0, (int)scrollingBack, WHITE);
            DrawTexture(roadTex, 0, (int)scrollingBack - virtualHeight, WHITE);

            // Draw Obstacles (Middle ground)
            for (auto& obs : obstacles) {
                obs.draw();
            }

            // Draw Player (Foreground)
            player.draw();
        EndTextureMode();

        // --- 6. DRAWING (SCREEN UPSCALE) ---
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(target.texture, 
                (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                (Rectangle){ 0.0f, 0.0f, (float)screenWidth, (float)screenHeight },
                (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }

    // --- 7. CLEANUP ---
    UnloadTexture(roadTex);
    UnloadTexture(potholeTex);
    UnloadTexture(manholeTex);
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}