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

Object* CreateObject(ObjClass class) {
    ObjType type = GetObjTypeOfClass(class);

    // find first open slot
    int id;
    for (id = 0; id < OBJ_SLOT_COUNT && game.objects[type][id].active; id++);

    Object* o = &game.objects[type][id];

    // insert into first available slot
    *o = (Object) {
        .active = true,
        .id = id,
        .type = type,
        .class = class
    };

    // init type data - just update and render funcs

    if (type == OBJ_TIMER) {
        o->update = ObjUpdateTimerCallback;
        o->render = NULL;
    } else if (type == OBJ_ENTITY_PLAYER) {
        o->update = ObjUpdatePlayerCallback;
        o->render = ObjRenderPlayerCallback;
    } else if (type == OBJ_ENTITY_ENEMY) {
        o->update = ObjUpdateEnemyCallback;
        o->render = ObjRenderEnemyCallback;
    } else if (type == OBJ_ENTITY_BULLET) {
        o->update = ObjUpdateBulletCallback;
        o->render = ObjRenderBulletCallback;
    } else if (type == OBJ_UI_TEXT) {
        o->update = NULL;
        o->render = ObjRenderTextCallback;
    } else if (type == OBJ_UI_BUTTON) {
        o->update = ObjUpdateButtonCallback;
        o->render = ObjRenderButtonCallback;
        o->ui_colors[0] = (Color) { 200, 200, 210, 255 };
        o->ui_colors[1] = (Color) { 150, 150, 160, 255 };
        o->ui_colors[2] = (Color) { 170, 170, 180, 255 };
        o->ui_colors[3] = (Color) { 120, 120, 150, 255 };
        o->ui_colors[4] = (Color) { 160, 160, 170, 255 };
    } else if (type == OBJ_UI_ELEMENT) {
        // each type of element will have different update and render
        // functions
        o->update = NULL;
        o->render = NULL;
    } else {
        printf("update CreateObject type data - L%d (object.c)\n", __LINE__);
    }

    // init class data - everything else

    if (class == OC_STARTSCREEN_UI_TITLE_TEXT) {
        o->pos = (Vector2) { (game.screen_size.x / 2) - 100,
                (game.screen_size.y / 2) - 200};
        o->size = (Vector2) { 200, 50 };
        o->ui_colors[0] = BLACK;
        o->ui_text = "Shooter Game";
        o->ui_font_size = 50;
        o->ui_callback = NULL;
    } else if (class == OC_STARTSCREEN_UI_START_BUTTON) {
        o->pos = (Vector2){ (game.screen_size.x / 2) - 100,
                (game.screen_size.y / 2) + 200 };
        o->size = (Vector2) { 200, 50 };
        o->ui_text = "Start";
        o->ui_font_size = 20;
        o->ui_callback = BtnStartCallback;
    } else if (class == OC_STARTSCREEN_UI_SETTINGS_BUTTON) {
        o->pos = (Vector2){ (game.screen_size.x / 2) - 100,
            (game.screen_size.y / 2) + 275 };
        o->size = (Vector2) { 200, 50 };
        o->ui_text = "Settings";
        o->ui_font_size = 20;
        o->ui_callback = BtnSettingsCallback;
    } else if (class == OC_STARTSCREEN_UI_QUIT_BUTTON) {
        o->pos = (Vector2){ (game.screen_size.x / 2) - 100,
            (game.screen_size.y / 2) + 350 };
        o->size = (Vector2) { 200, 50 };
        o->ui_text = "Quit";
        o->ui_font_size = 20;
        o->ui_callback = BtnQuitCallback;
    } else if (class == OC_ENDSCREEN_UI_RESTART_BUTTON) {
        o->pos = (Vector2){(game.screen_size.x / 2) - 100,
                (game.screen_size.y / 2) + 125};
        o->size = (Vector2) { 200, 50 };
        o->ui_text = "Restart?";
        o->ui_font_size = 20;
        o->ui_callback = BtnRestartCallback;
    } else if (class == OC_GAMEPLAY_ENTITY_PLAYER) {
        o->entity_speed = 5;
        o->entity_max_health = 100;
        o->entity_health = 100;
        o->entity_damage = 0;
        o->entity_hitbox_radius = 25;
    } else if (class == OC_GAMEPLAY_ENTITY_ENEMY_BASIC) {
        o->entity_speed = 1;
        o->entity_max_health = 20;
        o->entity_health = 20;
        o->entity_damage = 20;
        o->entity_hitbox_radius = 25;
    } else if (class == OC_GAMEPLAY_ENTITY_BULLET_BASIC) {
        o->entity_speed = 10;
        o->entity_damage = 10;
        o->entity_hitbox_radius = 20;
        o->entity_lifetime = 30;
    } else if (class == OC_GAMEPLAY_TIMER_PLAYER_SHOOT_BULLET_BASIC) {
        o->timer_last_recorded = GetTime();
        o->timer_callback = TimerPlayerShootBasicBulletCallback;
        o->timer_interval = 2.0;
        o->timer_num_triggers = -1;
    } else if (class == OC_GAMEPLAY_TIMER_SPAWN_ENEMY_BASIC) {
        o->timer_last_recorded = GetTime();
        o->timer_callback = TimerSpawnBasicEnemyCallback;
        o->timer_interval = 0.1;
        o->timer_num_triggers = -1;
    } else if (class == OC_GAMEPLAY_UI_ELEMENT_HEALTHBAR) {
        o->update = NULL;
        o->render = ObjRenderHealthbarCallback;
    } else if (class == OC_GAMEPLAY_UI_ELEMENT_GAME_TIMER) {
        o->update = NULL;
        o->render = ObjRenderGameTimerCallback;
    } else if (class == OC_ENDSCREEN_UI_ELEMENT_TIME_SURVIVED) {
        o->update = NULL;
        o->render = ObjRenderTimeSurvivedCallback;
    } else {
        printf("update CreateObject class data - L%d (object.c)\n", __LINE__);
    }

    // printf(
    //     "created object\n"
    //     "\ttype: %d,\n"
    //     "\tclass: %d,\n"        
    //     "\tactive? %d,\n"        
    //     "\tupdate(): %p,\n"        
    //     "\trender(): %p,\n"
    //     "\tpos: (%lf, %lf)\n"
    //     "\tsize: (%lf, %lf)\n"
    //     "\ttimer_cb(): %p,\n",
    //     o->type,
    //     o->class,
    //     o->active,
    //     o->update,
    //     o->render,
    //     o->pos.x,
    //     o->pos.y,
    //     o->size.x,
    //     o->size.y,
    //     o->timer_callback        
    // );

    return o;
}

Object* GetObject(ObjType type, int id) {
    return &game.objects[type][id];
}

// keeps rest of ids the same
void DeleteObject(ObjType type, int id) {
    game.objects[type][id].active = false;
    // now next time an object gets added it will overwrite this slot
}

ObjType GetObjTypeOfClass(ObjClass class) {
    switch(class) {

        case OC_STARTSCREEN_UI_TITLE_TEXT:
            return OBJ_UI_TEXT;
        
        case OC_STARTSCREEN_UI_START_BUTTON:
        case OC_STARTSCREEN_UI_SETTINGS_BUTTON:
        case OC_STARTSCREEN_UI_QUIT_BUTTON:
        case OC_ENDSCREEN_UI_RESTART_BUTTON:
            return OBJ_UI_BUTTON;
        
        case OC_GAMEPLAY_ENTITY_PLAYER:
            return OBJ_ENTITY_PLAYER;
        
        case OC_GAMEPLAY_ENTITY_ENEMY_BASIC:
            return OBJ_ENTITY_ENEMY;
        
        case OC_GAMEPLAY_ENTITY_BULLET_BASIC:
            return OBJ_ENTITY_BULLET;
        
        case OC_GAMEPLAY_TIMER_PLAYER_SHOOT_BULLET_BASIC:
        case OC_GAMEPLAY_TIMER_SPAWN_ENEMY_BASIC:
            return OBJ_TIMER;
        
        case OC_GAMEPLAY_UI_ELEMENT_HEALTHBAR:
        case OC_GAMEPLAY_UI_ELEMENT_GAME_TIMER:
        case OC_ENDSCREEN_UI_ELEMENT_TIME_SURVIVED:
            return OBJ_UI_ELEMENT;
            
        default:
            printf("update GetObjTypeOfClass - L%d (object.c)\n", __LINE__);
            return -1;
    }
}

#endif // OBJECT_C