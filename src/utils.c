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

Object* CreateObject(ObjType type) {
    Object o;
    switch(type) {
    case OBJ_ENTITY_PLAYER:
        o = (Object){
            .type = OBJ_ENTITY_PLAYER,
            .update = ObjUpdatePlayerCallback,
            .render = ObjRenderPlayerCallback,
            .data = (ObjData) {
                .ent_data = {
                    .speed = 2,
                    .max_health = 100,
                    .health = 100,
                    .damage = 0,
                    .hitbox_radius = 5
                }
            }
        };
        break;
    }
    g_array_append_val(game.objects[type], o);
    return (Object*) &game.objects[type][game.objects[type]->len - 1];
}

void UpdateObjects() {
    for (int type = 0; type < OBJ_COUNT; type++) {
        for (int i = 0; i < game.objects[type]->len; i++) {
            Object* o = (Object*) game.objects[type]->data[i];

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
        for (int i = 0; i < game.objects[type]->len; i++) {
            Object* o = (Object*) game.objects[type]->data[i];

            // skip if render = NULL
            if (!o->render) {
                continue;
            }

            // pass ptr so it can access itself
            o->render(o);
        }
    }
}

void DeleteObjects() {
    for (int type = 0; type < OBJ_COUNT; type++) {
        g_array_unref(game.objects[type]);
    }
}

void CreateTimer(TimerCallback fn, double interval, int num_triggers) {
    Timer* t = malloc(sizeof(Timer));
    *t = (Timer) {
        .interval = interval,
        .num_triggers = num_triggers,
        .fn = fn,
        .last_recorded = GetTime()
    };
    g_ptr_array_add(game.timers, t);
}

// returns true if timer is out of triggers
bool UpdateTimer(Timer* t) {
    double now = GetTime();
    if (now - t->last_recorded < t->interval) {
        return false;
    }

    t->last_recorded = now;
    t->fn();

    if (t->num_triggers > -1) {
        t->num_triggers--;
    }

    return t->num_triggers == 0;
}

void UpdateTimers() {
    for (int i = 0; i < game.timers->len; i++) {
        if (UpdateTimer(game.timers->pdata[i])) {
            g_ptr_array_remove_index_fast(game.timers, i);
        }
    }
}