#include "game.h"

// raylib configuration and system setup functions
void Config() {
    srand(time(NULL));

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), WINDOW_TITLE);
    ToggleFullscreen();

    // disable exit key when not debugging
    if (!DEBUGGING) SetExitKey(0);
    // for dev mode, make sure cwd is shooter-game not build
    ChangeDirectory("..");
    SetTargetFPS(60);
}

// set up the game
void Init() {
    game.current_scene = SCENE_STARTUP;
    game.frame_count = 0;

    game.screen_size = (Vector2) {
        GetScreenWidth(),
        GetScreenHeight()
    };

    // init game objects
    DeleteObjects();

    // create camera
    game.camera = (Camera2D) {
        .target = { 0, 0 },
        .offset = { game.screen_size.x / 2, game.screen_size.y / 2 },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    // init textures and fonts
    game.textures = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, FreeTextureCallback);
    game.fonts = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, FreeFontCallback);

    CreateTexture("background", "assets/bg.png");

    LoadScene(SCENE_STARTSCREEN);
}

// one iteration of the game loop
// draws current scene
void Draw() {
    static void (*scene_draw_functions[])() = {
        [SCENE_STARTSCREEN] = DrawSceneStartScreen,
        [SCENE_GAMEPLAY] = DrawSceneGameplay,
        [SCENE_ENDSCREEN] = DrawSceneEndScreen
    };

    scene_draw_functions[game.current_scene]();
}

void Quit() {
    // raylib cleanup functions run here
    UnloadAssets();
}

void TileBackground() {
    Texture2D bg_texture = *GetTexture("background");
    int i0 = ((game.player->data.ent_data.pos.x - game.screen_size.x/2) / bg_texture.width) - 5;
    int i1 = ((game.player->data.ent_data.pos.x + game.screen_size.x/2) / bg_texture.width) + 5;
    int j0 = ((game.player->data.ent_data.pos.y - game.screen_size.y/2) / bg_texture.height) - 5;
    int j1 = ((game.player->data.ent_data.pos.y + game.screen_size.y/2) / bg_texture.height) + 5;

    ClearBackground(RAYWHITE);
    for (int i = i0; i < i1; i++) {
        for (int j = j0; j < j1; j++) {
            DrawTexture(
                bg_texture,
                i * bg_texture.width,
                j * bg_texture.height,
                (Color){255, 255, 255, 255}
            );
        }
    }
}

// gets called when hp = 0 or when they surrender
// runs the death animation and transitions to endscreen
void KillPlayer() {
    LoadScene(SCENE_ENDSCREEN);
}
