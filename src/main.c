#include <raylib.h>
#include "menu.h"
#include "game.h"
#include "save.h"
#include <stdio.h>

#include "game_state.h"

int main() {

    // Inicializações do RayLib
    InitWindow(800, 600, "Brick Breaker");

    // Não fechar a janela com o ESC
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    // Estado do jogo
    Game game;
    GameState currentScreen = MENU;

    // Inicializar o jogo no menu
    InitGame(&game, &currentScreen);

    while (!WindowShouldClose()) {

        // Lógica de seleção do menu
        if (currentScreen == MENU) {
            UpdateMenu(&game, &currentScreen);
        }

        // Selecionar para começar o jogo
        else if (currentScreen == GAME) {
            UpdateGame(&game, &currentScreen);

            // ESC volta para o menu
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = MENU;
            }
        }

        // Tela de gameover
        else if (currentScreen == GAMEOVER) {

            // Enter volta ao menu
            if (IsKeyPressed(KEY_ENTER)) {
                currentScreen = MENU;
            }
        }

        // Tela de vitória
        else if (currentScreen == WINSCREEN) {
            if (IsKeyPressed(KEY_ENTER)) {
                currentScreen = MENU;
            }
        }

        else if (currentScreen == SELECTSAVE) {
            UpdateSaveSelection(&game, &currentScreen);
        }

        // Funções do RayLib
        BeginDrawing();

        ClearBackground(BLACK);

        // Desenhar as diferentes telas do jogo
        if (currentScreen == MENU) {
            DrawMenu();
        }

        if (currentScreen == GAME) {
            DrawGame(&game);
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
                TextFormat("SCORE FINAL: %d", game.score),
                220,
                300,
                30,
                WHITE
            );

            DrawText(
                "Pressione ENTER",
                220,
                400,
                30,
                GRAY
            );
        }

        if (currentScreen == WINSCREEN) {

            DrawText(
                "YOU WON!",
                220,
                200,
                60,
                YELLOW
            );

            DrawText(
                TextFormat("SCORE FINAL: %d", game.score),
                220,
                300,
                30,
                WHITE
            );

            DrawText(
                "Pressione ENTER",
                220,
                400,
                30,
                GRAY
            );
        }

        if (currentScreen == SELECTSAVE) {
            DrawSaveSelection();
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}