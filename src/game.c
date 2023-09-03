#include "game.h"

SG_Game* SG_NewGame(){
    SG_Game* g = malloc(sizeof(SG_Game));
    if (!g) {
        return NULL;
    }
    g->screenSize = (Vector2){
        SG_DEFAULT_SCREEN_W,
        SG_DEFAULT_SCREEN_H
    };
    g->player = (SG_Player){ .pos = {0, 0}};
    g->camera = (Camera2D){
        .target = {0, 0}, // target follows player and gets updated in the game loop
        .offset = {g->screenSize.x / 2, g->screenSize.y / 2},
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    g->textures = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, SG_FreeTextureCallback);
    g->fonts = g_hash_table_new_full(g_str_hash, g_str_equal,
        NULL, SG_FreeFontCallback);

    SG_LoadTexture(g, "bg tile", "assets/bg.png");

    return g;
}

// one iteration of the game loop
void SG_Draw(SG_Game* g) {

    SG_HandleInput(g);

    // update camera target to player
    g->camera.target = g->player.pos;

    BeginDrawing();

        ClearBackground(RAYWHITE);
        BeginMode2D(g->camera);
            
            /* all in-game objects get drawn here */

            // bg tiling
            Texture2D bg_texture = *SG_GetTexture(g, "bg tile");
            int tile_cols = ceil(g->screenSize.x / bg_texture.width);
            int tile_rows = ceil(g->screenSize.y / bg_texture.height);
            for (int x = 0; x < g->screenSize.x; x += bg_texture.width) {
                for (int y = 0; y < g->screenSize.y; y += bg_texture.height) {
                    DrawTexture(
                        bg_texture,
                        x - (g->screenSize.x / 2),
                        y - (g->screenSize.y / 2),
                        WHITE
                    );
                }
            }

            SG_DrawPlayer(g);

        EndMode2D();

        /* static ui elements get drawn here */

    EndDrawing();
}

void SG_Quit(SG_Game* g){
    // raylib cleanup functions run here
    SG_UnloadAssets(g);
}

void SG_DrawPlayer(SG_Game* g) {
    DrawCircle(g->player.pos.x, g->player.pos.y, 5.0f, BLACK);
}

void SG_DrawBackgroundTiles(SG_Game* g) {
    ClearBackground(RAYWHITE);
}

// Handle all key input and move the player
void SG_HandleInput(SG_Game* g) {
    // TODO add sequential key presses, so like if W is pressed 
    // and then S, character continues moving up
    if (IsKeyDown(KEY_W)) {
        g->player.pos.y -= 10;
    }
    if (IsKeyDown(KEY_A)) {
        g->player.pos.x -= 10;
    }
    if (IsKeyDown(KEY_S)) {
        g->player.pos.y += 10;
    }
    if (IsKeyDown(KEY_D)) {
        g->player.pos.x += 10;
    }
}

void SG_LoadTexture(SG_Game* g, char* name, const char* filepath) {
    Texture2D* tp = malloc(sizeof(Texture2D));
    *tp = LoadTexture(filepath);
    g_hash_table_insert(g->textures, name, tp);
}

Texture2D* SG_GetTexture(SG_Game* g, char* name) {
    return g_hash_table_lookup(g->textures, name); // null if not found
}

void SG_UnloadTexture(SG_Game* g, char* name) {
    g_hash_table_remove(g->textures, name);
}

void SG_LoadFont(SG_Game* g, char* name, const char* filepath) {
    Font* tp = malloc(sizeof(Font));
    *tp = LoadFont(filepath);
    g_hash_table_insert(g->fonts, name, tp);
}

Font* SG_GetFont(SG_Game* g, char* name) {
    return g_hash_table_lookup(g->fonts, name); // null if not found
}

void SG_UnloadFont(SG_Game* g, char* name) {
    g_hash_table_remove(g->fonts, name);
}

void SG_UnloadAssets(SG_Game* g) {
    g_hash_table_destroy(g->textures);
    g_hash_table_destroy(g->fonts);
}

void SG_FreeTextureCallback(void* texture) {
    UnloadTexture(*(Texture2D*)texture);
    free(texture);
}

void SG_FreeFontCallback(void* font) {
    UnloadFont(*(Font*)font);
    free(font);
}
