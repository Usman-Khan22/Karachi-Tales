#include <raylib.h>
#include <raymath.h>

int main() {
    InitWindow(1500, 800, "BASE");

    Rectangle r1{100, 300, 100, 100};
    Rectangle r2{500, 500, 300, 200};
    Vector2 direction;
    float speed = 400;
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        direction.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
        direction.y = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);
        direction = Vector2Normalize(direction);

        r1.x += direction.x * speed * dt;
        r1.y += direction.y * speed * dt;

        if (CheckCollisionRecs(r1,r2)) {
            if (direction.x > 0) r1.x = r2.x - r1.width;
            else if (direction.x < 0) r1.x = r2.width + r2.x;
            else if (direction.y > 0) r1.y = r2.y - r1.height;
            else if (direction.y < 0) r1.y = r2.height + r2.y;
        }

        BeginDrawing();
        ClearBackground(WHITE);

        DrawRectangleRec(r1, GREEN);
        DrawRectangleRec(r2, RED);

        EndDrawing();
    }



    return 0;
}