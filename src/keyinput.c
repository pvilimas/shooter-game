#include "game.h"

void HandleInput() {
    // loop through all keybinds
    GHashTableIter iter;
    g_hash_table_iter_init(&iter, game.keybinds);
    const char** k;
    Keybind* v;
    while (g_hash_table_iter_next(&iter, (gpointer)&k, (gpointer)&v)) {
        if (IsKeyDown(v->key_code)) {
            v->callback();
        }
    }
}

void CreateKeybind(const char* name, int key_code, KeybindCallback callback,
    int delay_ms) {
    Keybind* k = malloc(sizeof(Keybind));
    *k = (Keybind) {
        .name = name,
        .key_code = key_code,
        .callback = callback,
        .delay_ms = delay_ms
    };
    g_hash_table_insert(game.keybinds, (gpointer)name, (gpointer)k);
}

void RemapKeybind(const char* name, int new_key_code) {
    Keybind* k = (Keybind*) g_hash_table_lookup(game.keybinds, name);
    k->key_code = new_key_code;
}

void DeleteKeybind(const char* name) {
    g_hash_table_remove(game.keybinds, name);
}
