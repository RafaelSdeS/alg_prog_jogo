#ifndef SAVE_H
#define SAVE_H

#include "game.h"
#include "game_state.h"

#define MAX_SAVES 20

// Salvar jogo
void SaveGame(Game *game);

// Carregar jogo
int LoadGame(Game *game, const char *filename);

// Atualizar tela de saves
void UpdateSaveSelection(Game *game, GameState *currentScreen);

// Desenhar tela de saves
void DrawSaveSelection();

// Atualizar lista de saves
void RefreshSaveList();

#endif