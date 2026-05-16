#include <raylib.h>
#include "menu.h"
#include "game.h"

#define MENU 0
#define GAME 1

int main() {

    InitWindow(800, 600, "Brick Breaker");

    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    int currentScreen = MENU;

    while (!WindowShouldClose()) {
        
        // Lógica de seleção do menu
        if (currentScreen == MENU) {
            UpdateMenu(&currentScreen);
        }

        if (currentScreen == GAME) {
            UpdateGame();

            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = MENU;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        if (currentScreen == MENU) {
            DrawMenu();
        }

        if (currentScreen == GAME) {
            DrawGame();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}