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
    CreateObject(OC_STARTSCREEN_UI_TITLE_TEXT);
    CreateObject(OC_STARTSCREEN_UI_START_BUTTON);
    CreateObject(OC_STARTSCREEN_UI_SETTINGS_BUTTON);
    CreateObject(OC_STARTSCREEN_UI_QUIT_BUTTON);
}

void LoadSceneSettings() {

}

void LoadSceneGameplay() {

    // create objects for gameplay - player, timers, ui

    game.player = CreateObject(OC_GAMEPLAY_ENTITY_PLAYER);
    CreateObject(OC_GAMEPLAY_UI_ELEMENT_GAME_TIMER);
    CreateObject(OC_GAMEPLAY_UI_ELEMENT_HEALTHBAR);

    // timer: shoot basic bullets
    CreateObject(OC_GAMEPLAY_TIMER_PLAYER_SHOOT_BULLET_BASIC);

    // timer: spawn normal enemies
    CreateObject(OC_GAMEPLAY_TIMER_SPAWN_ENEMY_BASIC);

    // start the timer
    game.gameplay_time_elapsed = GetTime();
}

void LoadSceneEndScreen() {
    CreateObject(OC_ENDSCREEN_UI_RESTART_BUTTON);
    CreateObject(OC_ENDSCREEN_UI_ELEMENT_TIME_SURVIVED);
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
    game.camera.target = game.player->pos;
    HandleInput();

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
    game.gameplay_time_elapsed += (double) GetFrameTime();
   
}

void DrawSceneEndScreen() {
    BeginDrawing();

        ClearBackground(RAYWHITE);
        UpdateObjects();
        RenderObjects();

    EndDrawing();
}
