#ifndef OBJDATA_H
#define OBJDATA_H

// this file contains a macro to create each game object
// (basically subtypes)

// macro name format: CREATE_OBJ_<SCENE>_<NAME>
// scene can be anything from Scene enum in types.h
// names are unique within scenes

// OBJ_STARTSCREEN_*

#define CREATE_OBJ_STARTSCREEN_TITLE_TEXT() \
    Object* o = CreateObject(OBJ_UI_TEXT);  \
    UIObjData* obj_data = &o->data.ui_data; \
    obj_data->pos = (Vector2) { (game.screen_size.x / 2) - 100, \
        (game.screen_size.y / 2) - 200}; \
    obj_data->size = (Vector2) { 200, 30 }; \
    obj_data->color1 = BLACK; \
    obj_data->label = "Shooter Game"; \
    obj_data->font_size = 20; \
    obj_data->callback = NULL

#define CREATE_OBJ_STARTSCREEN_START_BUTTON() \
    o = CreateObject(OBJ_UI_BUTTON); \
    obj_data = &o->data.ui_data; \
    obj_data->pos = (Vector2){ (game.screen_size.x / 2) - 100, \
        (game.screen_size.y / 2) + 200 }; \
    obj_data->size = (Vector2) { 200, 50 }; \
    obj_data->color1 = (Color) { 200, 200, 210, 255 }; \
    obj_data->color2 = (Color) { 150, 150, 160, 255 }; \
    obj_data->color3 = (Color) { 170, 170, 180, 255 }; \
    obj_data->color4 = (Color) { 120, 120, 150, 255 }; \
    obj_data->color5 = (Color) { 160, 160, 170, 255 }; \
    obj_data->label = "Start"; \
    obj_data->font_size = 20; \
    obj_data->callback = BtnStartCallback

#define CREATE_OBJ_STARTSCREEN_SETTINGS_BUTTON() \
    o = CreateObject(OBJ_UI_BUTTON); \
    obj_data = &o->data.ui_data; \
    obj_data->pos = (Vector2){ (game.screen_size.x / 2) - 100, \
        (game.screen_size.y / 2) + 275 }; \
    obj_data->size = (Vector2) { 200, 50 }; \
    obj_data->color1 = (Color) { 200, 200, 210, 255 }; \
    obj_data->color2 = (Color) { 150, 150, 160, 255 }; \
    obj_data->color3 = (Color) { 170, 170, 180, 255 }; \
    obj_data->color4 = (Color) { 120, 120, 150, 255 }; \
    obj_data->color5 = (Color) { 160, 160, 170, 255 }; \
    obj_data->label = "Settings"; \
    obj_data->font_size = 20; \
    obj_data->callback = BtnSettingsCallback

// OBJ_GAMEPLAY_*

#define CREATE_OBJ_GAMEPLAY_PLAYER() \
    game.player = CreateObject(OBJ_ENTITY_PLAYER)

#define CREATE_OBJ_GAMEPLAY_TIMER_PLAYER_SHOOT_BASIC() \
    Object* o = CreateObject(OBJ_TIMER); \
    o->data.tm_data.callback = PlayerShootAtMouseCallback; \
    o->data.tm_data.interval = 2.0; \
    o->data.tm_data.num_triggers = -1

#define CREATE_OBJ_GAMEPLAY_TIMER_SPAWN_BASIC() \
    o = CreateObject(OBJ_TIMER); \
    o->data.tm_data.callback = SpawnEnemyCallback; \
    o->data.tm_data.interval = 0.1; \
    o->data.tm_data.num_triggers = -1

#define CREATE_OBJ_GAMEPLAY_UI_HEALTHBAR() \
    CreateObject(OBJ_UI_HEALTHBAR)

// OBJ_ENDSCREEN_*

#define CREATE_OBJ_ENDSCREEN_RESTART_BUTTON() \
    Object* o = CreateObject(OBJ_UI_BUTTON); \
    o->data.ui_data.pos = (Vector2){(game.screen_size.x / 2) - 100, \
        (game.screen_size.y / 2) - 25}; \
    o->data.ui_data.size = (Vector2) { 200, 50 }; \
    o->data.ui_data.color1 = (Color) { 200, 200, 200, 255 }; \
    o->data.ui_data.color2 = (Color) { 150, 150, 150, 255 }; \
    o->data.ui_data.color3 = (Color) { 170, 170, 170, 255 }; \
    o->data.ui_data.color4 = (Color) { 120, 120, 120, 255 }; \
    o->data.ui_data.color5 = (Color) { 160, 160, 160, 255 }; \
    o->data.ui_data.callback = BtnRestartCallback; \
    o->data.ui_data.label = "Restart"

#endif // OBJDATA_H