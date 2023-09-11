#include "game.h"

void TestCallback() {
    printf("test\n");
}

void MemFreeTextureCallback(void* texture) {
    UnloadTexture(*(Texture2D*)texture);
    free(texture);
}

void MemFreeFontCallback(void* font) {
    UnloadFont(*(Font*)font);
    free(font);
}

void TimerPlayerShootBasicBulletCallback() {
    Vector2 abs_mouse_pos = GetAbsMousePosition();
    int dx = abs_mouse_pos.x - game.player->data.ent_data.pos.x;
    int dy = abs_mouse_pos.y - game.player->data.ent_data.pos.y;
    double angle = atan2(dy, dx);

    for (int i = 0; i < 5; i++) {
        Object* o = CreateObject(OC_GAMEPLAY_ENTITY_BULLET_BASIC);
        o->data.ent_data.pos = game.player->data.ent_data.pos;
        o->data.ent_data.angle = RandFloat(angle - 0.125, angle + 0.125);
    }
}

void TimerSpawnBasicEnemyCallback() {
    int enemy_spawn_radius = sqrt(2 * (pow(game.screen_size.x, 2) + pow(game.screen_size.y, 2)));
    Vector2 camera_offset = GetCameraOffset();
    Vector2 pos = {
        GetRandomValue(camera_offset.x - enemy_spawn_radius, camera_offset.x + enemy_spawn_radius),
        GetRandomValue(camera_offset.y - enemy_spawn_radius, camera_offset.y + enemy_spawn_radius)
    };

    // repick until chosen point is offscreen by a certain radius
    while (PointNearScreen(pos, 10)) {
        pos = (Vector2){
            GetRandomValue(camera_offset.x - enemy_spawn_radius, camera_offset.x + enemy_spawn_radius),
            GetRandomValue(camera_offset.y - enemy_spawn_radius, camera_offset.y + enemy_spawn_radius)
        };
    }
    int dx = game.player->data.ent_data.pos.x - pos.x;
    int dy = game.player->data.ent_data.pos.y - pos.y;
    double angle = atan2(dy, dx);
    
    Object* o = CreateObject(OC_GAMEPLAY_ENTITY_ENEMY_BASIC);
    o->data.ent_data.pos = pos;
    o->data.ent_data.angle = angle;
}

// OBJ_TIMER

void ObjUpdateTimerCallback(void* obj) {
    double now = GetTime();
    Object* timer = (Object*) obj;
    TimerObjData* tdata = &timer->data.tm_data;

    if (now - tdata->last_recorded < tdata->interval) {
        return;
    }

    tdata->last_recorded = now;
    tdata->callback();

    if (tdata->num_triggers > -1) {
        tdata->num_triggers--;
    }

    if (tdata->num_triggers == 0) {
        DeleteObject(timer->type, timer->id);
    }
}

// OBJ_ENTITY_PLAYER

void ObjUpdatePlayerCallback(void* obj) {
    EntityObjData* pdata = &game.player->data.ent_data;

    if (IsKeyDown(KEY_W)) {
        pdata->pos.y -= 5;
    }
    if (IsKeyDown(KEY_A)) {
        pdata->pos.x -= 5;
    }
    if (IsKeyDown(KEY_S)) {
        pdata->pos.y += 5;
    }
    if (IsKeyDown(KEY_D)) {
        pdata->pos.x += 5;
    }

    if (pdata->iframes != 0) {
        pdata->iframes--;
    }
}

void ObjRenderPlayerCallback(void* obj) {
    EntityObjData* pdata = &game.player->data.ent_data;

    if (pdata->iframes % 20 < 10) {
        DrawCircle(pdata->pos.x, pdata->pos.y, 25.0f, BLACK);
    }
}

// OBJ_ENTITY_ENEMY

void ObjUpdateEnemyCallback(void* obj) {
    Object* enemy = (Object*) obj;
    EntityObjData* edata = &enemy->data.ent_data;
    EntityObjData* pdata = &game.player->data.ent_data;

    // track player
    int dx = pdata->pos.x - edata->pos.x;
    int dy = pdata->pos.y - edata->pos.y;
    edata->angle = atan2(dy, dx);

    edata->pos.x += cos(edata->angle) * edata->speed;
    edata->pos.y += sin(edata->angle) * edata->speed;

    // check collision between enemy and player
    bool player_takes_dmg = !pdata->iframes && CheckCollisionCircles(
        edata->pos, edata->hitbox_radius,
        pdata->pos, pdata->hitbox_radius);
    if (player_takes_dmg) {
        pdata->health -= edata->damage;
        pdata->iframes = 120;
    }
    if (pdata->health <= 0) {
        KillPlayer();
    }

    // check collision between enemy and bullet
    for (int j = 0; j < OBJ_SLOT_COUNT; j++) {
        Object* bullet = GetObject(OBJ_ENTITY_BULLET, j);
        EntityObjData* bdata = &bullet->data.ent_data;

        if (!bullet->active) {
            continue;
        }

        if (Vector2Distance(edata->pos, bdata->pos) > 50) {
            continue;
        }
        bool enemy_takes_dmg = CheckCollisionCircles(
            edata->pos, edata->hitbox_radius,
            bdata->pos, bdata->hitbox_radius);
        if (enemy_takes_dmg) {
            edata->health -= bdata->damage;
            DeleteObject(bullet->type, bullet->id);
        }
    }

    // remove if dead
    if (edata->health <= 0) {
        DeleteObject(enemy->type, enemy->id);
    }
}

void ObjRenderEnemyCallback(void* obj) {
    Object* enemy = (Object*) obj;
    EntityObjData* edata = &enemy->data.ent_data;

    DrawCircle(edata->pos.x, edata->pos.y, 25, BLACK);
    DrawCircle(edata->pos.x, edata->pos.y, 22, RED);
}

// OBJ_ENTITY_BULLET

void ObjUpdateBulletCallback(void* obj) {
    Object* bullet = (Object*) obj;
    EntityObjData* bdata = &bullet->data.ent_data;
    
    bdata->pos.x += cos(bdata->angle) * bdata->speed;
    bdata->pos.y += sin(bdata->angle) * bdata->speed;
    bdata->lifetime--;

    if (bdata->lifetime == 0) {
        DeleteObject(bullet->type, bullet->id);
    }
}

void ObjRenderBulletCallback(void* obj) {
    Object* bullet = (Object*) obj;
    EntityObjData* bdata = &bullet->data.ent_data;

    float c = cos(bdata->angle);
    float s = sin(bdata->angle);

    DrawLineEx(
        (Vector2){ bdata->pos.x - c * 5, bdata->pos.y - s * 5 },
        (Vector2){ bdata->pos.x + c * 5, bdata->pos.y + s * 5 },
        4.0f, BLACK);
    DrawLineEx(
        (Vector2){ bdata->pos.x - c * 4, bdata->pos.y - s * 4 },
        (Vector2){ bdata->pos.x + c * 4, bdata->pos.y + s * 4 },
        3.0f, WHITE);
}

// OBJ_UI_TEXT

void ObjRenderTextCallback(void* obj) {
    
    // text only uses color1

    Object* text = (Object*) obj;
    UIObjData* tdata = &text->data.ui_data;

    DrawText(tdata->label, tdata->pos.x, tdata->pos.y, tdata->font_size, tdata->color1);
}

// OBJ_UI_BUTTON

void ObjUpdateButtonCallback(void* obj) {
    Object* btn = (Object*) obj;
    UIObjData* bdata = &btn->data.ui_data;

    // if nothing to update, exit
    if (!bdata->callback) {
        return;
    }

    Rectangle r = {
        bdata->pos.x,
        bdata->pos.y,
        bdata->size.x,
        bdata->size.y,
    };

    bool mouse_is_hovering = CheckCollisionPointRec(GetMousePosition(), r);
    bool mouse_is_pressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    // click button
    if (mouse_is_hovering && mouse_is_pressed) {
        bdata->callback();
    }
}

void ObjRenderButtonCallback(void* obj) {
    /*
        button colors:
        1 - fill (normal)
        2 - border (normal)
        3 - fill (hover)
        4 - border (hover)
        5 - fill (hover+click)
    */

    Object* btn = (Object*) obj;
    UIObjData* bdata = &btn->data.ui_data;

    Rectangle r = {
        bdata->pos.x,
        bdata->pos.y,
        bdata->size.x,
        bdata->size.y,
    };

    bool mouse_is_hovering = CheckCollisionPointRec(GetMousePosition(), r);
    bool mouse_is_pressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    Color fill_color = (mouse_is_hovering)
        ? (mouse_is_pressed
            ? bdata->color5 
            : bdata->color3)
        : bdata->color1;
    
    Color bg_color = (mouse_is_hovering)
        ? bdata->color4
        : bdata->color2;

    // button border
    DrawRectangleRounded(r, 0.5, 5, bg_color);
    
    r = (Rectangle) {
        bdata->pos.x + 5,
        bdata->pos.y + 5,
        bdata->size.x - 10,
        bdata->size.y - 10,
    };

    // button foreground
    DrawRectangleRounded(r, 0.5, 5, fill_color);

    // if label is empty string, do nothing
    if (!strlen(bdata->label)) {
        return;
    }

    // button label, centered on the button
    int text_width = MeasureText(bdata->label, 20);
    float text_x = bdata->pos.x + text_width / 2;
    float text_y = bdata->pos.y + 20 / 2;

    DrawText(bdata->label, text_x, text_y, 20, BLACK);
}

// OBJ_UI_ELEMENT

void ObjRenderHealthbarCallback(void* obj) {
    Object* hb = (Object*) obj;
    UIObjData* hdata = &hb->data.ui_data;
    EntityObjData* pdata = &game.player->data.ent_data;

    Vector2 abs_pos = GetAbsPosition(hdata->pos);

    Texture2D* tex = GetTexture(
        TextFormat("healthbar_%d", (int) pdata->health / 20));

    DrawTexture(*tex, pdata->pos.x - 73, pdata->pos.y - 75, WHITE);
}

void ObjRenderGameTimerCallback(void* obj) {
    Vector2 abs_pos = GetAbsPosition((Vector2){20, 20});

    const char* time_string = GameTimeStr(game.gameplay_time_elapsed);
    DrawText(time_string, abs_pos.x, abs_pos.y, 40, BLACK);
}

void ObjRenderTimeSurvivedCallback(void* obj) {
    DrawText(TextFormat("You survived %d seconds", (int) game.gameplay_time_elapsed),
        game.screen_size.x / 2 - 50, game.screen_size.y / 2, 50, BLACK);
}

// button callbacks

void BtnStartCallback() {
    LoadScene(SCENE_GAMEPLAY);
}

void BtnSettingsCallback() {
    LoadScene(SCENE_SETTINGS);
}

void BtnRestartCallback() {
    LoadScene(SCENE_GAMEPLAY);
}

void BtnQuitCallback() {
    Quit();
}