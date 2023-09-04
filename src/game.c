#include "game.h"

// raylib configuration and system setup functions
void Config() {
    srand(time(NULL));

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_VSYNC_HINT
        | FLAG_MSAA_4X_HINT 
        | FLAG_WINDOW_TOPMOST 
        | FLAG_WINDOW_RESIZABLE);
    InitWindow(
        DEFAULT_SCREEN_W,
        DEFAULT_SCREEN_H,
        WINDOW_TITLE);

    // disable exit key when not debugging
    if (!DEBUGGING) SetExitKey(0);
    // for dev mode, make sure cwd is shooter-game not build
    ChangeDirectory("..");
    SetTargetFPS(60);
}

void Init() {
    game.screen_size = (Vector2) {
        DEFAULT_SCREEN_W,
        DEFAULT_SCREEN_H
    };
    game.player = (Player) {
        .pos = { 0, 0 },
        .health = 5
    };
    game.camera = (Camera2D) {
        .target = game.player.pos, /* target follows player */
        .offset = { game.screen_size.x / 2, game.screen_size.y / 2 },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    game.timers = g_ptr_array_new();
    game.enemies = g_ptr_array_new();
    game.bullets = g_ptr_array_new();

    game.textures = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, FreeTextureCallback);
    game.fonts = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, FreeFontCallback);

    CreateTexture("background", "assets/bg.png");
    CreateTimer(PlayerShootAtMouseCallback, 1.0, -1);
}

// one iteration of the game loop
void Draw() {

    HandleInput();
    UpdateTimers();
    UpdateEnemies();
    UpdateBullets();

    // update camera target to player
    game.camera.target = game.player.pos;

    BeginDrawing();

        BeginMode2D(game.camera);
            
            /* all in-game objects get drawn here */

            TileBackground();
            DrawPlayer();

        EndMode2D();

        /* static ui elements get drawn here */

        DrawUI();

    EndDrawing();
}

void Quit() {
    // raylib cleanup functions run here
    UnloadAssets();
}

// Handle all key input and move the player
void HandleInput() {
    // TODO add sequential key presses, so like if W is pressed 
    // and then S, character continues moving up
    if (IsKeyDown(KEY_W)) {
        game.player.pos.y -= 5;
    }
    if (IsKeyDown(KEY_A)) {
        game.player.pos.x -= 5;
    }
    if (IsKeyDown(KEY_S)) {
        game.player.pos.y += 5;
    }
    if (IsKeyDown(KEY_D)) {
        game.player.pos.x += 5;
    }
}

void TileBackground() {
    Texture2D bg_texture = *GetTexture("background");
    int i0 = ((game.player.pos.x - game.screen_size.x/2) / bg_texture.width) - 1;
    int i1 = ((game.player.pos.x + game.screen_size.x/2) / bg_texture.width) + 1;
    int j0 = ((game.player.pos.y - game.screen_size.y/2) / bg_texture.height) - 1;
    int j1 = ((game.player.pos.y + game.screen_size.y/2) / bg_texture.height) + 1;

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

void DrawPlayer() {
    DrawCircle(game.player.pos.x, game.player.pos.y, 5.0f, BLACK);
}

void DrawUI() {
    DrawHealthBar();
}

void DrawHealthBar() {
    for (int i = 0; i < game.player.health; i++) {
        DrawRectangleRec((Rectangle){20+(40*i), 20, 40, 15}, BLACK);
        DrawRectangleRec((Rectangle){23+(40*i), 23, 34, 9}, (Color){255, 0, 0, 255});
    }
}

void CreateEnemy(Vector2 pos, Vector2 speed) {
    Enemy* e = malloc(sizeof(Enemy));
    *e = (Enemy){pos, speed};
    g_ptr_array_add(game.enemies, e);
}

void UpdateEnemies() {
    for (int i = 0; i < game.enemies->len; i++) {
        UpdateEnemy(game.enemies->pdata[i]);
    }
}

void UpdateEnemy(Enemy* e) {
    e->pos = Vector2Add(e->pos, e->speed);
}

void CreateBullet(Vector2 pos, Vector2 speed) {
    Bullet* b = malloc(sizeof(Bullet));
    *b = (Bullet){pos, speed};
    g_ptr_array_add(game.bullets, b);
}

void UpdateBullets() {
    for (int i = 0; i < game.bullets->len; i++) {
        UpdateBullet(game.bullets->pdata[i]);
    }
}

void UpdateBullet(Bullet* b) {
    b->pos = Vector2Add(b->pos, b->speed);
}

void CreateTimer(TimerCallback fn, double interval, int num_triggers) {
    Timer* t = malloc(sizeof(Timer));
    *t = (Timer) {
        .interval = interval,
        .num_triggers = num_triggers,
        .fn = fn,
        .last_recorded = GetTime()
    };
}

void UpdateTimers() {
    for (int i = 0; i < game.timers->len; i++) {
        if (UpdateTimer(game.timers->pdata[i])) {
            g_ptr_array_remove_index_fast(game.timers, i);
        }
    }
}

// returns true if timer is out of triggers
bool UpdateTimer(Timer* t) {
    double now = GetTime();
    if (now - t->last_recorded < t->interval) {
        return false;
    }
    
    t->fn();

    if (--t->num_triggers == 0) {
        return true;
    }

    return false;
}

void UnloadAssets() {
    g_hash_table_destroy(game.textures);
    g_hash_table_destroy(game.fonts);
}

void CreateTexture(char* name, const char* filepath) {
    Texture2D* t = malloc(sizeof(Texture2D));
    *t = LoadTexture(filepath);
    g_hash_table_insert(game.textures, name, t);
}

Texture2D* GetTexture(char* name) {
    return g_hash_table_lookup(game.textures, name); // null if not found
}

void DeleteTexture(char* name) {
    g_hash_table_remove(game.textures, name);
}

void CreateFont(char* name, const char* filepath) {
    Font* f = malloc(sizeof(Font));
    *f = LoadFont(filepath);
    g_hash_table_insert(game.fonts, name, f);
}

Font* GetFont(char* name) {
    return g_hash_table_lookup(game.fonts, name); // null if not found
}

void DeleteFont(char* name) {
    g_hash_table_remove(game.fonts, name);
}

void FreeTextureCallback(void* texture) {
    UnloadTexture(*(Texture2D*)texture);
    free(texture);
}

void FreeFontCallback(void* font) {
    UnloadFont(*(Font*)font);
    free(font);
}

void DefaultTimerCallback() {
   
}

void PlayerShootAtMouseCallback() {
    int dx = GetMouseX() - game.player.pos.x;
    int dy = GetMouseY() - game.player.pos.y;
    double angle = atan2(dy, dx);
    CreateBullet(game.player.pos, (Vector2){cos(angle)*10, sin(angle)*10});
}