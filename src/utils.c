#include "game.h"

/*
    utils.c - math and utils functions
*/

// offset of the camera from the origin
Vector2 GetCameraOffset() {
    return (Vector2){
        game.camera.target.x - GetScreenWidth() / 2,
        game.camera.target.y - GetScreenHeight() / 2
    };
}

// absolute mouse position, accounting for camera offset
Vector2 GetAbsMousePosition() {
    return Vector2Add(GetMousePosition(), GetCameraOffset());
}

// returns true if p is onscreen
bool PointOnScreen(Vector2 p) {
    return PointNearScreen(p, 0.0f);
}

// returns true if p is onscreen or within range pixels of screen edge
// negative range works as expected
bool PointNearScreen(Vector2 p, float range) {
    return p.x >= game.camera.target.x + range - GetScreenWidth() / 2
        && p.x <= game.camera.target.x - range + GetScreenWidth() / 2
        && p.y >= game.camera.target.y + range - GetScreenHeight() / 2
        && p.y <= game.camera.target.y - range + GetScreenHeight() / 2;
}

int RandInt(int min, int max) {
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

float RandFloat(float min, float max) {
    return min + (float)rand() / ((float)(RAND_MAX / (max-min)));
}

void UpdateObjects() {
    for (int type = 0; type < OBJ_COUNT; type++) {
        for (int i = 0; i < OBJECT_SLOTS; i++) {
            Object* o = GetObject(type, i);

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
    for (int type = 0; type < OBJ_COUNT; type++) {
        for (int i = 0; i < OBJECT_SLOTS; i++) {
            Object* o = GetObject(type, i);

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
    for (id = 0; id < OBJECT_SLOTS && game.objects[type][id].active; id++);

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
            .speed = 20,
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
