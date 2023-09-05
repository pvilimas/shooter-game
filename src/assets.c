#include "game.h"

/*
    assets.c - asset management
        - fonts
        - textures
        - sounds/music
*/

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