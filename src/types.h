#ifndef TYPES_H
#define TYPES_H

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "glib.h"
#include "raylib.h"
#include "raymath.h"

#define WINDOW_TITLE "Shooter Game"

#define DEBUG printf("\t%s: %d\n", __FILE__, __LINE__)

#define DEBUGGING 1

typedef enum {
    SCENE_STARTSCREEN,
    SCENE_GAMEPLAY,
    SCENE_ENDSCREEN
} Scene;

// object - any game element
// game has limited # of object slots
// each object has 2 methods: update and render (each called once per frame)

#define OBJ_SLOT_COUNT 1000

typedef enum {
    OBJ_TIMER,

    OBJ_ENTITY_PLAYER,
    OBJ_ENTITY_ENEMY,
    OBJ_ENTITY_BULLET,
    
    OBJ_UI_BUTTON,
    OBJ_UI_TEXT,
    OBJ_UI_HEALTHBAR,

    OBJ_TYPE_COUNT
} ObjType;

// the type of object.update and object.render
// object is passed through a voidptr
typedef void (*ObjCallback)(void* object);

// data that all entities have
typedef struct {
    Vector2     pos;
    int         speed;
    float       angle;
    int         max_health;
    int         health;
    int         damage;
    int         iframes;
    int         lifetime;
    int         hitbox_radius;
} EntityObjData;

// data that all ui elements have
typedef struct {
    Vector2 pos;
} UIObjData;

typedef void (*TimerCallback)();

// data for timers
typedef struct {
    double          interval;
    int             num_triggers; // -1 = inf
    TimerCallback   callback;
    double          last_recorded;
} TimerObjData;

typedef union {
    EntityObjData   ent_data;
    UIObjData       ui_data;
    TimerObjData    tm_data;
} ObjData;

// a game object
typedef struct {
    bool        active;     // if false it's empty
    int         id;         // local to each sublist
    ObjType     type;
    ObjCallback update;
    ObjCallback render;
    ObjData     data;
} Object;

// the game class
typedef struct {
    Vector2     screen_size;
    Camera2D    camera;
    Object*     player;

    Scene       current_scene;
    int         frame_count;            // increment every frame

    GHashTable* textures;               // <cchar*, Texture2D*>
    GHashTable* fonts;                  // <cchar*, Font*>

    Object      objects[OBJ_TYPE_COUNT][OBJ_SLOT_COUNT];
} Game;

// global instance of the game
extern Game game;

#endif // TYPES_H