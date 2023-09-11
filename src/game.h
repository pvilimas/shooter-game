#ifndef GAME_H
#define GAME_H

#include "types.h"
#include "objdata.h"

/*
    TODO
    - start screen
        - add title
        - add quit button
        - add settings button/menu (leave it empty for now)
    - gameplay
        - redesign healthbar using colors 1-5
        - add timer
        - pause menu
    - end screen
        - time survived

    - find fonts
    - start adding different types of enemies and bullets, maybe 5 of each
        - make sure collision detection isn't totally slow/broken
    - make some kinda upgrade tree
    - add some sound effects
    - add a dev mode CLI flag
        - prints debug info and logs stuff
        - makes hitboxes visible
    - make some pixel art instead of drawing primitives
        - animate them
    - resolution menu like terraria has
*/

// game.c
void        Config();
void        Init();
void        Draw();
void        Quit();

void        TileBackground();
void        KillPlayer();

// scene.c
void        LoadScene(Scene new_scene);

void        LoadSceneStartScreen();
void        LoadSceneSettings();
void        LoadSceneGameplay();
void        LoadSceneEndScreen();

void        DrawSceneStartScreen();
void        DrawSceneSettings();
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

// object.c
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

// glib/memory

void        FreeTextureCallback(void* texture);
void        FreeFontCallback(void* font);

// timer

void        PlayerShootAtMouseCallback();
void        SpawnEnemyCallback();

// object

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

// button/ui

void        BtnStartCallback();
void        BtnSettingsCallback();
void        BtnRestartCallback();

#endif // GAME_H
