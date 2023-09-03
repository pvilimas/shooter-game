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
    g->bgTexture = LoadTexture("./assets/bg.png");
}

void SG_Draw(SG_Game* g){
    ClearBackground(RAYWHITE);
    // DrawTexture(g->bgTexture, 0, 0, WHITE);
    SG_DrawPlayer(g);
}

void SG_Quit(SG_Game* g){
    // UnloadTexture(g->bgTexture);
    // raylib cleanup functions run here
}

void SG_DrawBackgroundTiles(SG_Game* g) {
    ClearBackground(RAYWHITE);
}

void SG_DrawPlayer(SG_Game* g) {
    DrawCircle(g->player.pos.x, g->player.pos.y, 5.0f, BLACK);
}

// Handle all key input and move the player
void SG_HandleInput(SG_Game* g) {
    // TODO add sequential key presses, so like if W is pressed 
    // and then S, character continues moving up
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
