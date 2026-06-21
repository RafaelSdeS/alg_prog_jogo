/*
 * Definição central do estado do jogo e interface pública do sistema.
 *
 * Este header concentra duas responsabilidades principais:
 *
 * - Estrutura Game:
 *   Contém todo o estado global necessário para simulação e renderização,
 *   incluindo:
 *   - estado da bola(s) e cache derivado
 *   - estado do paddle
 *   - sistema de progresso (vidas, score, nível)
 *   - estado de power-ups e colisões por nível
 *   - matriz do level atual
 *   - dados de entrada do jogador (nome)
 *
 * - API pública do jogo:
 *   Declara as funções principais usadas por outros módulos, incluindo:
 *   - ciclo de vida (InitGame, UpdateGame, ResetPositions)
 *   - lógica de nível e colisão de blocos
 *   - gerenciamento de power-ups e progressão
 *   - renderização da UI do jogo
 *
 * Este arquivo funciona como ponto único de referência do domínio "Game",
 * permitindo coordenação entre subsistemas sem expor detalhes internos de implementação.
 *
 * Ele atua como contrato entre módulos e evita acoplamento direto com arquivos de implementação (game_core.c, brick.c, etc.).
 */

#ifndef GAME_H
#define GAME_H

#include "game_state.h"
#include "config.h"
#include "level.h"
#include "power_up.h"
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

// Inicializa estado completo do jogo
void InitGame(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]);

// Atualiza lógica principal do jogo
void UpdateGame(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]);

// Reseta posições e estado entre fases
void ResetPositions(Game *game);

// Detecta colisão da bola com blocos e aplica efeitos
void HandleBallBrickCollision(Game *game, Ball *ball, PowerUp powerUps[MAX_POWERUPS]);

// Consome o power-up associado a um bloco destruído
int ConsumeLevelPowerUp(Game *game, int row, int col);

// Define quais blocos conterão power-ups na fase
void PrepareLevelPowerUps(Game *game);

// Verifica se todos os blocos destrutíveis foram eliminados
int LevelCompleted(Game *game);

// Carrega o nível atual do jogo
void LoadCurrentLevel(Game *game, GameState *currentScreen);

// Renderiza a tela principal do jogo
void DrawGame(Game *game, PowerUp powerUps[MAX_POWERUPS]);

// Renderiza a tela de game over
void DrawGameOverScreen(Game *game);

// Renderiza a tela de vitória
void DrawWinScreen(Game *game);

// Renderiza a tela de entrada de nome
void DrawEnterNameScreen(Game *game);

#endif