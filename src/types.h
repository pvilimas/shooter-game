#ifndef TYPES_H
#define TYPES_H

#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "glib.h"
#include "raylib.h"
#include "raymath.h"

#include "dev.h"

#define WINDOW_TITLE    "Shooter Game"
#define DEV_MODE        true
#define OBJ_SLOT_COUNT  1000

typedef void (*KeybindCallback)();
typedef void (*ObjCallback)(void* object);
typedef void (*TimerCallback)();
typedef void (*UICallback)();

typedef struct {
    const char*     name;
    int             key_code;
    KeybindCallback callback;
    int             delay_ms; // -1 = one time, 0 = hold, >0 = hold with delay
} Keybind;

typedef enum {
    SCENE_STARTUP,
    SCENE_STARTSCREEN,
    SCENE_SETTINGS,
    SCENE_GAMEPLAY,
    SCENE_ENDSCREEN
} Scene;

// object - any game element
// game has limited # of object slots
// each object has 2 methods: update and render (each called once per frame)

// object type
typedef enum {
    OBJ_TIMER = 0,

    OBJ_ENTITY_PLAYER = 1,
    OBJ_ENTITY_ENEMY = 2,
    OBJ_ENTITY_BULLET = 3,
    
    OBJ_UI_TEXT = 4,
    OBJ_UI_BUTTON = 5,
    OBJ_UI_ELEMENT = 6,

    OBJ_TYPE_COUNT
} ObjType;

// object class - like a subtype
// naming scheme: OC_<SCENE>_<TYPE>_<NAME>
typedef enum {
    OC_STARTSCREEN_UI_TITLE_TEXT = 0,
    OC_STARTSCREEN_UI_START_BUTTON = 1,
    OC_STARTSCREEN_UI_SETTINGS_BUTTON = 2,
    OC_STARTSCREEN_UI_QUIT_BUTTON = 3,

    OC_GAMEPLAY_ENTITY_PLAYER = 4,
    OC_GAMEPLAY_ENTITY_ENEMY_BASIC = 5,
    OC_GAMEPLAY_ENTITY_BULLET_BASIC = 6,
    OC_GAMEPLAY_TIMER_PLAYER_SHOOT_BULLET_BASIC = 7,
    OC_GAMEPLAY_TIMER_SPAWN_ENEMY_BASIC = 8,
    OC_GAMEPLAY_UI_ELEMENT_HEALTHBAR = 9,
    OC_GAMEPLAY_UI_ELEMENT_GAME_TIMER = 10,
    
    OC_ENDSCREEN_UI_ELEMENT_TIME_SURVIVED = 11,
    OC_ENDSCREEN_UI_RESTART_BUTTON = 12,

    OC_COUNT
} ObjClass;

// a game object
typedef struct {
    int             id;         // local to each sublist
    ObjType         type;
    ObjClass        class;
    bool            active;     // if false it's empty

    ObjCallback     update;
    ObjCallback     render;

    // data

    Vector2         pos;
    Vector2         size;

    double          timer_interval;
    double          timer_num_triggers;     // -1 = inf
    TimerCallback   timer_callback;
    double          timer_last_recorded;

    int             entity_speed;
    float           entity_angle;
    int             entity_max_health;
    int             entity_health;
    int             entity_damage;
    int             entity_iframes;
    int             entity_lifetime;
    int             entity_hitbox_radius;

    Color           ui_colors[10];
    const char*     ui_text;
    int             ui_font_size;
    UICallback      ui_callback;            // on-click function
} Object;

// game type

typedef struct {
    Vector2         screen_size;
    Camera2D        camera;
    Object*         player;

    bool            paused;
    Scene           current_scene;
    int             frame_count;            // increment every frame
    double          gameplay_time_elapsed;  // since user clicked start (sec)
    int             current_monitor;        // for checking if window was moved

    GHashTable*     textures;               // <cchar*, Texture2D*>
    GHashTable*     fonts;                  // <cchar*, Font*>
    GHashTable*     keybinds;               // <cchar*, Keybind*>

    RenderTexture2D render_texture;         // for pause screen backdrop

    Object          objects[OBJ_TYPE_COUNT][OBJ_SLOT_COUNT];
    char            buffer[1000];
} Game;

// global instance of the game
extern Game game;

#endif // TYPES_H