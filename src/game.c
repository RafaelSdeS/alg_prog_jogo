#include <raylib.h>
#include "game.h"
#include "level.h"

#define MENU 0
#define GAME 1
#define GAMEOVER 2

// Bola
float ballX;
float ballY;

float ballRadius;

float ballSpeedX;
float ballSpeedY;

// Plataforma
float paddleX;
float paddleY;

float paddleWidth;
float paddleHeight;

float paddleSpeed;

// Sistema do jogo
int lives;
int score;
int currentLevel;

// Matriz do nível
char level[ROWS][COLS];

void ResetBallAndPaddle() {

    // Bola
    ballX = 400;
    ballY = 300;

    ballSpeedX = 5;
    ballSpeedY = -5;

    // Plataforma
    paddleX = 350;
    paddleY = 550;
}

void InitGame() {

    // Sistema do jogo
    lives = 3;
    score = 0;
    currentLevel = 1;

    // Bola
    ballRadius = 10;

    // Plataforma
    paddleWidth = 100;
    paddleHeight = 20;

    paddleSpeed = 7;

    // Reinicia bola e plataforma
    ResetBallAndPaddle();

    // Carregar nível
    LoadLevel(level, "fases/fase1.txt");
}

void UpdateGame(int *currentScreen) {

    // Movimento da bola
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Colisão com paredes
    if (ballX >= 800 - ballRadius ||
        ballX <= ballRadius) {

        ballSpeedX *= -1;
    }

    if (ballY <= ballRadius) {

        ballSpeedY *= -1;
    }

    // colisão com tijolos
    for (int row = 0; row < ROWS; row++) {

        for (int col = 0; col < COLS; col++) {

            char brick = level[row][col];

            if (brick == '1' || brick == '2' || brick == '3' || brick == 'X') {

                int brickX = col * 30;
                int brickY = row * 20 + 50;

                if (
                    CheckCollisionCircleRec(
                        (Vector2){ballX, ballY},
                        ballRadius,
                        (Rectangle){
                            brickX,
                            brickY,
                            30,
                            20
                        }
                    )
                ) {

                    if (brick == '1')
                    {
                        // remove o tijolo
                        level[row][col] = '0';

                        // adiciona score
                        score += 100;
                    }

                    if (brick == '2')
                    {
                        // diminui em 1 a resistencia do tijolo
                        level[row][col] = '1';

                        // adiciona score
                        score += 50;
                    }

                    if (brick == '3')
                    {
                        // diminui em 1 a resistencia do tijolo
                        level[row][col] = '2';

                        // adiciona score
                        score += 25;
                    }

                    if (brick == 'X')
                    {
                        // mantém o tijolo
                        level[row][col] = 'X';
                    }

                    // colisão com tijolo no eixo y
                    if (
                        ballX > brickX &&
                        ballX < brickX + 30
                    ) {

                        ballSpeedY *= -1;

                        // reposiciona a bola fora do tijolo

                        if (ballSpeedY > 0) {
                            ballY = brickY + 20 + ballRadius;
                        }

                        else {
                            ballY = brickY - ballRadius;
                        }
                    }

                    // colisão com tijolo no eixo x
                    else {

                        ballSpeedX *= -1;

                        // reposiciona a bola fora do tijolo

                        if (ballSpeedX > 0) {
                            ballX = brickX + 30 + ballRadius;
                        }

                        else {
                            ballX = brickX - ballRadius;
                        }
                    }

                    return;
                }
            }
        }
    }

    // Movimento plataforma
    if (IsKeyDown(KEY_RIGHT)) {
        paddleX += paddleSpeed;
    }

    if (IsKeyDown(KEY_LEFT)) {
        paddleX -= paddleSpeed;
    }

    // Limites da plataforma
    if (paddleX < 0) {
        paddleX = 0;
    }

    if (paddleX + paddleWidth > 800) {
        paddleX = 800 - paddleWidth;
    }

    // Colisão da bola com a plataforma
    if (
        CheckCollisionCircleRec(
            (Vector2){ballX, ballY},
            ballRadius,
            (Rectangle){
                paddleX,
                paddleY,
                paddleWidth,
                paddleHeight
            }
        )
    ) {

        // Faz a bola subir
        ballSpeedY *= -1;

        // Evita que a bola grude na plataforma
        ballY = paddleY - ballRadius;
    }

    // Bola caiu para fora da tela
    if (ballY > 600) {

        // Perde uma vida
        lives--;

        // Reinicia bola e plataforma
        ResetBallAndPaddle();
    }

    // Fim de jogo
    if (lives <= 0) {

        // Reinicia o jogo
        *currentScreen = GAMEOVER;
    }
}

void DrawGame() {

    // HUD
    DrawText(
        TextFormat("Lives: %d", lives),
        20,
        10,
        20,
        WHITE
    );

    DrawText(
        TextFormat("Score: %d", score),
        200,
        10,
        20,
        WHITE
    );

    DrawText(
        TextFormat("Level: %d", currentLevel),
        400,
        10,
        20,
        WHITE
    );

    DrawLevel(level);

    // Bola
    DrawCircle(
        ballX,
        ballY,
        ballRadius,
        WHITE
    );

    // Plataforma
    DrawRectangle(
        paddleX,
        paddleY,
        paddleWidth,
        paddleHeight,
        WHITE
    );
}