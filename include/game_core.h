#ifndef GAME_CORE_H
#define GAME_CORE_H

#include "game.h"
#include "power_up.h"

// Inicializa estado completo do jogo
void InitGame(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]);

// Atualiza um frame da lógica principal do jogo
void UpdateGame(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]);

// Reseta posição da plataforma e das bolas entre fases
void ResetPositions(Game *game);

// Inicializa os campos básicos da estrutura Game
void InitGameState(Game *game);

// Limpa a matriz de power-ups associada ao nível atual
void InitLevelPowerUps(Game *game);

// Inicializa o array de power-ups ativos
void InitPowerUps(PowerUp powerUps[MAX_POWERUPS]);

// Reseta subsistemas globais do jogo
void ResetSystems(Game *game);

// Desativa o efeito fireball quando o tempo expira
void UpdateFireBall(Game *game, float currentTime);

// Aplica aumentos periódicos de velocidade às bolas
void UpdateBallSpeed(Game *game, float currentTime);

// Atualiza movimento e limites da plataforma
void UpdatePaddle(Game *game);

// Limpa todos os power-ups ativos ao finalizar uma fase
void ResetActivePowerUps(PowerUp powerUps[MAX_POWERUPS]);

#endif