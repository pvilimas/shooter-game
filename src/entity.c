#include "game.h"

entity_defaults = {
    [ENT_PLAYER] = {
        .speed = 3,
        .max_health = 100,
        .hitbox_radius = 5,
    },
    [ENT_ENEMY] = {
        .speed = 1,
        .max_health = 20,
        .hitbox_radius = 25
    },
    [ENT_BULLET] = {
        .speed = 10,
        .lifetime = 30,
        .damage = 20,
        .hitbox_radius = 10
    }
};