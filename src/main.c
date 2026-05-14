#include <raylib.h>
#include "menu.h"

int main() {

    InitWindow(800, 600, "Teste");

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(BLACK);

        DrawMenu();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}