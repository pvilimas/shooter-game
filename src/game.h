#ifndef GAME_H
#define GAME_H

#include "common.h"

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

// the game class
typedef struct {
    Vector2     screen_size;
    Camera2D    camera;
    Player      player;

    GPtrArray*  enemies;        // <Enemy*>
    GPtrArray*  bullets;        // <Bullet*>
    GPtrArray*  timers;         // <Timer*>

    GHashTable* textures;       // <cchar*, Texture2D*>
    GHashTable* fonts;          // <cchar*, Font*>

    Scene       current_scene;
    int         frame_count;    // increment every frame
} Game;

// global instance of the game
static Game game;

// main functions
void        Config();
void        Init();
void        Draw();
void        Quit();

// math/utils

Vector2     GetCameraOffset();
Vector2     GetAbsMousePosition();
bool        PointOnScreen(Vector2 p);
bool        PointNearScreen(Vector2 p, float range);
int         RandInt(int min, int max);
float       RandFloat(float min, float max);

// game functions
void        LoadScene(Scene new_scene);

void        HandleInput();

void        TileBackground();
void        DrawUI();

void        RenderPlayer();

void        RenderEnemies();
void        RenderBullets();

void        SpawnEnemy(Vector2 pos, float angle, int speed);
void        SpawnBullet(Vector2 pos, float angle, int speed);

void        CreateTimer(TimerCallback fn, double interval, int num_triggers);
bool        UpdateTimer(Timer* t);
void        UpdateTimers();

// TODO
void        CheckCollisions();

// asset handling
void        UnloadAssets();

void        CreateTexture(char* name, const char* filepath);
Texture2D*  GetTexture(char* name);
void        DeleteTexture(char* name);

void        CreateFont(char* name, const char* filepath);
Font*       GetFont(char* name);
void        DeleteFont(char* name);

// callbacks
void        TestCallback();
void        FreeTextureCallback(void* texture);
void        FreeFontCallback(void* font);
void        DefaultTimerCallback();
void        PlayerShootAtMouseCallback();
void        SpawnEnemyCallback();

#endif // GAME_H
