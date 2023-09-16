#include "game.h"

// raylib configuration and system setup functions
void Config() {
    srand(time(NULL));

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_VSYNC_HINT
        | FLAG_MSAA_4X_HINT
        | FLAG_WINDOW_RESIZABLE
        // | FLAG_WINDOW_UNDECORATED
        | FLAG_WINDOW_TOPMOST);
    InitWindow(GetScreenWidth(), GetScreenHeight(), WINDOW_TITLE);
    game.current_monitor = GetCurrentMonitor();
    game.screen_size = (Vector2) {
        GetMonitorWidth(game.current_monitor),
        GetMonitorHeight(game.current_monitor)};\

    DevPrintf("Detected monitor size: (%d, %d)",
        (int) game.screen_size.x,
        (int) game.screen_size.y);

    SetWindowSize((int) game.screen_size.x, (int) game.screen_size.y);
    DevPrintf("Set monitor size to (%d, %d) fullscreen",
        (int) game.screen_size.x,
        (int) game.screen_size.y);

    // disable exit key 
    // SetExitKey(0);
    // make sure cwd is shooter-game not build
    ChangeDirectory("..");
    SetTargetFPS(60);
}

// set up the game
void Init() {
    game.current_scene = SCENE_STARTUP;
    game.paused = false;

    // init data structures
    game.textures = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, MemFreeTextureCallback);
    game.fonts = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, MemFreeFontCallback);
    game.keybinds = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, free);
    game.shaders = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, MemFreeShaderCallback);

    // load assets

    CreateTexture("healthbar_1", "assets/images/healthbar_1.png");
    CreateTexture("healthbar_2", "assets/images/healthbar_2.png");
    CreateTexture("healthbar_3", "assets/images/healthbar_3.png");
    CreateTexture("healthbar_4", "assets/images/healthbar_4.png");
    CreateTexture("healthbar_5", "assets/images/healthbar_5.png");
    CreateTexture("background", "assets/images/bg_new_new.png");

    // TODO write these shaders to make gaussian blur on the pause screen

    CreateShader("pause menu", "assets/shaders/pause_menu.vert",
        "assets/shaders/pause_menu.frag");

    CreateKeybind("move up", KEY_W, KeybindMoveUpCallback, 0);
    CreateKeybind("move down", KEY_S, KeybindMoveDownCallback, 0);
    CreateKeybind("move left", KEY_A, KeybindMoveLeftCallback, 0);
    CreateKeybind("move right", KEY_D, KeybindMoveRightCallback, 0);
    CreateKeybind("toggle pause", KEY_P, KeybindTogglePauseCallback, 0);

    game.frame_count = 0;

    // create camera
    game.camera = (Camera2D) {
        .target = { 0, 0 },
        .offset = { game.screen_size.x / 2, game.screen_size.y / 2 },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    game.render_texture = LoadRenderTexture(game.screen_size.x,
        game.screen_size.y);

    LoadScene(SCENE_STARTSCREEN);
}

// one iteration of the game loop
// draws current scene
void Draw() {
    static void (*scene_draw_functions[])() = {
        [SCENE_STARTSCREEN] = DrawSceneStartScreen,
        [SCENE_SETTINGS] = DrawSceneSettings,
        [SCENE_GAMEPLAY] = DrawSceneGameplay,
        [SCENE_ENDSCREEN] = DrawSceneEndScreen
    };

    if (WindowMovedToNewMonitor()) ResizeDisplayToMonitor();
    scene_draw_functions[game.current_scene]();
}

void Quit() {
    // raylib cleanup functions run here
    UnloadAssets();
    exit(0);
}

void Pause() {
    game.paused = true;

    // update texture for pause menu shader
    Shader* s = GetShader("pause menu");
    SetShaderValueTexture(*s, GetShaderLocation(*s, "tex"),
        game.render_texture.texture);
}

void Unpause() {
    game.paused = false;
}

void TileBackground() {
    Texture2D bg_texture = *GetTexture("background");
    int x = game.player->pos.x;
    int y = game.player->pos.y;
    int i0 = ((x - game.screen_size.x/2) / bg_texture.width) - 5;
    int i1 = ((x + game.screen_size.x/2) / bg_texture.width) + 5;
    int j0 = ((y - game.screen_size.y/2) / bg_texture.height) - 5;
    int j1 = ((y + game.screen_size.y/2) / bg_texture.height) + 5;

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

bool WindowMovedToNewMonitor() {
    return game.current_monitor != GetCurrentMonitor();
}

void ResizeDisplayToMonitor() {
    game.current_monitor = GetCurrentMonitor();
    game.screen_size = (Vector2) {
        GetMonitorWidth(game.current_monitor),
        GetMonitorHeight(game.current_monitor)
    };

    UnloadRenderTexture(game.render_texture);
    game.render_texture = LoadRenderTexture(game.screen_size.x,
        game.screen_size.y);
    SetWindowSize(game.screen_size.x, game.screen_size.y);
    MaximizeWindow();

    // update screen size for pause menu shader
    Shader* s = GetShader("pause menu");
    SetShaderValueV(*s, GetShaderLocation(*s, "screenSize"), &game.screen_size,
        SHADER_UNIFORM_VEC2, 1);
}

void DrawRenderTexture() {
    BeginDrawing();
        DrawTextureRec(game.render_texture.texture,
            (Rectangle){0, 0, game.render_texture.texture.width,
                -game.render_texture.texture.height},
            (Vector2){0, 0}, WHITE);
    EndDrawing();
}
