#ifndef OBJECT_H
#define OBJECT_H

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "glib.h"
#include "raylib.h"
#include "raymath.h"

// object - any game element
// game has limited # of object slots
// each object has 1 method: update (called once per frame)
// optional: render function to draw it

typedef enum {
    OBJ_ENTITY,     // any in-game entity like player, bullet, enemy
    OBJ_UI,         // any ui element - buttons, text
    OBJ_TIMER       // timer object
} ObjectType;

typedef void (*ObjCallback)();

typedef struct {
    ObjectType type;
    ObjCallback update;
    ObjCallback render;
} Object;



#endif // OBJECT_H