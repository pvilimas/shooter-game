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
    game.screen_size = (Vector2) {
        GetScreenWidth(),
        GetScreenHeight()
    };

    // game.objects = {};
    for (int type = 0; type < OBJ_COUNT; type++) {
        for (int id = 0; id < OBJECT_SLOTS; id++) {
            game.objects[type][id].active = false;
        }
    }

    game.player = CreateObject(OBJ_ENTITY_PLAYER);

    game.camera = (Camera2D) {
        .target = game.player->data.ent_data.pos, /* target follows player */
        .offset = { game.screen_size.x / 2, game.screen_size.y / 2 },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    game.timers = g_ptr_array_new();

    game.textures = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, FreeTextureCallback);
    game.fonts = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, FreeFontCallback);

    game.current_scene = SCENE_GAMEPLAY;
    game.frame_count = 0;

    CreateTexture("background", "assets/bg.png");

    Object* timer = CreateObject(OBJ_TIMER);
    timer->data.tm_data.fn = PlayerShootAtMouseCallback;
    timer->data.tm_data.interval = 2.0;
    timer->data.tm_data.num_triggers = -1;

    timer = CreateObject(OBJ_TIMER);
    timer->data.tm_data.fn = SpawnEnemyCallback;
    timer->data.tm_data.interval = 0.1;
    timer->data.tm_data.num_triggers = -1;
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

void DrawUI() {
    // player health bar
    for (int i = 0; i < game.player->data.ent_data.max_health / 20; i++) {
        DrawRectangleRec((Rectangle){20+(40*i), 20, 43, 15}, BLACK);
    }
    for (int i = 0; i < game.player->data.ent_data.health / 20; i++) {
        DrawRectangleRec((Rectangle){23+(40*i), 23, 37, 9}, (Color){255, 0, 0, 255});
    }
}

void SpawnEnemy(Vector2 pos, float angle, int speed) {
    Object* o = CreateObject(OBJ_ENTITY_ENEMY);
    o->data.ent_data.pos = pos;
    o->data.ent_data.angle = angle;
}

void SpawnBullet(Vector2 pos, float angle, int speed) {
    Object* o = CreateObject(OBJ_ENTITY_BULLET);
    o->data.ent_data.pos = pos;
    o->data.ent_data.angle = angle;
}
