#ifndef GAME_H
#define GAME_H

#include "common.h"

// the types of entities
// TODO unify these into a single type
typedef struct {
    Vector2 pos;
    int health;
} Player;

typedef struct {
    Vector2 pos;
    Vector2 speed;
} Enemy;

typedef struct {
    Vector2 pos;
    Vector2 speed;
} Bullet;

// timer class
typedef void (*TimerCallback)();

typedef struct {
    double interval;
    int num_triggers; // -1 = inf
    TimerCallback fn;

    double last_recorded;
} Timer;

// the game class
typedef struct {
    Vector2     screen_size;
    Camera2D    camera;
    Player      player;

    GPtrArray*  timers;     // <Timer*>

    GHashTable* textures;   // <cchar*, Texture2D*>
    GHashTable* fonts;      // <cchar*, Font*>
} Game;

// global instance of the game
static Game game;

// main functions
void        Config();
void        Init();
void        Draw();
void        Quit();

// game functions
void        HandleInput();

void        TileBackground();
void        DrawPlayer();

void        DrawUI();
void        DrawHealthBar();

void        CreateTimer(double interval, int num_triggers, TimerCallback fn);
void        CheckTimers();
bool        CheckTimer(Timer* t);

// asset handling
void        UnloadAssets();

void        CreateTexture(char* name, const char* filepath);
Texture2D*  GetTexture(char* name);
void        DeleteTexture(char* name);

void        CreateFont(char* name, const char* filepath);
Font*       GetFont(char* name);
void        DeleteFont(char* name);

// callbacks
void        FreeTextureCallback(void* texture);
void        FreeFontCallback(void* font);
void        DefaultTimerCallback();

#endif // GAME_H
