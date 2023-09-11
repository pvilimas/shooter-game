#include "game.h"

// scene.c - scene management functions

// loads in the objects for the new scene
void LoadScene(Scene new_scene) {

    // unload previous scene
    DeleteObjects();

    // initial startup
    if (game.current_scene == SCENE_STARTUP
        && new_scene == SCENE_STARTSCREEN)
        LoadSceneStartScreen();

    // going into settings
    else if (game.current_scene == SCENE_STARTSCREEN
        && new_scene == SCENE_SETTINGS)
        LoadSceneSettings();
    
    // going back into start screen from settings
    else if (game.current_scene == SCENE_SETTINGS
        && new_scene == SCENE_STARTSCREEN)
        LoadSceneStartScreen();

    // start playing the game
    else if (game.current_scene == SCENE_STARTSCREEN
        && new_scene == SCENE_GAMEPLAY)
        LoadSceneGameplay();

    // player hit "give up" or died
    else if (game.current_scene == SCENE_GAMEPLAY
        && new_scene == SCENE_ENDSCREEN)
        LoadSceneEndScreen();

    // player hit "restart"
    else if (game.current_scene == SCENE_ENDSCREEN
        && new_scene == SCENE_GAMEPLAY)
        LoadSceneGameplay();

    // player used arbitrary code execution
    else {
        printf("Invalid scene transition, exiting\n");
        Quit();
    }
    
    game.current_scene = new_scene;
}

void LoadSceneStartScreen() {
    CREATE_OBJ_STARTSCREEN_TITLE_TEXT();
    CREATE_OBJ_STARTSCREEN_START_BUTTON();
    CREATE_OBJ_STARTSCREEN_SETTINGS_BUTTON();
}

void LoadSceneSettings() {

}

void LoadSceneGameplay() {

    // create objects for gameplay - player, timers, ui

    CREATE_OBJ_GAMEPLAY_PLAYER();
    CREATE_OBJ_GAMEPLAY_UI_HEALTHBAR();

    // timer: shoot basic bullets
    CREATE_OBJ_GAMEPLAY_TIMER_PLAYER_SHOOT_BASIC();

    // timer: spawn normal enemies
    CREATE_OBJ_GAMEPLAY_TIMER_SPAWN_BASIC();
}

void LoadSceneEndScreen() {
    CREATE_OBJ_ENDSCREEN_RESTART_BUTTON();
}

void DrawSceneStartScreen() {
    BeginDrawing();

        ClearBackground(RAYWHITE);
        UpdateObjects();
        RenderObjects();

    EndDrawing();
}

void DrawSceneSettings() {

}

void DrawSceneGameplay() {
    // update camera target to player
    game.camera.target = game.player->data.ent_data.pos;

    BeginDrawing();
        BeginMode2D(game.camera);

            /* all in-game objects get drawn here */
            
            TileBackground();
            UpdateObjects();
            RenderObjects();

        EndMode2D();

        /* static ui elements get drawn here */

    EndDrawing();
    game.frame_count++;
}

void DrawSceneEndScreen() {
    BeginDrawing();

        ClearBackground(RAYWHITE);
        UpdateObjects();
        RenderObjects();

    EndDrawing();
}
