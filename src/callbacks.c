#include "game.h"

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
    int dx = abs_mouse_pos.x - game.player->data.ent_data.pos.x;
    int dy = abs_mouse_pos.y - game.player->data.ent_data.pos.y;
    double angle = atan2(dy, dx);
    for (int i = 0; i < 5; i++) {
        SpawnBullet(game.player->data.ent_data.pos, RandFloat(angle - 0.125, angle + 0.125), 10);
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
    int dx = game.player->data.ent_data.pos.x - pos.x;
    int dy = game.player->data.ent_data.pos.y - pos.y;
    double angle = atan2(dy, dx);
    SpawnEnemy(pos, angle, 1);
}

void ObjUpdatePlayerCallback(void* obj) {
    if (IsKeyDown(KEY_W)) {
        game.player->data.ent_data.pos.y -= 5;
    }
    if (IsKeyDown(KEY_A)) {
        game.player->data.ent_data.pos.x -= 5;
    }
    if (IsKeyDown(KEY_S)) {
        game.player->data.ent_data.pos.y += 5;
    }
    if (IsKeyDown(KEY_D)) {
        game.player->data.ent_data.pos.x += 5;
    }

    if (game.player->data.ent_data.iframes != 0) {
        game.player->data.ent_data.iframes--;
    }
}

void ObjRenderPlayerCallback(void* obj) {
    if (game.player->data.ent_data.iframes % 20 < 10) {
        DrawCircle(game.player->data.ent_data.pos.x, game.player->data.ent_data.pos.y, 5.0f, BLACK);
    }
}
