#include "game.h"

/*
    scene.c - scene management functions
*/

// loads in the objects for the new scene
void LoadScene(Scene new_scene) {

    // unload previous scene
    DeleteObjects();

    // initial startup
    if (game.current_scene == SCENE_STARTUP && new_scene == SCENE_STARTSCREEN) {
        LoadSceneStartScreen();

    // going into settings
    } else if (game.current_scene == SCENE_STARTSCREEN && new_scene == SCENE_SETTINGS) {
        LoadSceneSettings();
    
    // going back into start screen from settings
    } else if (game.current_scene == SCENE_SETTINGS && new_scene == SCENE_STARTSCREEN) {
        LoadSceneStartScreen();

    // start playing the game
    } else if (game.current_scene == SCENE_STARTSCREEN && new_scene == SCENE_GAMEPLAY) {
        LoadSceneGameplay();

    // player hit "give up" or died
    } else if (game.current_scene == SCENE_GAMEPLAY && new_scene == SCENE_ENDSCREEN) {
        LoadSceneEndScreen();

    // player hit "restart"
    } else if (game.current_scene == SCENE_ENDSCREEN && new_scene == SCENE_GAMEPLAY) {
        LoadSceneGameplay();

    // player used arbitrary code execution
    } else {
        printf("Invalid scene transition, exiting\n");
        Quit();
    }
    
    game.current_scene = new_scene;
}

void LoadSceneStartScreen() {
    game.frame_count = 0;
    game.screen_size = (Vector2) {
        GetScreenWidth(),
        GetScreenHeight()
    };

    // create camera
    game.camera = (Camera2D) {
        .target = { 0, 0 },
        .offset = { game.screen_size.x / 2, game.screen_size.y / 2 },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    // start game button
    Object* o = CreateObject(OBJ_UI_BUTTON);
    o->data.ui_data.pos = (Vector2){ (game.screen_size.x / 2) - 100, (game.screen_size.y / 2) + 200 };
    o->data.ui_data.size = (Vector2) { 200, 50 };
    o->data.ui_data.color1 = (Color) { 200, 200, 210, 255 };
    o->data.ui_data.color2 = (Color) { 150, 150, 160, 255 };
    o->data.ui_data.color3 = (Color) { 170, 170, 180, 255 };
    o->data.ui_data.color4 = (Color) { 120, 120, 150, 255 };
    o->data.ui_data.color5 = (Color) { 160, 160, 170, 255 };
    o->data.ui_data.callback = BtnStartCallback;
    o->data.ui_data.label = "Start";

    // settings button
    o = CreateObject(OBJ_UI_BUTTON);
    o->data.ui_data.pos = (Vector2){ (game.screen_size.x / 2) - 100, (game.screen_size.y / 2) - + 275 };
    o->data.ui_data.size = (Vector2) { 200, 50 };
    o->data.ui_data.color1 = (Color) { 200, 200, 210, 255 };
    o->data.ui_data.color2 = (Color) { 150, 150, 160, 255 };
    o->data.ui_data.color3 = (Color) { 170, 170, 180, 255 };
    o->data.ui_data.color4 = (Color) { 120, 120, 150, 255 };
    o->data.ui_data.color5 = (Color) { 160, 160, 170, 255 };
    o->data.ui_data.callback = BtnSettingsCallback;
    o->data.ui_data.label = "Settings";

}

void LoadSceneSettings() {

}

void LoadSceneGameplay() {
    // create objects for gameplay - player, timers, ui

    game.player = CreateObject(OBJ_ENTITY_PLAYER);

    // timer: shoot basic bullets
    Object* o = CreateObject(OBJ_TIMER);
    o->data.tm_data.callback = PlayerShootAtMouseCallback;
    o->data.tm_data.interval = 2.0;
    o->data.tm_data.num_triggers = -1;

    // timer: spawn normal enemies
    o = CreateObject(OBJ_TIMER);
    o->data.tm_data.callback = SpawnEnemyCallback;
    o->data.tm_data.interval = 0.1;
    o->data.tm_data.num_triggers = -1;

    // player healthbar
    CreateObject(OBJ_UI_HEALTHBAR);
}

void LoadSceneEndScreen() {
    // restart game button
    Object* o = CreateObject(OBJ_UI_BUTTON);
    o->data.ui_data.pos = (Vector2){ (game.screen_size.x / 2) - 100, (game.screen_size.y / 2) - 25 };
    o->data.ui_data.size = (Vector2) { 200, 50 };
    o->data.ui_data.color1 = (Color) { 200, 200, 200, 255 };
    o->data.ui_data.color2 = (Color) { 150, 150, 150, 255 };
    o->data.ui_data.color3 = (Color) { 170, 170, 170, 255 };
    o->data.ui_data.color4 = (Color) { 120, 120, 120, 255 };
    o->data.ui_data.color5 = (Color) { 160, 160, 160, 255 };
    o->data.ui_data.callback = BtnRestartCallback;
    o->data.ui_data.label = "Restart";
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
