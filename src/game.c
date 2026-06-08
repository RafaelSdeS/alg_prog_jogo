#include <raylib.h>
#include "game.h"
#include "level.h"
#include "save.h"
#include "game_state.h"

// Carregar o nível atual e passar para o próximo. No momento, não é dinâmico
void LoadCurrentLevel(Game *game, GameState *currentScreen) {

    if (game->currentLevel == 1) {
        LoadLevel(game->level, "fases/fase1.txt");
    }

    else if (game->currentLevel == 2) {
        LoadLevel(game->level, "fases/fase2.txt");
    }

    else if (game->currentLevel == 3) {
        LoadLevel(game->level, "fases/fase3.txt");
    }

    else {
        *currentScreen = WINSCREEN;
    }
}

// Detecta se o nível está completo (verifica se ainda há tijolos destrutíveis na matriz)
int LevelCompleted(Game *game) {

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            if (game->level[row][col] == '1' ||
                game->level[row][col] == '2' ||
                game->level[row][col] == '3'
            ) {
                return 0;
            }
        }
    }

    return 1;
}

// Inicializa o jogo
void InitGame(Game *game, GameState *currentScreen) {

    // Sistema do jogo
    game->lives = 3;
    game->score = 0;
    game->currentLevel = 1;

    // Bola
    game->ballRadius = 10;

    // Plataforma
    game->paddleWidth = 100;
    game->paddleHeight = 20;

    game->paddleSpeed = 7;

    // Reinicia bola e plataforma
    game->ballX = 400;
    game->ballY = 300;

    game->ballSpeedX = 5;
    game->ballSpeedY = -5;

    game->paddleX = 350;
    game->paddleY = 550;

    // Carregar nível
    LoadCurrentLevel(game, currentScreen);
}

// Atualiza o jogo
void UpdateGame(Game *game, GameState *currentScreen) {

    // Movimento da bola
    game->ballX += game->ballSpeedX;
    game->ballY += game->ballSpeedY;

    // Colisão com paredes
    if (game->ballX >= 800 - game->ballRadius ||
        game->ballX <= game->ballRadius) {

        game->ballSpeedX *= -1;
    }

    if (game->ballY <= game->ballRadius) {

        game->ballSpeedY *= -1;
    }

    // colisão com tijolos
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            char brick = game->level[row][col];

            if (brick == '1' || brick == '2' || brick == '3' || brick == 'X') {

                int brickX = col * 30;
                int brickY = row * 20 + 50;

                if (CheckCollisionCircleRec(
                        (Vector2){game->ballX, game->ballY},
                        game->ballRadius,
                        (Rectangle){ brickX, brickY, 30, 20 }
                    )
                ) {

                    if (brick == '1') {
                        game->level[row][col] = '0';
                        game->score += 100;
                    }

                    if (brick == '2') {
                        game->level[row][col] = '1';
                        game->score += 50;
                    }

                    if (brick == '3') {
                        game->level[row][col] = '2';
                        game->score += 25;
                    }

                    if (brick == 'X') {
                        game->level[row][col] = 'X';
                    }

                    // colisão com tijolo no eixo y
                    if (game->ballX > brickX && game->ballX < brickX + 30) {

                        game->ballSpeedY *= -1;

                        // reposiciona a bola fora do tijolo (evitar glitches)
                        if (game->ballSpeedY > 0) {
                            game->ballY = brickY + 20 + game->ballRadius;
                        } else {
                            game->ballY = brickY - game->ballRadius;
                        }
                    }

                    // colisão com tijolo no eixo x
                    else {

                        game->ballSpeedX *= -1;

                        // reposiciona a bola fora do tijolo (evitar glitches)
                        if (game->ballSpeedX > 0) {
                            game->ballX = brickX + 30 + game->ballRadius;
                        } else {
                            game->ballX = brickX - game->ballRadius;
                        }
                    }

                    return;
                }
            }
        }
    }

    // Movimento plataforma
    if (IsKeyDown(KEY_RIGHT)) {
        game->paddleX += game->paddleSpeed;
    }

    if (IsKeyDown(KEY_LEFT)) {
        game->paddleX -= game->paddleSpeed;
    }

    // Limites da plataforma
    if (game->paddleX < 0) {
        game->paddleX = 0;
    }

    if (game->paddleX + game->paddleWidth > 800) {
        game->paddleX = 800 - game->paddleWidth;
    }

    // Colisão da bola com a plataforma
    if (
        CheckCollisionCircleRec(
            (Vector2){game->ballX, game->ballY},
            game->ballRadius,
            (Rectangle){
                game->paddleX,
                game->paddleY,
                game->paddleWidth,
                game->paddleHeight
            }
        )
    ) {
        game->ballSpeedY *= -1;
        game->ballY = game->paddleY - game->ballRadius;
    }

    // Bola caiu para fora da tela
    if (game->ballY > 600) {

        game->lives--;

        game->ballX = 400;
        game->ballY = 300;
        game->ballSpeedX = 5;
        game->ballSpeedY = -5;
    }

    // Fim de jogo
    if (game->lives <= 0) {
        *currentScreen = GAMEOVER;
    }

    // Quando acaba a fase, reinicializa as posições e carrega a nova fase
    if (LevelCompleted(game)) {
        game->currentLevel++;
        LoadCurrentLevel(game, currentScreen);
    }

    // Salvar jogo
    if (IsKeyPressed(KEY_F5)) {
        SaveGame(game);
    }
}

// Lógica de desenho do jogo
void DrawGame(Game *game) {

    // HUD
    DrawText(
        TextFormat("Vidas: %d", game->lives),
        20,
        10,
        20,
        WHITE
    );

    DrawText(
        TextFormat("Score: %d", game->score),
        200,
        10,
        20,
        WHITE
    );

    DrawText(
        TextFormat("Nível: %d", game->currentLevel),
        400,
        10,
        20,
        WHITE
    );

    DrawLevel(game->level);

    // Bola
    DrawCircle(
        game->ballX,
        game->ballY,
        game->ballRadius,
        WHITE
    );

    // Plataforma
    DrawRectangle(
        game->paddleX,
        game->paddleY,
        game->paddleWidth,
        game->paddleHeight,
        WHITE
    );
}