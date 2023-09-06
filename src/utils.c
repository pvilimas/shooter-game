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

// absolute mouse position, accounting for camera offset
Vector2 GetAbsMousePosition() {
    return Vector2Add(GetMousePosition(), GetCameraOffset());
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
