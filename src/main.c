#include "game.h"

// global instance
Game game;

int main() {
    Config();
    Init();
    while (!WindowShouldClose()) {
        Draw();
    }
    Quit();
    return 0;
}