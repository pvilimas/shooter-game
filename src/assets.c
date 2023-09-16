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

void CreateTexture(const char* name, const char* filepath) {
    Texture2D* t = malloc(sizeof(Texture2D));
    *t = LoadTexture(filepath);
    g_hash_table_insert(game.textures, (char*)name, t);
}

Texture2D* GetTexture(const const char* name) {
    return g_hash_table_lookup(game.textures, name); // null if not found
}

void DeleteTexture(const char* name) {
    g_hash_table_remove(game.textures, name);
}

void CreateFont(const char* name, const char* filepath) {
    Font* f = malloc(sizeof(Font));
    *f = LoadFont(filepath);
    g_hash_table_insert(game.fonts, (char*)name, f);
}

Font* GetFont(const char* name) {
    return g_hash_table_lookup(game.fonts, name); // null if not found
}

void DeleteFont(const char* name) {
    g_hash_table_remove(game.fonts, name);
}

void CreateShader(const char* name, const char* vert_shader_path,
    const char* frag_shader_path) {
    Shader* s = malloc(sizeof(Shader));
    *s = LoadShader(vert_shader_path, frag_shader_path);
    g_hash_table_insert(game.shaders, (char*)name, s);
}

Shader* GetShader(const char* name) {
    return g_hash_table_lookup(game.shaders, name);
}

void DeleteShader(const char* name) {
    g_hash_table_remove(game.shaders, name);
}
