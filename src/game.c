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
