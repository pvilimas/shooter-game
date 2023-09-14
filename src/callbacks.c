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
    int dx = abs_mouse_pos.x - game.player->pos.x;
    int dy = abs_mouse_pos.y - game.player->pos.y;
    double angle = atan2(dy, dx);

    for (int i = 0; i < 5; i++) {
        Object* o = CreateObject(OC_GAMEPLAY_ENTITY_BULLET_BASIC);
        o->pos = game.player->pos;
        o->entity_angle = RandFloat(angle - 0.125, angle + 0.125);
    }
}

void TimerSpawnBasicEnemyCallback() {
    int enemy_spawn_radius = sqrt(2 * (pow(game.screen_size.x, 2)
        + pow(game.screen_size.y, 2)));
    Vector2 camera_offset = GetCameraOffset();
    Vector2 pos = {
        GetRandomValue(camera_offset.x - enemy_spawn_radius,
            camera_offset.x + enemy_spawn_radius),
        GetRandomValue(camera_offset.y - enemy_spawn_radius,
            camera_offset.y + enemy_spawn_radius)};

    // repick until chosen point is offscreen by a certain radius
    while (PointNearScreen(pos, 10)) {
        pos = (Vector2){
            GetRandomValue(camera_offset.x - enemy_spawn_radius,
                camera_offset.x + enemy_spawn_radius),
            GetRandomValue(camera_offset.y - enemy_spawn_radius,
                camera_offset.y + enemy_spawn_radius)};
    }
    
    int dx = game.player->pos.x - pos.x;
    int dy = game.player->pos.y - pos.y;
    double angle = atan2(dy, dx);
    
    Object* o = CreateObject(OC_GAMEPLAY_ENTITY_ENEMY_BASIC);
    o->pos = pos;
    o->entity_angle = angle;
}

// OBJ_TIMER

void ObjUpdateTimerCallback(void* obj) {
    double now = GetTime();
    Object* o = (Object*) obj;

    if (now - o->timer_last_recorded < o->timer_interval) {
        return;
    }

    o->timer_last_recorded = now;
    o->timer_callback();

    if (o->timer_num_triggers > -1) {
        o->timer_num_triggers--;
    }

    if (o->timer_num_triggers == 0) {
        DeleteObject(o->type, o->id);
    }
}

// OBJ_ENTITY_PLAYER

void ObjUpdatePlayerCallback(void* obj) {
    if (game.player->entity_iframes != 0) {
        game.player->entity_iframes--;
    }
}

void ObjRenderPlayerCallback(void* obj) {
    if (game.player->entity_iframes % 20 < 10) {
        DrawCircle(game.player->pos.x, game.player->pos.y, 25.0f, BLACK);
    }
}

// OBJ_ENTITY_ENEMY

void ObjUpdateEnemyCallback(void* obj) {
    Object* enemy = (Object*) obj;

    // track player
    int dx = game.player->pos.x - enemy->pos.x;
    int dy = game.player->pos.y - enemy->pos.y;
    enemy->entity_angle = atan2(dy, dx);

    enemy->pos.x += cos(enemy->entity_angle) * enemy->entity_speed;
    enemy->pos.y += sin(enemy->entity_angle) * enemy->entity_speed;

    // check collision between enemy and player
    bool player_takes_dmg = !game.player->entity_iframes &&
        CheckCollisionCircles(enemy->pos, enemy->entity_hitbox_radius,
            game.player->pos, game.player->entity_hitbox_radius);
    if (player_takes_dmg) {
        game.player->entity_health -= enemy->entity_damage;
        game.player->entity_iframes = 120;
    }
    if (game.player->entity_health <= 0) {
        KillPlayer();
    }

    // check collision between enemy and bullet
    for (int j = 0; j < OBJ_SLOT_COUNT; j++) {
        Object* bullet = GetObject(OBJ_ENTITY_BULLET, j);

        if (!bullet->active) {
            continue;
        }

        if (Vector2Distance(enemy->pos, bullet->pos) > 50) {
            continue;
        }

        bool enemy_takes_dmg = CheckCollisionCircles(
            enemy->pos, enemy->entity_hitbox_radius,
            bullet->pos, bullet->entity_hitbox_radius);
        if (enemy_takes_dmg) {
            enemy->entity_health -= bullet->entity_damage;
            DeleteObject(bullet->type, bullet->id);
        }
    }

    // remove if dead
    if (enemy->entity_health <= 0) {
        DeleteObject(enemy->type, enemy->id);
    }
}

void ObjRenderEnemyCallback(void* obj) {
    Object* enemy = (Object*) obj;

    DrawCircle(enemy->pos.x, enemy->pos.y, 25, BLACK);
    DrawCircle(enemy->pos.x, enemy->pos.y, 22, RED);
}

// OBJ_ENTITY_BULLET

void ObjUpdateBulletCallback(void* obj) {
    Object* bullet = (Object*) obj;
    
    bullet->pos.x += cos(bullet->entity_angle) * bullet->entity_speed;
    bullet->pos.y += sin(bullet->entity_angle) * bullet->entity_speed;
    bullet->entity_lifetime--;

    if (bullet->entity_lifetime == 0) {
        DeleteObject(bullet->type, bullet->id);
    }
}

void ObjRenderBulletCallback(void* obj) {
    Object* bullet = (Object*) obj;

    float c = cos(bullet->entity_angle);
    float s = sin(bullet->entity_angle);

    DrawLineEx(
        (Vector2){ bullet->pos.x - c * 10, bullet->pos.y - s * 10 },
        (Vector2){ bullet->pos.x + c * 10, bullet->pos.y + s * 10 },
        4.0f, BLACK);
    DrawLineEx(
        (Vector2){ bullet->pos.x - c * 8, bullet->pos.y - s * 8 },
        (Vector2){ bullet->pos.x + c * 8, bullet->pos.y + s * 8 },
        3.0f, WHITE);
}

// OBJ_UI_TEXT

void ObjRenderTextCallback(void* obj) {
    Object* text = (Object*) obj;
    
    // text only uses color1

    DrawText(text->ui_text, text->pos.x, text->pos.y,
        text->ui_font_size, text->ui_colors[0]);
}

// OBJ_UI_BUTTON

void ObjUpdateButtonCallback(void* obj) {
    Object* button = (Object*) obj;

    // if nothing to update, exit
    if (!button->ui_callback) {
        return;
    }

    Rectangle r = {
        button->pos.x,
        button->pos.y,
        button->size.x,
        button->size.y,
    };

    bool mouse_is_hovering = CheckCollisionPointRec(GetMousePosition(), r);
    bool mouse_is_pressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    // click button
    if (mouse_is_hovering && mouse_is_pressed) {
        button->ui_callback();
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

    Object* button = (Object*) obj;

    Rectangle r = {
        button->pos.x,
        button->pos.y,
        button->size.x,
        button->size.y,
    };

    bool mouse_is_hovering = CheckCollisionPointRec(GetMousePosition(), r);
    bool mouse_is_pressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    Color fill_color = (mouse_is_hovering)
        ? (mouse_is_pressed
            ? button->ui_colors[4] 
            : button->ui_colors[2])
        : button->ui_colors[0];
    
    Color bg_color = (mouse_is_hovering)
        ? button->ui_colors[3]
        : button->ui_colors[1];

    // button border
    DrawRectangleRounded(r, 0.5, 5, bg_color);
    
    r = (Rectangle) {
        button->pos.x + 5,
        button->pos.y + 5,
        button->size.x - 10,
        button->size.y - 10,
    };

    // button foreground
    DrawRectangleRounded(r, 0.5, 5, fill_color);

    // if label is empty string, do nothing
    if (!strlen(button->ui_text)) {
        return;
    }

    // button label, centered on the button
    int text_width = MeasureText(button->ui_text, 20);
    float text_x = button->pos.x + text_width / 2;
    float text_y = button->pos.y + 20 / 2;

    DrawText(button->ui_text, text_x, text_y, 20, BLACK);
}

// OBJ_UI_ELEMENT

void ObjRenderHealthbarCallback(void* obj) {
    Object* healthbar = (Object*) obj;

    Vector2 abs_pos = GetAbsPosition(healthbar->pos);
    const char* tex_name = TextFormat("healthbar_%d",
        (int) game.player->entity_health / 20);
    Texture2D* tex = GetTexture(tex_name);

    DrawTexture(*tex, game.player->pos.x - 73, game.player->pos.y - 75, WHITE);
}

void ObjRenderGameTimerCallback(void* obj) {
    Vector2 abs_pos = GetAbsPosition((Vector2){20, 20});

    const char* time_string = GameTimeStr(game.gameplay_time_elapsed);
    DrawText(time_string, abs_pos.x, abs_pos.y, 40, BLACK);
}

void ObjRenderTimeSurvivedCallback(void* obj) {
    DrawText(
        TextFormat("You survived %d seconds", (int)game.gameplay_time_elapsed),
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

// keybinds

void KeybindMoveUpCallback() {
    game.player->pos.y -= game.player->entity_speed;
}

void KeybindMoveDownCallback() {
    game.player->pos.y += game.player->entity_speed;
}

void KeybindMoveLeftCallback() {
    game.player->pos.x -= game.player->entity_speed;
}

void KeybindMoveRightCallback() {
    game.player->pos.x += game.player->entity_speed;
}

