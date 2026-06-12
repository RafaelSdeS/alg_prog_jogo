#ifndef GAME_H
#define GAME_H

#include "game_state.h"
#include "config.h"

// Struct para salvar os dados atuais do jogo
typedef struct {
    // Bola
    float ballX, ballY;
    float ballSpeedX, ballSpeedY;
    float ballRadius;

    // Paddle
    float paddleX, paddleY;
    float paddleWidth, paddleHeight;
    float paddleSpeed;

    // Sistema
    int lives;
    int score;
    int currentLevel;

    // Level
    char level[ROWS][COLS];

    char playerName[50];
    int playerNameLength;

} Game;

// Inicializar o jogo
void InitGame(Game *game, GameState *currentScreen);

// Atualizar o jogo (60 fps; cuida da lógica)
void UpdateGame(Game *game, GameState *currentScreen);

// Desenhar o jogo (cuida apenas do visual, lógica fica em UpdateGame)
void DrawGame(Game *game);

// Carregar nível atual e passar de fase
void LoadCurrentLevel(Game *game, GameState *currentScreen);

// Desenhar tela de game over
void DrawGameOverScreen(Game *game);

// Desenhar tela de vitória
void DrawWinScreen(Game *game);

// Desenhar tela para inserir nome
void DrawEnterNameScreen(Game *game);

#endif