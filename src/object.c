#ifndef OBJECT_C
#define OBJECT_C

#include "game.h"

void UpdateObjects() {
    for (int type = 0; type < OBJ_TYPE_COUNT; type++) {
        for (int i = 0; i < OBJ_SLOT_COUNT; i++) {
            Object* o = GetObject(type, i);

            if (!o->active) {
                continue;
            }

            // skip if update = NULL
            if (!o->update) {
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

            if (!o->active) {
                continue;
            }

            // skip if render = NULL
            if (!o->render) {
                continue;
            }

            // pass ptr so it can access itself
            o->render(o);
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
}

#endif // OBJECT_C