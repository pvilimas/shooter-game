#include "game.h"

// global instance
Game game;

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
    game.player = (Player) {
        .pos = { 0, 0 },
        .health = 5,
        .iframes = 0,
        .hitbox_radius = 5
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

    game.current_scene = SCENE_GAMEPLAY;
    game.frame_count = 0;

    CreateTexture("background", "assets/bg.png");
    CreateTimer(PlayerShootAtMouseCallback, 2.0, -1);
    CreateTimer(SpawnEnemyCallback, 0.1, -1);
}

// one iteration of the game loop
void Draw() {
    UpdateTimers();

    // update camera target to player
    game.camera.target = game.player.pos;

    BeginDrawing();
        BeginMode2D(game.camera);
            
            /* all in-game objects get drawn here */
            
            TileBackground();
            RenderPlayer();
            RenderEnemies();
            RenderBullets();

        EndMode2D();

        /* static ui elements get drawn here */

        DrawUI();
    EndDrawing();
    game.frame_count++;
}

void Quit() {
    // raylib cleanup functions run here
    UnloadAssets();
}

// offset of the camera from the origin
Vector2 GetCameraOffset() {
    return (Vector2){
        game.camera.target.x - GetScreenWidth() / 2,
        game.camera.target.y - GetScreenHeight() / 2
    };
}

// absolute mouse position, accounting for camera offset
Vector2 GetAbsMousePosition() {
    return Vector2Add(GetMousePosition(), GetCameraOffset());
}

// returns true if p is onscreen
bool PointOnScreen(Vector2 p) {
    return PointNearScreen(p, 0.0f);
}

// returns true if p is onscreen or within range pixels of screen edge
// negative range works as expected
bool PointNearScreen(Vector2 p, float range) {
    return p.x >= game.camera.target.x + range - GetScreenWidth() / 2
        && p.x <= game.camera.target.x - range + GetScreenWidth() / 2
        && p.y >= game.camera.target.y + range - GetScreenHeight() / 2
        && p.y <= game.camera.target.y - range + GetScreenHeight() / 2;
}

int RandInt(int min, int max) {
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

float RandFloat(float min, float max) {
    return min + (float)rand() / ((float)(RAND_MAX / (max-min)));
}

// contains initialization logic for each scene
void LoadScene(Scene new_scene) {
    if (game.current_scene == SCENE_STARTSCREEN && new_scene == SCENE_GAMEPLAY) {

    } else if (game.current_scene == SCENE_GAMEPLAY && new_scene == SCENE_ENDSCREEN) {

    } else if (game.current_scene == SCENE_ENDSCREEN && new_scene == SCENE_GAMEPLAY) {

    } else {
        printf("Invalid scene transition, exiting\n");
        Quit();
    }
    game.current_scene = new_scene;
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
    int i0 = ((game.player.pos.x - game.screen_size.x/2) / bg_texture.width) - 5;
    int i1 = ((game.player.pos.x + game.screen_size.x/2) / bg_texture.width) + 5;
    int j0 = ((game.player.pos.y - game.screen_size.y/2) / bg_texture.height) - 5;
    int j1 = ((game.player.pos.y + game.screen_size.y/2) / bg_texture.height) + 5;

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
    for (int i = 0; i < 5; i++) {
        DrawRectangleRec((Rectangle){20+(40*i), 20, 43, 15}, BLACK);
    }
    for (int i = 0; i < game.player.health; i++) {
        DrawRectangleRec((Rectangle){23+(40*i), 23, 37, 9}, (Color){255, 0, 0, 255});
    }
}

void RenderPlayer() {
    // update
    HandleInput();
    if (game.player.iframes != 0) {
        game.player.iframes--;
    }

    // draw
    if (game.player.iframes % 20 < 10) {
        DrawCircle(game.player.pos.x, game.player.pos.y, 5.0f, BLACK);
    }
}

void RenderEnemies() {
    for (int i = 0; i < game.enemies->len; i++) {

        Enemy* e = game.enemies->pdata[i];

        // update
        // track player
        int dx = game.player.pos.x - e->pos.x;
        int dy = game.player.pos.y - e->pos.y;
        e->angle = atan2(dy, dx);

        e->pos.x += cos(e->angle) * e->speed;
        e->pos.y += sin(e->angle) * e->speed;

        // check collision between enemy and player
        bool player_takes_dmg = !game.player.iframes && CheckCollisionCircles(
            e->pos, e->hitbox_radius,
            game.player.pos, game.player.hitbox_radius);
        if (player_takes_dmg) {
            game.player.health--;
            game.player.iframes = 120;
        }

        // check collision between enemy and bullet
        for (int j = 0; j < game.bullets->len; j++) {
            Bullet* b = game.bullets->pdata[j];
            if (Vector2Distance(e->pos, b->pos) > 50) {
                continue;
            }
            bool enemy_takes_dmg = CheckCollisionCircles(
                e->pos, e->hitbox_radius,
                b->pos, b->hitbox_radius);
            if (enemy_takes_dmg) {
                e->health--;
                g_ptr_array_remove_index_fast(game.bullets, j);
            }
        }
        
        if (e->health <= 0) {
            g_ptr_array_remove_index_fast(game.enemies, i);
        }

        // draw
        DrawCircle(e->pos.x, e->pos.y, 25, BLACK);
        DrawCircle(e->pos.x, e->pos.y, 22, RED);
    }
}

void RenderBullets() {
    for (int i = 0; i < game.bullets->len; i++) {

        Bullet* b = game.bullets->pdata[i];

        // update
        b->pos.x += cos(b->angle) * b->speed;
        b->pos.y += sin(b->angle) * b->speed;
        b->lifetime--;

        if (b->lifetime == 0) {
            g_ptr_array_remove_index_fast(game.bullets, i);
        }

        // draw
        DrawLineEx(
            (Vector2){b->pos.x - cos(b->angle) * 5, b->pos.y - sin(b->angle) * 5},
            (Vector2){b->pos.x + cos(b->angle) * 5, b->pos.y + sin(b->angle) * 5},
            4.0f, BLACK);
        DrawLineEx(
            (Vector2){b->pos.x - cos(b->angle) * 4, b->pos.y - sin(b->angle) * 4},
            (Vector2){b->pos.x + cos(b->angle) * 4, b->pos.y + sin(b->angle) * 4},
            3.0f, WHITE);
    }
}

void SpawnEnemy(Vector2 pos, float angle, int speed) {
    Enemy* e = malloc(sizeof(Enemy));
    *e = (Enemy){pos, angle, speed, 1, 25};
    g_ptr_array_add(game.enemies, e);
}

void SpawnBullet(Vector2 pos, float angle, int speed) {
    Bullet* b = malloc(sizeof(Bullet));
    *b = (Bullet){pos, angle, speed, 30, 1, 10};
    g_ptr_array_add(game.bullets, b);
}

void CreateTimer(TimerCallback fn, double interval, int num_triggers) {
    Timer* t = malloc(sizeof(Timer));
    *t = (Timer) {
        .interval = interval,
        .num_triggers = num_triggers,
        .fn = fn,
        .last_recorded = GetTime()
    };
    g_ptr_array_add(game.timers, t);
}

// returns true if timer is out of triggers
bool UpdateTimer(Timer* t) {
    double now = GetTime();
    if (now - t->last_recorded < t->interval) {
        return false;
    }

    t->last_recorded = now;
    t->fn();
    if (t->num_triggers > -1) {
        t->num_triggers--;
    }

    if (!t->num_triggers) {
        return true;
    }

    return false;
}

void UpdateTimers() {
    for (int i = 0; i < game.timers->len; i++) {
        if (UpdateTimer(game.timers->pdata[i])) {
            g_ptr_array_remove_index_fast(game.timers, i);
        }
    }
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

void TestCallback() {
    printf("test\n");
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
    Vector2 abs_mouse_pos = GetAbsMousePosition();
    int dx = abs_mouse_pos.x - game.player.pos.x;
    int dy = abs_mouse_pos.y - game.player.pos.y;
    double angle = atan2(dy, dx);
    for (int i = 0; i < 5; i++) {
        SpawnBullet(game.player.pos, RandFloat(angle - 0.125, angle + 0.125), 10);
    }
}

void SpawnEnemyCallback() {
    int enemy_spawn_radius = sqrt(2 * (pow(game.screen_size.x, 2) + pow(game.screen_size.y, 2)));
    Vector2 camera_offset = GetCameraOffset();
    Vector2 pos = {
        GetRandomValue(camera_offset.x - enemy_spawn_radius, camera_offset.x + enemy_spawn_radius),
        GetRandomValue(camera_offset.y - enemy_spawn_radius, camera_offset.y + enemy_spawn_radius)
    };
    while (PointNearScreen(pos, 10)) {
        pos = (Vector2){
            GetRandomValue(camera_offset.x - enemy_spawn_radius, camera_offset.x + enemy_spawn_radius),
            GetRandomValue(camera_offset.y - enemy_spawn_radius, camera_offset.y + enemy_spawn_radius)
        };
    }
    int dx = game.player.pos.x - pos.x;
    int dy = game.player.pos.y - pos.y;
    double angle = atan2(dy, dx);
    SpawnEnemy(pos, angle, 1);
}
