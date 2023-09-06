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

void ObjUpdateTimerCallback(void* obj) {
    double now = GetTime();
    Object* timer = (Object*) obj;

    if (now - timer->data.tm_data.last_recorded < timer->data.tm_data.interval) {
        return;
    }

    timer->data.tm_data.last_recorded = now;
    timer->data.tm_data.callback();

    if (timer->data.tm_data.num_triggers > -1) {
        timer->data.tm_data.num_triggers--;
    }

    if (timer->data.tm_data.num_triggers == 0) {
        DeleteObject(OBJ_TIMER, timer->id);
    }
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

void ObjUpdateEnemyCallback(void* obj) {
    Object* o = (Object*) obj;

    // track player
    int dx = game.player->data.ent_data.pos.x - o->data.ent_data.pos.x;
    int dy = game.player->data.ent_data.pos.y - o->data.ent_data.pos.y;
    o->data.ent_data.angle = atan2(dy, dx);

    o->data.ent_data.pos.x += cos(o->data.ent_data.angle) * o->data.ent_data.speed;
    o->data.ent_data.pos.y += sin(o->data.ent_data.angle) * o->data.ent_data.speed;

    // check collision between enemy and player
    bool player_takes_dmg = !game.player->data.ent_data.iframes && CheckCollisionCircles(
        o->data.ent_data.pos, o->data.ent_data.hitbox_radius,
        game.player->data.ent_data.pos, game.player->data.ent_data.hitbox_radius);
    if (player_takes_dmg) {
        game.player->data.ent_data.health--;
        game.player->data.ent_data.iframes = 120;
    }

    // check collision between enemy and bullet
    for (int j = 0; j < OBJ_SLOT_COUNT; j++) {
        Object* b = GetObject(OBJ_ENTITY_BULLET, j);

        if (Vector2Distance(o->data.ent_data.pos, b->data.ent_data.pos) > 50) {
            continue;
        }
        bool enemy_takes_dmg = CheckCollisionCircles(
            o->data.ent_data.pos, o->data.ent_data.hitbox_radius,
            b->data.ent_data.pos, b->data.ent_data.hitbox_radius);
        if (enemy_takes_dmg) {
            o->data.ent_data.health -= b->data.ent_data.damage;
            DeleteObject(OBJ_ENTITY_BULLET, b->id);
        }
    }

    // remove if dead
    if (o->data.ent_data.health <= 0) {
        DeleteObject(OBJ_ENTITY_ENEMY, o->id);
    }
}

void ObjRenderEnemyCallback(void* obj) {
    Object* o = (Object*) obj;
    DrawCircle(o->data.ent_data.pos.x, o->data.ent_data.pos.y, 25, BLACK);
    DrawCircle(o->data.ent_data.pos.x, o->data.ent_data.pos.y, 22, RED);
}

void ObjUpdateBulletCallback(void* obj) {
    Object* o = (Object*) obj;
    
    o->data.ent_data.pos.x += cos(o->data.ent_data.angle) * o->data.ent_data.speed;
    o->data.ent_data.pos.y += sin(o->data.ent_data.angle) * o->data.ent_data.speed;
    o->data.ent_data.lifetime--;

    if (o->data.ent_data.lifetime == 0) {
        DeleteObject(OBJ_ENTITY_BULLET, o->id);
    }
}

void ObjRenderBulletCallback(void* obj) {
    Object* o = (Object*) obj;
    float c = cos(o->data.ent_data.angle);
    float s = sin(o->data.ent_data.angle);

    DrawLineEx(
        (Vector2){o->data.ent_data.pos.x - c * 5, o->data.ent_data.pos.y - s * 5},
        (Vector2){o->data.ent_data.pos.x + c * 5, o->data.ent_data.pos.y + s * 5},
        4.0f, BLACK);
    DrawLineEx(
        (Vector2){o->data.ent_data.pos.x - c * 4, o->data.ent_data.pos.y - s * 4},
        (Vector2){o->data.ent_data.pos.x + c * 4, o->data.ent_data.pos.y + s * 4},
        3.0f, WHITE);
}
