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
    SG_DrawPlayer(g);
}

void SG_Quit(SG_Game* g){
    // raylib cleanup functions run here
}

void SG_DrawPlayer(SG_Game* g) {
    DrawCircle(g->player.pos.x, g->player.pos.y, 5.0f, BLACK);
}

void SG_HandleInput(SG_Game* g) {
    if (IsKeyDown(KEY_W)) {
        g->player.pos.y -= 1;
    }
    if (IsKeyDown(KEY_A)) {
        g->player.pos.x -= 1;
    }
    if (IsKeyDown(KEY_S)) {
        g->player.pos.y += 1;
    }
    if (IsKeyDown(KEY_D)) {
        g->player.pos.x += 1;
    }
}