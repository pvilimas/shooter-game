#ifndef TYPES_H
#define TYPES_H

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "glib.h"
#include "raylib.h"
#include "raymath.h"

#define WINDOW_TITLE "Shooter Game"

#define DEBUG printf("\t%s: %d\n", __FILE__, __LINE__)
#define DEBUGGING 1

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

#define OBJ_SLOT_COUNT 1000

// object type
typedef enum {
    OBJ_TIMER,

    OBJ_ENTITY_PLAYER,
    OBJ_ENTITY_ENEMY,
    OBJ_ENTITY_BULLET,
    
    OBJ_UI_TEXT,
    OBJ_UI_BUTTON,
    OBJ_UI_ELEMENT,

    OBJ_TYPE_COUNT
} ObjType;

// object class - like a subtype
// naming scheme: OC_<SCENE>_<TYPE>_<NAME>
typedef enum {
    OC_STARTSCREEN_UI_TITLE_TEXT,
    OC_STARTSCREEN_UI_START_BUTTON,
    OC_STARTSCREEN_UI_SETTINGS_BUTTON,
    OC_STARTSCREEN_UI_QUIT_BUTTON,

    OC_GAMEPLAY_ENTITY_PLAYER,
    OC_GAMEPLAY_ENTITY_ENEMY_BASIC,
    OC_GAMEPLAY_ENTITY_BULLET_BASIC,
    OC_GAMEPLAY_TIMER_PLAYER_SHOOT_BULLET_BASIC,
    OC_GAMEPLAY_TIMER_SPAWN_ENEMY_BASIC,
    OC_GAMEPLAY_UI_ELEMENT_HEALTHBAR,
    OC_GAMEPLAY_UI_ELEMENT_GAME_TIMER,
    
    OC_ENDSCREEN_UI_ELEMENT_TIME_SURVIVED,
    OC_ENDSCREEN_UI_RESTART_BUTTON,

    OC_COUNT
} ObjClass;

// the type of object.update and object.render
// object is passed through a voidptr
typedef void (*ObjCallback)(void* object);
typedef void (*TimerCallback)();
typedef void (*UICallback)();

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
    Shape           entity_hitbox;

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

    GHashTable*     textures;               // <cchar*, Texture2D*>
    GHashTable*     fonts;                  // <cchar*, Font*>

    RenderTexture   render_texture;         // for pause screen backdrop

    Object          objects[OBJ_TYPE_COUNT][OBJ_SLOT_COUNT];
    char            buffer[1000];
} Game;

// global instance of the game
extern Game game;

#endif // TYPES_H