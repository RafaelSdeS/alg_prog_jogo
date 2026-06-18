#include <raylib.h>
#include "..\include\menu.h"
#include "..\include\game.h"
#include "..\include\save.h"
#include <stdio.h>

#include "..\include\game_state.h"

int selectedOption = 0;

// Lógica de seleção do menu - Menu "rotaciona" entre as opções
void UpdateMenu(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]) {

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
            *currentScreen = GAME;
        }

        if (selectedOption == 1) {
            printf("Entrando em Selecionar Save");
            RefreshSaveList();
            *currentScreen = SELECTSAVE;
        }

        if (selectedOption == 2) {
            *currentScreen = RANKING;
        }

        if (selectedOption == 3) {
            CloseWindow();
        }
    }
}

// Desenhar o menu na tela
void DrawMenu() {

    char options[][20] = {
        "Novo Jogo",
        "Carregar Save",
        "Ranking",
        "Sair"
    };

    DrawText("MENU", 300, 100, 40, WHITE);

    // Percorrer as opções do menu de forma que não ocupe muitas linhas de código
    for (int i = 0; i < 4; i++) {
        DrawText(options[i], 300, 200 + i * 100, 40, i == selectedOption ? YELLOW : WHITE);
    }
}