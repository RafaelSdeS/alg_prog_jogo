/*
 * Sistema de menu principal do jogo.
 *
 * Responsável por:
 * - Controle de navegação entre opções do menu (input UP/DOWN)
 * - Gerenciamento de estado de seleção (selectedOption)
 * - Transição entre telas do jogo (GAME, SELECTSAVE, RANKING, saída)
 * - Inicialização de novo jogo via InitGame
 * - Integração com sistema de saves e ranking
 * - Renderização da interface do menu principal
 *
 * Este módulo funciona como camada de entrada da aplicação,
 * direcionando o fluxo para os demais subsistemas do jogo.
 */

#include <raylib.h>
#include <stdio.h>

#include "menu.h"
#include "game.h"
#include "save.h"
#include "game_state.h"

int selectedOption = 0;

// Lógica de seleção do menu
void UpdateMenu(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]) {

    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption++;
    }

    if (IsKeyPressed(KEY_UP)) {
        selectedOption--;
    }

    // Menu rotaciona entre as opções
    if (selectedOption < 0) {
        selectedOption = 3;
    }

    if (selectedOption > 3) {
        selectedOption = 0;
    }

    // Seleções do menu
    if (IsKeyPressed(KEY_ENTER)) {

        if (selectedOption == 0) {
            InitGame(game, currentScreen, powerUps);
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