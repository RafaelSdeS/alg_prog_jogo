#ifndef GAME_H
#define GAME_H

#include "game.h"
#include "game_state.h"
#include "config.h"

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

} Game;

// Inicializar o jogo
void InitGame(Game *game, GameState *currentScreen);

// Atualizar o jogo (60 fps; cuida da lógica)
void UpdateGame(Game *game, GameState *currentScreen);

// Desenhar o jogo (cuida apenas do visual, lógica fica em UpdateGame)
void DrawGame(Game *game);

// Carregar nível atual e passar de fase
void LoadCurrentLevel(Game *game, GameState *currentScreen);

#endif