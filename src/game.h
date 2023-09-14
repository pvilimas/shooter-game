#ifndef GAME_H
#define GAME_H

#include "types.h"

/*
    TODO
    - start handling resolutions
    - pause menu
            - make game draw to rendertexture the size of th screen
    - add keybind objects with callbacks
    - debug mode
    - fix in-game timer
    - proper hitboxes and collision shapes
        - shape type

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
    
    ideas:
    - camera damping/stays in front of player
*/

// game.c
void        Config();
void        Init();
void        Draw();
void        Quit();

// TODO
void        Pause();
void        Unpause();

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
bool        PercentChance(float percent);
const char* GameTimeStr(double seconds);

// object.c
void        UpdateObjects();
void        RenderObjects();
void        DeleteObjects();

Object*     CreateObject(ObjClass class);
Object*     GetObject(ObjType type, int id);
void        DeleteObject(ObjType type, int id);

ObjType     GetObjTypeOfClass(ObjClass class);

// assets.c
void        UnloadAssets();

void        CreateTexture(const char* name, const char* filepath);
Texture2D*  GetTexture(const char* name);
void        DeleteTexture(const char* name);

void        CreateFont(const char* name, const char* filepath);
Font*       GetFont(const char* name);
void        DeleteFont(const char* name);

// keyinput.c

void        HandleInput();

void        CreateKeybind(const char* name, int key_code, KeybindCallback);
void        RemapKeybind(const char* name, int new_key_code);
void        DeleteKeybind(const char* name);

// callbacks.c
void        TestCallback();

void        MemFreeTextureCallback(void* texture);
void        MemFreeFontCallback(void* font);

void        TimerPlayerShootBasicBulletCallback();
void        TimerSpawnBasicEnemyCallback();

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
void        ObjRenderGameTimerCallback(void* obj);
void        ObjRenderTimeSurvivedCallback(void* obj);

void        BtnStartCallback();
void        BtnSettingsCallback();
void        BtnRestartCallback();
void        BtnQuitCallback();

void        KeybindMoveUpCallback();
void        KeybindMoveDownCallback();
void        KeybindMoveLeftCallback();
void        KeybindMoveRightCallback();

#endif // GAME_H
