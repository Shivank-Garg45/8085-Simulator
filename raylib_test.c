#include "raylib.h"

int main() {
    InitWindow(800, 450, "8085 Simulator");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("8085 Simulator", 250, 180, 40, BLACK);
        DrawText("Raylib is working!", 280, 240, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
