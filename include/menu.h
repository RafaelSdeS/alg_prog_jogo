#ifndef MENU_H
#define MENU_H

#include "game.h"
#include "game_state.h"
#include "power_up.h"

// Desenhar o menu
void DrawMenu(void);

// Atualizar o menu com a tela atual
void UpdateMenu(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]);

#endif