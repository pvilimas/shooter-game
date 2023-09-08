#ifndef OBJECT_C
#define OBJECT_C

#include "game.h"

/*
    object.c - managing object slots
    - there are OBJ_TYPE_COUNT types, and each has OBJ_SLOT_COUNT slots
    - everything in the game is an object: player, enemies, buttons, etc
*/

void UpdateObjects() {
    for (int type = 0; type < OBJ_TYPE_COUNT; type++) {
        for (int i = 0; i < OBJ_SLOT_COUNT; i++) {
            Object* o = GetObject(type, i);

            // skip if unloaded or update = NULL
            if (!o->active || !o->update) {
                continue;
            }

            // pass ptr so it can modify itself
            o->update(o);
        }
    }
}

void RenderObjects() {
    for (int type = 0; type < OBJ_TYPE_COUNT; type++) {
        for (int i = 0; i < OBJ_SLOT_COUNT; i++) {
            Object* o = GetObject(type, i);

            // skip if unloaded or render = NULL
            if (!o->active || !o->render) {
                continue;
            }

            // pass ptr so it can access itself
            o->render(o);
        }
    }
}

void DeleteObjects() {
    for (int type = 0; type < OBJ_TYPE_COUNT; type++) {
        for (int id = 0; id < OBJ_SLOT_COUNT; id++) {
            game.objects[type][id].active = false;
        }
    }
}

Object* CreateObject(ObjType type) {
    // find first open slot
    int id;
    for (id = 0; id < OBJ_SLOT_COUNT && game.objects[type][id].active; id++);

    Object o = (Object) {
        .active = true,
        .id = id,
        .type = type,
        .data = {}
    };
    switch(type) {
    case OBJ_TIMER:
        o.update = ObjUpdateTimerCallback;
        o.render = NULL;
        o.data.tm_data = (TimerObjData) {
            .last_recorded = GetTime()
        };
        break;
    case OBJ_ENTITY_PLAYER:
        o.update = ObjUpdatePlayerCallback;
        o.render = ObjRenderPlayerCallback;
        o.data.ent_data = (EntityObjData) {
            .speed = 2,
            .max_health = 100,
            .health = 100,
            .damage = 0,
            .hitbox_radius = 5
        };
        break;
    case OBJ_ENTITY_ENEMY:
        o.update = ObjUpdateEnemyCallback;
        o.render = ObjRenderEnemyCallback;
        o.data.ent_data = (EntityObjData) {
            .speed = 1,
            .max_health = 20,
            .health = 20,
            .damage = 20,
            .hitbox_radius = 25
        };
        break;
    case OBJ_ENTITY_BULLET:
        o.update = ObjUpdateBulletCallback;
        o.render = ObjRenderBulletCallback;
        o.data.ent_data = (EntityObjData) {
            .speed = 10,
            .damage = 10,
            .hitbox_radius = 10,
            .lifetime = 30
        };
        break;
    case OBJ_UI_TEXT:
        o.update = NULL;
        o.render = ObjRenderTextCallback;
        break;
    case OBJ_UI_BUTTON:
        o.update = ObjUpdateButtonCallback;
        o.render = ObjRenderButtonCallback;
        o.data.ui_data = (UIObjData) {
            .label = "",
            .callback = NULL
        };
        break;
    case OBJ_UI_HEALTHBAR:
        o.update = NULL;
        o.render = ObjRenderHealthbarCallback;
        o.data.ui_data = (UIObjData) {
            .pos = {20, 20},
            .size = {200, 43}
        };
        break;
    }

    // insert into first available slot
    game.objects[type][id] = o;
    return &game.objects[type][id];
}

Object* GetObject(ObjType type, int id) {
    return &game.objects[type][id];
}

// keeps rest of ids the same
void DeleteObject(ObjType type, int id) {
    game.objects[type][id].active = false;
    // now next time an object gets added it will overwrite this slot
}

#endif // OBJECT_C