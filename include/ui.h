#ifndef UI_H
#define UI_H

#include "game.h"
#include "power_up.h"
#include "level.h"

// Renderização principal do jogo (HUD + elementos visuais)
void DrawGame(Game *game, PowerUp powerUps[MAX_POWERUPS]);

// Tela de game over
void DrawGameOverScreen(Game *game);

// Tela de vitória
void DrawWinScreen(Game *game);

// Tela de entrada de nome
void DrawEnterNameScreen(Game *game);

// Tela de ranking
void DrawRanking(void);

// Desenhar level
void DrawLevel(char level[ROWS][COLS]);

#endif
