#include "game.h"

SG_Game* SG_NewGame(){
    SG_Game* g = malloc(sizeof(SG_Game));
    if (!g) {
        return NULL;
    }
    g->screenSize = (Vector2){
        SG_DEFAULT_SCREEN_W,
        SG_DEFAULT_SCREEN_H
    };
    g->player = (SG_Player){ .pos = {0, 0}};
    g->camera = (Camera2D){0};

}

void SG_Draw(SG_Game* g){
    ClearBackground(WHITE);
    DrawText("cocks", GetMouseX(), GetMouseY(), 20, BLACK);
}

void SG_Quit(SG_Game* g){
    // raylib cleanup functions run here
}
