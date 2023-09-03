#include "game.h"

void SG_Init(){
    game.screenSize = (Vector2){
        SG_DEFAULT_SCREEN_W,
        SG_DEFAULT_SCREEN_H
    };
    game.player = (SG_Player){ .pos = {0, 0}};
    game.camera = (Camera2D){
        .target = {0, 0}, // target follows player and gets updated in the game loop
        .offset = {game.screenSize.x / 2, game.screenSize.y / 2},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    game.textures = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, SG_FreeTextureCallback);
    game.fonts = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, SG_FreeFontCallback);

    SG_LoadTexture("background", "assets/bg.png");
}

// one iteration of the game loop
void SG_Draw() {

    SG_HandleInput();

    // update camera target to player
    game.camera.target = game.player.pos;

    BeginDrawing();

        ClearBackground(RAYWHITE);
        BeginMode2D(game.camera);
            
            /* all in-game objects get drawn here */

            SG_TileBackground();
            SG_DrawPlayer();

        EndMode2D();

        /* static ui elements get drawn here */

    EndDrawing();
}

void SG_Quit(){
    // raylib cleanup functions run here
    SG_UnloadAssets();
}

void SG_DrawPlayer() {
    DrawCircle(game.player.pos.x, game.player.pos.y, 5.0f, BLACK);
}

void SG_TileBackground() {
    Texture2D bg_texture = *SG_GetTexture("background");
    int i0 = ((game.player.pos.x - game.screenSize.x/2) / bg_texture.width) - 1;
    int i1 = ((game.player.pos.x + game.screenSize.x/2) / bg_texture.width) + 1;
    int j0 = ((game.player.pos.y - game.screenSize.y/2) / bg_texture.height) - 1;
    int j1 = ((game.player.pos.y + game.screenSize.y/2) / bg_texture.height) + 1;

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

// Handle all key input and move the player
void SG_HandleInput() {
    // TODO add sequential key presses, so like if W is pressed 
    // and then S, character continues moving up
    if (IsKeyDown(KEY_W)) {
        game.player.pos.y -= 10;
    }
    if (IsKeyDown(KEY_A)) {
        game.player.pos.x -= 10;
    }
    if (IsKeyDown(KEY_S)) {
        game.player.pos.y += 10;
    }
    if (IsKeyDown(KEY_D)) {
        game.player.pos.x += 10;
    }
}

void SG_LoadTexture(char* name, const char* filepath) {
    Texture2D* tp = malloc(sizeof(Texture2D));
    *tp = LoadTexture(filepath);
    g_hash_table_insert(game.textures, name, tp);
}

Texture2D* SG_GetTexture(char* name) {
    return g_hash_table_lookup(game.textures, name); // null if not found
}

void SG_UnloadTexture(char* name) {
    g_hash_table_remove(game.textures, name);
}

void SG_LoadFont(char* name, const char* filepath) {
    Font* tp = malloc(sizeof(Font));
    *tp = LoadFont(filepath);
    g_hash_table_insert(game.fonts, name, tp);
}

Font* SG_GetFont(char* name) {
    return g_hash_table_lookup(game.fonts, name); // null if not found
}

void SG_UnloadFont(char* name) {
    g_hash_table_remove(game.fonts, name);
}

void SG_UnloadAssets() {
    g_hash_table_destroy(game.textures);
    g_hash_table_destroy(game.fonts);
}

void SG_FreeTextureCallback(void* texture) {
    UnloadTexture(*(Texture2D*)texture);
    free(texture);
}

void SG_FreeFontCallback(void* font) {
    UnloadFont(*(Font*)font);
    free(font);
}
