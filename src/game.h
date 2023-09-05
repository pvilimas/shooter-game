#ifndef GAME_H
#define GAME_H

#include "types.h"

// main functions
void        Config();
void        Init();
void        Draw();
void        Quit();

// game functions
void        HandleInput();

void        TileBackground();
void        DrawUI();

void        RenderPlayer();
void        RenderEnemies();
void        RenderBullets();

void        SpawnEnemy(Vector2 pos, float angle, int speed);
void        SpawnBullet(Vector2 pos, float angle, int speed);

// scene.c
void        LoadScene(Scene new_scene);
void        DrawSceneStartScreen();
void        DrawSceneGameplay();
void        DrawSceneEndScreen();

// utils.c
Vector2     GetCameraOffset();
Vector2     GetAbsMousePosition();
bool        PointOnScreen(Vector2 p);
bool        PointNearScreen(Vector2 p, float range);
int         RandInt(int min, int max);
float       RandFloat(float min, float max);
// TODO
void        CheckCollisions();

void        CreateTimer(TimerCallback fn, double interval, int num_triggers);
bool        UpdateTimer(Timer* t);
void        UpdateTimers();

// assets.c
void        UnloadAssets();
void        CreateTexture(char* name, const char* filepath);
Texture2D*  GetTexture(char* name);
void        DeleteTexture(char* name);
void        CreateFont(char* name, const char* filepath);
Font*       GetFont(char* name);
void        DeleteFont(char* name);

// callbacks.c
void        TestCallback();
void        FreeTextureCallback(void* texture);
void        FreeFontCallback(void* font);
void        DefaultTimerCallback();
void        PlayerShootAtMouseCallback();
void        SpawnEnemyCallback();

#endif // GAME_H
