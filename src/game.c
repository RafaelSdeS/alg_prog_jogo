#include "game.h"
#include <raylib.h>

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
}