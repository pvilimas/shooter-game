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

#define OBJECT_SLOTS 10000

#define DEBUGGING 1

// the types of entities
typedef struct {
    Vector2 pos;
    int speed; // 3
    int health; // 5
    int iframes; // # invincibility frames remaining
    int hitbox_radius;
} Player;

typedef struct {
    Vector2 pos;
    float angle; // 0 = ->
    int speed; // 2
    int health; // 1
    int hitbox_radius;
} Enemy;

typedef struct {
    Vector2 pos;
    float angle;
    int speed; // 10
    int lifetime; // decrements each frame to 0
    int damage; // 1
    int hitbox_radius;
} Bullet;

// timer class
typedef void (*TimerCallback)();

// TODO fix num_triggers
typedef struct {
    double interval;
    int num_triggers; // -1 = inf
    TimerCallback fn;
    double last_recorded;
} Timer;

typedef enum {
    SCENE_STARTSCREEN,
    SCENE_GAMEPLAY,
    SCENE_ENDSCREEN
} Scene;

typedef enum {
    ENT_PLAYER,
    ENT_ENEMY,
    ENT_BULLET,

    ENT_COUNT
} EntityType;

typedef struct {
    int speed;          // how much it moves by each frame
    float angle;        // direction of travel  
    int max_health;
    int damage;         // how much damage it does (to the player or enemies)
    int iframes;        // # invincibility frames, counts down each frame
    int lifetime;       // # frames it lasts, counts down each frame
    int hitbox_radius;  // radius of circular hitbox
} EntityDefaults;

// defaults/initial settings for each entity type
extern EntityDefaults entity_defaults[ENT_COUNT];

// object - any game element
// game has limited # of object slots
// each object has 2 methods: update and render (each called once per frame)

typedef enum {
    OBJ_ENTITY,         // any in-game entity like player, bullet, enemy
    OBJ_UI,             // any ui element - buttons, text
    OBJ_TIMER           // timer object
} ObjectType;

// the type of object.update and object.render
// object is passed through a voidptr
typedef void (*ObjCallback)(void* object);

// data that all entities have
typedef struct {
    EntityType  type;
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

// data for timers
typedef struct {
    double          interval;
    int             num_triggers; // -1 = inf
    TimerCallback   fn;
    double          last_recorded;
} TimerObjData;

typedef union {
    EntityObjData   ent_data;
    UIObjData       ui_data;
    TimerObjData    timer_data;
} ObjData;

// a game object (id = slot #)
typedef struct {
    int id;
    ObjectType type;
    ObjCallback update;
    ObjCallback render;

    ObjData data;
} Object;

// the game class
typedef struct {
    Vector2     screen_size;
    Camera2D    camera;
    Player      player;

    // TODO change to GArray
    GPtrArray*  enemies;        // <Enemy*>
    GPtrArray*  bullets;        // <Bullet*>
    GPtrArray*  timers;         // <Timer*>

    GHashTable* textures;       // <cchar*, Texture2D*>
    GHashTable* fonts;          // <cchar*, Font*>

    Scene       current_scene;
    int         frame_count;    // increment every frame

    GPtrArray*  objects;        // <Object*>
} Game;

// global instance of the game
extern Game game;

#endif // TYPES_H