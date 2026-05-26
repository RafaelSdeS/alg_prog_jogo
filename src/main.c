#include <raylib.h>
#include "menu.h"
#include "game.h"

#define MENU 0
#define GAME 1
#define GAMEOVER 2
#define WINSCREEN 3

int main() {

    // Inicializações do RayLib
    InitWindow(800, 600, "Brick Breaker");

    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    int currentScreen = MENU;

    while (!WindowShouldClose()) {
        
        // Lógica de seleção do menu
        if (currentScreen == MENU) {
            UpdateMenu(&currentScreen);
        }

        // Selecionar para começar o jogo
        if (currentScreen == GAME) {
            UpdateGame(&currentScreen);;

            // ESC volta para o menu
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = MENU;
            }
        }

        // Tela de gameover
        if (currentScreen == GAMEOVER) {

            // Enter volta ao menu
            if (IsKeyPressed(KEY_ENTER)) {

                InitGame(&currentScreen);

                currentScreen = MENU;
            }
        }

        // Tela de vitória
        if (currentScreen == WINSCREEN) {

            if (IsKeyPressed(KEY_ENTER)) {

                currentScreen = MENU;
            }
        }

        // Funções do RayLib
        BeginDrawing();

        ClearBackground(BLACK);

        // Desenhar as diferentes telas do jogo
        if (currentScreen == MENU) {
            DrawMenu();
        }

        if (currentScreen == GAME) {
            DrawGame();
        }

        if (currentScreen == GAMEOVER) {

            DrawText(
                "GAME OVER",
                220,
                200,
                60,
                RED
            );

            DrawText(
                TextFormat("FINAL SCORE: %d", score),
                220,
                300,
                30,
                WHITE
            );

            DrawText(
                "Press ENTER to return",
                220,
                400,
                30,
                GRAY
            );
        }

        if (currentScreen == WINSCREEN){
                DrawText(
                "YOU WON!",
                220,
                200,
                60,
                YELLOW
            );

            DrawText(
                TextFormat("FINAL SCORE: %d", score),
                220,
                300,
                30,
                WHITE
            );

            DrawText(
                "Press ENTER to return",
                220,
                400,
                30,
                GRAY
            );
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}