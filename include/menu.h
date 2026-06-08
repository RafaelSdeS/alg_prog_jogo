#ifndef MENU_H
#define MENU_H

#include "game.h"
#include "game_state.h"

// Desenhar o menu
void DrawMenu();

// Atualizar o menu com a tela atual
void UpdateMenu(Game *game, GameState *currentScreen);

#endif