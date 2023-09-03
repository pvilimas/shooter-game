#ifndef SG_GAME_H
#define SG_GAME_H

#include "common.h"

typedef struct {
    Vector2 pos;
} SG_Player;

// the game class
typedef struct {
    Vector2 screenSize;
    SG_Player player;
    Camera2D camera;

    GHashTable* textures;   // <cchar*, Texture2D*>
    GHashTable* fonts;      // <cchar*, Font*>
} SG_Game;

// global instance of the game
static SG_Game game;

// managing game state
void        SG_Init();
void        SG_Draw();
void        SG_Quit();

// game loop functions
void        SG_DrawPlayer();
void        SG_TileBackground();
void        SG_HandleInput();

// asset handling
void        SG_LoadTexture(char* name, const char* filepath);
Texture2D*  SG_GetTexture(char* name);
void        SG_UnloadTexture(char* name);

void        SG_LoadFont(char* name, const char* filepath);
Font*       SG_GetFont(char* name);
void        SG_UnloadFont(char* name);

void        SG_UnloadAssets();

// callbacks

void        SG_FreeTextureCallback(void* texture);
void        SG_FreeFontCallback(void* font);

#endif // SG_GAME_H
