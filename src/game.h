#ifndef SG_GAME_H
#define SG_GAME_H

#include "common.h"

#include "player.h"

// the game class
typedef struct {
    Vector2 screenSize;
    SG_Player player;
    Camera2D camera;
} SG_Game;

SG_Game*    SG_NewGame();
void        SG_Draw(SG_Game* g);
void        SG_Quit(SG_Game* g);

#endif // SG_GAME_H