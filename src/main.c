#include "common.h"

#include "game.h"

int main() {

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_RESIZABLE);
    InitWindow(
        SG_DEFAULT_SCREEN_W,
        SG_DEFAULT_SCREEN_H,
        SG_WINDOW_TITLE
    );
    SetTargetFPS(60);

    // disable exit key when not debugging
    if (!DEBUGGING) SetExitKey(0);
    // for dev mode, make sure cwd is shooter-game not build
    ChangeDirectory("..");

    SG_Game* g = SG_NewGame();

    while (!WindowShouldClose()) {
        SG_Draw(g);
    }

    SG_Quit(g);

    return 0;
}