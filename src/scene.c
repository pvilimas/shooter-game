#include "game.h"

/*
    scene.c - scene management functions
*/

void DrawSceneStartScreen() {

}

void DrawSceneGameplay() {
    UpdateTimers();

    // update camera target to player
    game.camera.target = game.player.pos;

    BeginDrawing();
        BeginMode2D(game.camera);
            
            /* all in-game objects get drawn here */
            
            TileBackground();
            RenderPlayer();
            RenderEnemies();
            RenderBullets();

        EndMode2D();

        /* static ui elements get drawn here */

        DrawUI();
    EndDrawing();
    game.frame_count++;
}

void DrawSceneEndScreen() {

}

// contains initialization logic for each scene
void LoadScene(Scene new_scene) {
    if (game.current_scene == SCENE_STARTSCREEN && new_scene == SCENE_GAMEPLAY) {

    } else if (game.current_scene == SCENE_GAMEPLAY && new_scene == SCENE_ENDSCREEN) {

    } else if (game.current_scene == SCENE_ENDSCREEN && new_scene == SCENE_GAMEPLAY) {

    } else {
        printf("Invalid scene transition, exiting\n");
        Quit();
    }
    game.current_scene = new_scene;
}