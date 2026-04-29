#include "raylib.h"
#include "game.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Karachi Tales: Rush Hour");
    SetTargetFPS(60);
    SetExitKey(KEY_ESCAPE);
    Game game(screenWidth, screenHeight);

    while (!WindowShouldClose()) {
        game.Update();
        game.Draw(screenWidth, screenHeight);
    }

    CloseWindow();
    return 0;
}