#include "common.h"

#include "game.h"

int main() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_TOPMOST);
    InitWindow(
        SG_DEFAULT_SCREEN_W,
        SG_DEFAULT_SCREEN_H,
        SG_WINDOW_TITLE
    );
    SetTargetFPS(60);
     // disable exit key when not debugging
    if (!DEBUGGING) SetExitKey(0);

    SG_Game* g = SG_NewGame();

    while (!WindowShouldClose()) {
        BeginDrawing();
        SG_HandleInput(g);
        SG_Draw(g);
        EndDrawing();
    }

    SG_Quit(g);

    return 0;
}