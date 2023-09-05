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

// object - any game element
// game has limited # of object slots
// each object has 1 method: update (called once per frame)

typedef enum {
    OBJ_ENTITY,     // any in-game entity like player, bullet, enemy
    OBJ_UI,         // any ui element - buttons, text
    OBJ_TIMER       // timer object
} ObjectType;

typedef void (*ObjCallback)(struct _object*);

typedef struct _object {
    ObjectType type;
    ObjCallback update;
} Object;

#define OBJECT_SLOTS 10000

// the game class
typedef struct _game {
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

    // Object objects[OBJECT_SLOTS];
} Game;

// global instance of the game
extern Game game;

#endif // TYPES_H