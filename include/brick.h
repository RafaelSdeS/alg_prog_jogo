#ifndef BRICK_H
#define BRICK_H

#include "game.h"
#include "power_up.h"

// Zera completamente o mapa de power-ups do nível atual
void ClearLevelPowerUps(void);

// Pré-define posições de power-ups no nível com base nos blocos existentes
// Escolhe aleatoriamente quais blocos irão conter power-ups
void PrepareLevelPowerUps(Game *game);

// Detecta colisão da bola com blocos e aplica lógica de destruição
// Também controla score e spawn de power-ups
void HandleBallBrickCollision(Game *game, Ball *ball, PowerUp powerUps[MAX_POWERUPS]);

// Consome power-up associado a um bloco
int ConsumeLevelPowerUp(Game *game, int row, int col);

// Verifica se todos os blocos destrutíveis foram eliminados
int LevelCompleted(Game *game);

#endif
