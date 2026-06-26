#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "ball.h"

// Struct para salvar os dados atuais do jogo
typedef struct Game {
    // Bola
    float ballX, ballY;
    float ballSpeedX, ballSpeedY;
    float ballRadius;

    // Plataforma
    float paddleX, paddleY;
    float paddleWidth, paddleHeight;
    float paddleSpeed;

    // Sistema
    int lives;
    int score;
    int currentLevel;
    int fireBallActive;
    double fireBallEndTime;
    int ballCount;

    // Array com as bolas (em tela e não em tela que podem ser ativadas)
    Ball balls[MAX_BALLS];

    // Matriz de powerups do nível
    int levelPowerUps[ROWS][COLS];

    // Level
    char level[ROWS][COLS];

    // Nome do jogador para guardar no ranking
    char playerName[50];
    int playerNameLength;

} Game;

#endif