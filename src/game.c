#include <raylib.h>

#include "game.h"
#include "level.h"
#include "save.h"
#include "game_state.h"

// Carregar o nível atual
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

// Detecta se o nível foi concluído
int LevelCompleted(Game *game) {

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (game->level[row][col] == '1' || game->level[row][col] == '2' || game->level[row][col] == '3') {
                return 0;
            }
        }
    }

    return 1;
}

// Reiniciar posição da bola
void ResetBall(Game *game) {

    game->ballX = 400;
    game->ballY = 300;

    game->ballSpeedX = 5;
    game->ballSpeedY = -5;
}

void ResetPositions(Game *game) {

    ResetBall(game);

    game->paddleX = 350;
    game->paddleY = 550;
}

// Inicializar jogo
void InitGame(Game *game, GameState *currentScreen) {

    game->playerName[0] = '\0';

    game->lives = 3;
    game->score = 0;
    game->currentLevel = 1;

    game->ballRadius = 10;

    game->paddleWidth = 100;
    game->paddleHeight = 20;
    game->paddleSpeed = 7;

    game->paddleX = 350;
    game->paddleY = 550;

    ResetBall(game);

    LoadCurrentLevel(game, currentScreen);
}

// Atualizar jogo
void UpdateGame(Game *game, GameState *currentScreen) {

    game->ballX += game->ballSpeedX;
    game->ballY += game->ballSpeedY;

    // Colisão com paredes
    if (game->ballX <= game->ballRadius ||
        game->ballX >= 800 - game->ballRadius) {

        game->ballSpeedX *= -1;
    }

    if (game->ballY <= game->ballRadius) {
        game->ballSpeedY *= -1;
    }

    // Colisão com tijolos
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            char brick = game->level[row][col];

            if (brick != '1' &&
                brick != '2' &&
                brick != '3' &&
                brick != 'X') {
                continue;
            }

            int brickX = col * 30;
            int brickY = row * 20 + 50;

            if (!CheckCollisionCircleRec(
                    (Vector2){game->ballX, game->ballY},
                    game->ballRadius,
                    (Rectangle){brickX, brickY, 30, 20})) {
                continue;
            }

            if (brick == '1') {
                game->level[row][col] = '0';
                game->score += 100;
            }

            else if (brick == '2') {
                game->level[row][col] = '1';
                game->score += 50;
            }

            else if (brick == '3') {
                game->level[row][col] = '2';
                game->score += 25;
            }

            if (game->ballX > brickX &&
                game->ballX < brickX + 30) {

                game->ballSpeedY *= -1;

                if (game->ballSpeedY > 0) {
                    game->ballY = brickY + 20 + game->ballRadius;
                }

                else {
                    game->ballY = brickY - game->ballRadius;
                }
            }

            else {

                game->ballSpeedX *= -1;

                if (game->ballSpeedX > 0) {
                    game->ballX = brickX + 30 + game->ballRadius;
                }

                else {
                    game->ballX = brickX - game->ballRadius;
                }
            }

            return;
        }
    }

    // Movimento da plataforma
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

    // Colisão com a plataforma
    if (CheckCollisionCircleRec(
            (Vector2){game->ballX, game->ballY},
            game->ballRadius,
            (Rectangle){
                game->paddleX,
                game->paddleY,
                game->paddleWidth,
                game->paddleHeight
            })) {

        game->ballSpeedY *= -1;
        game->ballY = game->paddleY - game->ballRadius;
    }

    // Bola caiu
    if (game->ballY > 600) {
        game->lives--;
        ResetBall(game);
    }

    // Derrota
    if (game->lives <= 0) {
        *currentScreen = GAMEOVER;
        return;
    }

    // Próxima fase
    if (LevelCompleted(game)) {
        ResetPositions(game);
        game->currentLevel++;
        LoadCurrentLevel(game, currentScreen);
    }

    // Salvar jogo
    if (IsKeyPressed(KEY_F5)) {
        SaveGame(game);
    }
}

// Desenhar jogo
void DrawGame(Game *game) {

    DrawText(TextFormat("Vidas: %d", game->lives), 20, 10, 20, WHITE);

    DrawText(TextFormat("Score: %d", game->score), 200, 10, 20, WHITE);

    DrawText(TextFormat("Nível: %d", game->currentLevel), 400, 10, 20, WHITE);

    DrawLevel(game->level);

    DrawCircle(game->ballX, game->ballY, game->ballRadius, WHITE);

    DrawRectangle(game->paddleX, game->paddleY, game->paddleWidth, game->paddleHeight, WHITE);
}

// Tela de derrota
void DrawGameOverScreen(Game *game) {

    DrawText("GAME OVER", 220, 200, 60, RED);

    DrawText(TextFormat("SCORE FINAL: %d", game->score), 220, 300, 30, WHITE);

    DrawText("Pressione ENTER", 220, 400, 30, GRAY);
}

// Tela de vitória
void DrawWinScreen(Game *game) {

    DrawText("YOU WON!", 220, 200, 60, YELLOW);

    DrawText(TextFormat("SCORE FINAL: %d", game->score), 220, 300, 30, WHITE);

    DrawText("Pressione ENTER", 220, 400, 30, GRAY);
}

// Tela para digitar nome
void DrawEnterNameScreen(Game *game) {

    DrawText("NOVO TOP SCORE!", 180, 150, 40, YELLOW);

    DrawText("Digite seu nome:", 180, 250, 30, WHITE);

    DrawRectangleLines(180, 300, 400, 50, WHITE);

    DrawText(game->playerName, 190, 310, 30, WHITE);

    DrawText("ENTER para confirmar", 180, 400, 25, GRAY);
}