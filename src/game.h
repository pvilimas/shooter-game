#ifndef GAME_H
#define GAME_H

#include "types.h"

/*
    TODO
    - completely rewrite objects
    - make everything an object, even scenes
        - add more functions to object like
            - should_update() -> bool
            - should_render() -> bool
    - primary and secondary objects
        - only primary ones get updated, and they trigger updates in
            the objects they spawn/manage

    - fix pause menu shader

    - pause menu
        - handle inputs on pause screen
    - start handling resolutions
    - fix in-game timer
    - proper hitboxes and collision shapes
        - shape type
    - improve keybinds - split into ui and game keybinds
        - game keybinds - queue, so if W is held then S is pressed,
            player keeps moving up
        - ui - can't really be remapped, maybe use IsKeyPressed not
            Down

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
    - improve background tiling - allow for different tiles
    
    ideas:
    - camera damping/stays in front of player

    list of all possible inputs:
    - gameplay
         - player move
         - player shoot
         - player use abilities
         - anything else the player object can do in the game
    - menus
        - up/down/left/right in button menus
        - mouse clicking on certain buttons (handled in their callbacks)
    - gameplay pause state is handled separately
*/

// game.c
void        Config();
void        Init();
void        Draw();
void        Quit();

void        Pause();
void        Unpause();

void        TileBackground();
void        KillPlayer();

bool        WindowMovedToNewMonitor();
void        ResizeDisplayToMonitor();

void        DrawRenderTexture();

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

void        CreateShader(const char* name, const char* vert_shader_path,
                const char* frag_shader_path);
Shader*     GetShader(const char* name);
void        DeleteShader(const char* name);

// keyinput.c

void        HandleInput();

void        CreateKeybind(const char* name, int key_code,
                KeybindCallback callback, int delay_ms);
void        RemapKeybind(const char* name, int new_key_code);
void        DeleteKeybind(const char* name);

// callbacks.c
void        TestCallback();

void        MemFreeTextureCallback(void* texture);
void        MemFreeFontCallback(void* font);
void        MemFreeShaderCallback(void* shader);

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
void        KeybindTogglePauseCallback();

#endif // GAME_H
