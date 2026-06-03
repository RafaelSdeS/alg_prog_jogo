#include "level.h"
#include <stdio.h>
#include <raylib.h>

// Lógica para preencher a matriz que irá gerar o nível
void LoadLevel(char level[ROWS][COLS], char *filename) {
    int row = 0, col = 0;
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

            // Tijolo tipo 1
            if (brick == '1') {
                DrawRectangle(
                    x,
                    y,
                    brickWidth,
                    brickHeight,
                    GREEN
                );
            }

            // Tijolo tipo 2
            if (brick == '2') {
                DrawRectangle(
                    x,
                    y,
                    brickWidth,
                    brickHeight,
                    ORANGE
                );
            }

            // Tijolo tipo 3
            if (brick == '3') {
                DrawRectangle(
                    x,
                    y,
                    brickWidth,
                    brickHeight,
                    RED 
                );
            }

            // Tijolo indestrutível
            if (brick == 'X') {
                DrawRectangle(
                    x,
                    y,
                    brickWidth,
                    brickHeight,
                    BLUE
                );
            }
        }
    }
}