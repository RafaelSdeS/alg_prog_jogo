#ifndef MENU_H
#define MENU_H

#include "..\include\game.h"
#include "..\include\game_state.h"
#include "..\include\PowerUp.h"

// Desenhar o menu
void DrawMenu();

// Atualizar o menu com a tela atual
void UpdateMenu(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]);

#endif