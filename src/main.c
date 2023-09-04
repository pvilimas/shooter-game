#include "common.h"
#include "game.h"

int main() {
    Config();
    Init();
    while (!WindowShouldClose()) {
        Draw();
    }
    Quit();
    return 0;
}