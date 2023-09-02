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
    Texture2D bgTileTex;
} SG_Game;

// functions for managing game state
SG_Game*    SG_NewGame();
void        SG_Draw(SG_Game* g);
void        SG_Quit(SG_Game* g);

// specific draw functions
void        SG_DrawBackgroundTiles(SG_Game* g);
void        SG_DrawPlayer(SG_Game* g);

// other
void        SG_HandleInput(SG_Game* g);

#endif // SG_GAME_H
