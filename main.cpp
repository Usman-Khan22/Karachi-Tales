#include "raylib.h"
#include "game.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Karachi Tales: Rush Hour");
    SetTargetFPS(60);

    Game game(screenWidth, screenHeight);

    while (!WindowShouldClose()) {
        game.Update();
        game.Draw(screenWidth, screenHeight);
    }

    // game destructor automatically cleans up when it goes out of scope!
    CloseWindow();
    return 0;
}