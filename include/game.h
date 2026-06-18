#ifndef GAME_H
#define GAME_H

#include "..\include\game.h"
#include "..\include\game_state.h"
#include "..\include\config.h"
#include "..\include\level.h"
#include "..\include\PowerUp.h"

// Struct para salvar os dados atuais do jogo
typedef struct Game {
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
void InitGame(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]);

// Atualizar o jogo (60 fps; cuida da lógica)
void UpdateGame(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]);

// Desenhar o jogo (cuida apenas do visual, lógica fica em UpdateGame)
void DrawGame(Game *game);

// Carregar nível atual e passar de fase
void LoadCurrentLevel(Game *game, GameState *currentScreen);

// Telas finais
void DrawGameOverScreen(Game *game);
void DrawWinScreen(Game *game);
void DrawEnterNameScreen(Game *game);

#endif