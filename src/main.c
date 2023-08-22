#include "raylib.h"

int main() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST);
    InitWindow(800, 600, "shooter game");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText("cocks", GetMouseX(), GetMouseY(), 20, BLACK);
        EndDrawing();        
    }

    return 0;
}