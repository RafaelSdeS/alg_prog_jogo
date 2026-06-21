/*
 * Sistema de carregamento, representação e renderização de níveis.
 *
 * Responsável por:
 * - Leitura de arquivos de fase e preenchimento da matriz de level (LoadLevel)
 * - Conversão de símbolos de bloco em cores para renderização (GetBrickColor)
 * - Desenho da grade de blocos na tela (DrawLevel)
 * - Controle de progressão de fases baseado no estado do jogo (LoadCurrentLevel)
 *
 * Este módulo abstrai completamente a representação do nível como uma matriz de chars,
 * permitindo que outros sistemas (colisão, score, powerups) operem sobre o level.
 */

#include <raylib.h>
#include <stdio.h>

#include "level.h"
#include "game.h"
#include "brick.h"
#include "audio.h"

// Lógica para preencher a matriz que irá gerar o nível
void LoadLevel(char level[ROWS][COLS], char *filename) {

    // Colunas e linhas da matriz de tijolos para criar o nível
    int row = 0;
    int col = 0;

    // Variável char para ler o tipo dos tijolos
    char c;
    FILE *file = fopen(filename, "r"); // Acessar o arquivo de texto

    if (file == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    // Preencher a matriz do nível com os tijolos
    while ((c = fgetc(file)) != EOF && row < ROWS) {

        // Lógica para quebra de linha
        if (c == '\n') {
            row++;
            col = 0;
            continue;
        }

        if (col < COLS) {
            level[row][col] = c;
            col++;
        }
    }

    fclose(file); // Fechar arquivo
}

// Retorna a cor do tijolo com base no tipo
Color GetBrickColor(char brick) {

    if (brick == '1') return GREEN;
    if (brick == '2') return ORANGE;
    if (brick == '3') return RED;
    if (brick == 'X') return BLUE;

    return BLANK;
}

// Configurações para desenhar os tijolos
void DrawLevel(char level[ROWS][COLS]) {

    // Tamanho dos tijolos desenhados
    int brickWidth = 30;
    int brickHeight = 20;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            char brick = level[row][col];

            // Desenhar os tijolos na tela de acordo com a sua posição na matriz
            int x = col * brickWidth;
            int y = row * brickHeight + 50;

            // Conseguir a cor do tijolo
            Color color = GetBrickColor(brick);

            if (color.a != 0) {
                DrawRectangle(x, y, brickWidth, brickHeight, color);
            }
        }
    }
}

// Carrega o nível atual baseado no progresso do jogo
void LoadCurrentLevel(Game *game, GameState *currentScreen) {

    if (game->currentLevel == 1) {
        LoadLevel(game->level, "fases/fase1.txt");
        PrepareLevelPowerUps(game);
    }

    else if (game->currentLevel == 2) {
        LoadLevel(game->level, "fases/fase2.txt");
        PrepareLevelPowerUps(game);
    }

    else if (game->currentLevel == 3) {
        LoadLevel(game->level, "fases/fase3.txt");
        PrepareLevelPowerUps(game);
    }

    else {
        PlayVictorySound();
        *currentScreen = WINSCREEN;
    }
}