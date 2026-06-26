/*
 * Ponto de entrada do jogo (main loop).
 *
 * Responsável por:
 * - Inicialização do Raylib
 * - Criação e inicialização do estado global do jogo (Game)
 * - Gerenciamento do fluxo de estados da aplicação (GameState)
 * - Encaminhamento de input e atualização para módulos específicos (menu, game, save, ranking)
 * - Controle do loop principal de execução (update + render)
 * - Coordenação entre telas (MENU, GAME, GAMEOVER, WINSCREEN, etc.)
 * - Processamento de input textual para entrada de nome do jogador
 * - Inicialização e liberação de recursos globais (texturas, janela)
 *
 * Este arquivo atua como camada de orquestração de alto nível,
 * sem conter lógica de gameplay direta, apenas roteamento entre sistemas.
 */

#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "game_core.h"
#include "save.h"
#include "ranking.h"
#include "game_state.h"
#include "power_up.h"
#include "audio.h"
#include "ui.h"

int main() {

    // Incialização do srand para definir a chance de conseguir um powerup
    srand(time(NULL));

    // Inicializações do RayLib
    InitWindow(800, 600, "Brick Breaker");

    // Não fechar a janela com o ESC
    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    // Estado do jogo
    Game game;
    GameState currentScreen = MENU;

    // Inicilização da struct de powerups
    PowerUp powerUps[MAX_POWERUPS] = {0};

    // Carrega as texturas dos powerups
    LoadPowerUpTextures();

    // Inicializar o jogo no menu
    InitGame(&game, &currentScreen, powerUps);

    // Inicializar audio
    InitAudioSystem();

    // Loop pirncipal do jogo
    while (!WindowShouldClose()) {

        UpdateAudioSystem();

        // Lógica de seleção do menu
        if (currentScreen == MENU) {
            UpdateMenu(&game, &currentScreen, powerUps);
        }

        // Selecionar para começar o jogo
        else if (currentScreen == GAME) {
            UpdateGame(&game, &currentScreen, powerUps);

            // ESC volta para o menu
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = MENU;
            }
        }

        // Tela de gameover
        else if (currentScreen == GAMEOVER) {
            // Lógica para entrar na tela para colocar o nome no ranking
            if (IsKeyPressed(KEY_ENTER)) {
                if (IsTopScore(game.score)) {

                    game.playerName[0] = '\0';
                    currentScreen = ENTERNAME;

                } else {
                    currentScreen = MENU;
                }
            }
        }

        // Tela de vitória
        else if (currentScreen == WINSCREEN) {
            // Lógica para entrar na tela para colocar o nome no ranking
            if (IsKeyPressed(KEY_ENTER)) {
                if (IsTopScore(game.score)) {

                    game.playerName[0] = '\0';
                    currentScreen = ENTERNAME;

                } else {
                    currentScreen = MENU;
                }
            }
        }

        // Tela de seleção de saves
        else if (currentScreen == SELECTSAVE) {
            UpdateSaveSelection(&game, &currentScreen);
        }

        // Tela de ranking
        else if (currentScreen == RANKING) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = MENU;
            }
        }

        // Tela para digitar o nome
        else if (currentScreen == ENTERNAME) {

            // Lê caracteres digitados no frame atual (buffer de input do Raylib)
            int key = GetCharPressed();

            // Processa todos os caracteres que foram digitados neste frame
            while (key > 0) {

                // Aceita apenas caracteres ASCII imprimíveis e limita tamanho do nome
                if (key >= 32 && key <= 125 && strlen(game.playerName) < 49) {

                    int len = strlen(game.playerName);

                    // Adiciona o caractere no final da string
                    game.playerName[len] = (char)key;
                    game.playerName[len + 1] = '\0';
                }

                // Pega próximo caractere do buffer (caso exista mais de um)
                key = GetCharPressed();
            }

            // Remove último caractere (backspace)
            if (IsKeyPressed(KEY_BACKSPACE)) {

                int len = strlen(game.playerName);

                if (len > 0) {
                    game.playerName[len - 1] = '\0';
                }
            }

            // Confirma o nome e salva no ranking
            if (IsKeyPressed(KEY_ENTER) && strlen(game.playerName) > 0) {
                UpdateRanking(game.playerName, game.score);
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

        else if (currentScreen == GAME) {
            DrawGame(&game, powerUps);
        }

        else if (currentScreen == GAMEOVER) {
            DrawGameOverScreen(&game);
        }

        else if (currentScreen == WINSCREEN) {
            DrawWinScreen(&game);
        }

        else if (currentScreen == SELECTSAVE) {
            DrawSaveSelection();
        }

        else if (currentScreen == RANKING) {
            DrawRanking();
        }

        else if (currentScreen == ENTERNAME) {
            DrawEnterNameScreen(&game);
        }

        EndDrawing();
    }

    // Descarrega as texturas
    UnloadPowerUpTextures();
    UnloadAudioSystem();
    CloseWindow();

    return 0;
}