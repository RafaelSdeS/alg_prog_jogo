#include <raylib.h>
#include "menu.h"
#include "game.h"

#define MENU 0
#define GAME 1

int selectedOption = 0;

// Lógica de seleção do menu
void UpdateMenu(int *currentScreen) {

    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption++;
    }

    if (IsKeyPressed(KEY_UP)) {
        selectedOption--;
    }

    if (selectedOption < 0) {
        selectedOption = 3;
    }

    if (selectedOption > 3) {
        selectedOption = 0;
    }

    if (IsKeyPressed(KEY_ENTER)) {

        if (selectedOption == 0) {
            InitGame(currentScreen);
            *currentScreen = GAME;
    }

        if (selectedOption == 3) {
            CloseWindow();
        }
    }
}

// Mostrar as opções do menu
void DrawMenu() {

    DrawText("MENU", 300, 100, 40, WHITE);

    DrawText(
        "Novo Jogo",
        300,
        200,
        40,
        selectedOption == 0 ? YELLOW : WHITE
    );

    DrawText(
        "Carregar Save",
        300,
        300,
        40,
        selectedOption == 1 ? YELLOW : WHITE
    );

    DrawText(
        "Ranking",
        300,
        400,
        40,
        selectedOption == 2 ? YELLOW : WHITE
    );

    DrawText(
        "Sair",
        300,
        500,
        40,
        selectedOption == 3 ? YELLOW : WHITE
    );
}