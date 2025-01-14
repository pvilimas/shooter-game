#include "game.h"

/*
    utils.c - math and utils functions
*/

// offset of the camera from the origin
Vector2 GetCameraOffset() {
    return (Vector2){
        game.camera.target.x - GetScreenWidth() / 2,
        game.camera.target.y - GetScreenHeight() / 2
    };
}

// absolute position on screen - used for drawing static ui elements
Vector2 GetAbsPosition(Vector2 p) {
    return Vector2Add(p, GetCameraOffset());
}

// absolute mouse position, accounting for camera offset
Vector2 GetAbsMousePosition() {
    return GetAbsPosition(GetMousePosition());
}

// returns true if p is onscreen
bool PointOnScreen(Vector2 p) {
    return PointNearScreen(p, 0.0f);
}

// returns true if p is onscreen or within range pixels of screen edge
// negative range works as expected
bool PointNearScreen(Vector2 p, float range) {
    return p.x >= game.camera.target.x + range - GetScreenWidth() / 2
        && p.x <= game.camera.target.x - range + GetScreenWidth() / 2
        && p.y >= game.camera.target.y + range - GetScreenHeight() / 2
        && p.y <= game.camera.target.y - range + GetScreenHeight() / 2;
}

int RandInt(int min, int max) {
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

float RandFloat(float min, float max) {
    return min + (float)rand() / ((float)(RAND_MAX / (max-min)));
}

// if (PercentChance(33.3)) {...}
bool PercentChance(float percent) {
    if (percent < 0.0f || percent > 100.0f) {
        return false;
    }

    return (float)rand() / (float)RAND_MAX * 100.0f <= percent;
}

// returns the game time as a string
const char* GameTimeStr(double seconds) {
    // int h = (int)seconds / 3600;
    // int m = ((int)seconds % 3600) / 60;
    // int s = (int)seconds % 60;

    // if (h > 0) {
    //     sprintf(game.buffer, "%d:%d:%02d", h, m, s);
    // } else {
    //     sprintf(game.buffer, "%d:%d", m, s);
    // }

    sprintf(game.buffer, "%.1lf", seconds);

    return (const char*) game.buffer;
}
