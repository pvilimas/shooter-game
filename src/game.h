#ifndef GAME_H
#define GAME_H

#include "types.h"

// main functions
void        Config();
void        Init();
void        Draw();
void        Quit();

// game functions
void        TileBackground();
void        KillPlayer();

// scene.c
void        LoadScene(Scene new_scene);
void        DrawSceneStartScreen();
void        DrawSceneGameplay();
void        DrawSceneEndScreen();

// utils.c
Vector2     GetCameraOffset();
Vector2     GetAbsPosition(Vector2 p);
Vector2     GetAbsMousePosition();
bool        PointOnScreen(Vector2 p);
bool        PointNearScreen(Vector2 p, float range);
int         RandInt(int min, int max);
float       RandFloat(float min, float max);

void        UpdateObjects();
void        RenderObjects();
void        DeleteObjects();

Object*     CreateObject(ObjType type);
Object*     GetObject(ObjType type, int id);
void        DeleteObject(ObjType type, int id);

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

// glib or memory management

void        FreeTextureCallback(void* texture);
void        FreeFontCallback(void* font);

// timer callbacks

void        PlayerShootAtMouseCallback();
void        SpawnEnemyCallback();

// object callbacks

void        ObjUpdateTimerCallback(void* obj);

void        ObjUpdatePlayerCallback(void* obj);
void        ObjRenderPlayerCallback(void* obj);
void        ObjUpdateEnemyCallback(void* obj);
void        ObjRenderEnemyCallback(void* obj);
void        ObjUpdateBulletCallback(void* obj);
void        ObjRenderBulletCallback(void* obj);

void        ObjRenderTextCallback(void* obj);
void        ObjUpdateButtonCallback(void* obj);
void        ObjRenderButtonCallback(void* obj);
void        ObjRenderHealthbarCallback(void* obj);

// button/ui callbacks (on-click)

void        BtnStartCallback();
void        BtnRestartCallback();

#endif // GAME_H
