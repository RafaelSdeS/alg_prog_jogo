#include <raylib.h>
#include "game.h"
#include "level.h"

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

// Matriz do nível
char level[ROWS][COLS];

void InitGame() {

    // Bola
    ballX = 400;
    ballY = 300;

    ballRadius = 10;

    ballSpeedX = 5;
    ballSpeedY = -5;

    // Plataforma
    paddleWidth = 100;
    paddleHeight = 20;

    paddleX = 350;
    paddleY = 550;

    paddleSpeed = 7;

    // Carregar nível
    LoadLevel(level, "fases/fase1.txt");
}

void UpdateGame() {

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

    //colisão com tijolos
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
                    if (brick =='1')
                    {
                        //remove o tijolo
                        level[row][col] = '0';
                    }
                    if (brick =='2')
                    {
                        //diminui em 1 a resistencia do tijolo
                        level[row][col] = '1';
                    }
                    if (brick =='3')
                    {
                        //diminui em 1 a resistencia do tijolo
                        level[row][col] = '2';
                    }
                    if (brick =='X')
                    {
                        //remove o tijolo
                        level[row][col] = 'X';
                    }
                    // colisão com tijolo no eixo y
                    if(
                        ballX > brickX &&
                        ballX < brickX + 30
                    ) {
                        ballSpeedY *= -1;
                    }
                    // colisão com tijolo no eixo x
                    else {
                        ballSpeedX *= -1;
                    }
                    
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

        // Reinicia posição da bola
        ballX = 400;
        ballY = 300;

        ballSpeedX = 5;
        ballSpeedY = -5;
    }
}

void DrawGame() {

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

    DrawLevel(level);
}