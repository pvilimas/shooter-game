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

// managing game state
SG_Game*    SG_NewGame();
void        SG_Draw(SG_Game* g);
void        SG_Quit(SG_Game* g);

// game loop functions
void        SG_DrawPlayer(SG_Game* g);
void        SG_TileBackground(SG_Game* g);
void        SG_HandleInput(SG_Game* g);

// asset handling
void        SG_LoadTexture(SG_Game* g, char* name, const char* filepath);
Texture2D*  SG_GetTexture(SG_Game* g, char* name);
void        SG_UnloadTexture(SG_Game* g, char* name);

void        SG_LoadFont(SG_Game* g, char* name, const char* filepath);
Font*       SG_GetFont(SG_Game* g, char* name);
void        SG_UnloadFont(SG_Game* g, char* name);

void        SG_UnloadAssets(SG_Game* g);

// callbacks

void        SG_FreeTextureCallback(void* texture);
void        SG_FreeFontCallback(void* font);

#endif // SG_GAME_H
