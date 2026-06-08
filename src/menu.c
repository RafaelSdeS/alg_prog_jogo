#include <raylib.h>
#include "menu.h"
#include "game.h"
#include "save.h"
#include <stdio.h>

#include "game_state.h"

int selectedOption = 0;

// Lógica de seleção do menu - Menu "rotaciona" entre as opções
void UpdateMenu(Game *game, GameState *currentScreen) {

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
            InitGame(game, currentScreen);
            *currentScreen = GAME;
        }

        if (selectedOption == 1) {
            printf("Entrando em Selecionar Save");
            RefreshSaveList();
            *currentScreen = SELECTSAVE;
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