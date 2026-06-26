#ifndef SAVE_H
#define SAVE_H

#include "game.h"
#include "game_state.h"

// Máximo número de saves
#define MAX_SAVES 20

// Salvar jogo
void SaveGame(Game *game);

// Carregar jogo
int LoadGame(Game *game, const char *filename);

// Atualizar tela de saves
void UpdateSaveSelection(Game *game, GameState *currentScreen);

// Desenhar tela de saves
void DrawSaveSelection(void);

// Atualizar lista de saves
void RefreshSaveList(void);

#endif