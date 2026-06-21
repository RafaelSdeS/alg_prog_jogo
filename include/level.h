#ifndef LEVEL_H
#define LEVEL_H
#include "config.h"
#include "game_state.h"

typedef struct Game Game;

// Carregar o nível
void LoadLevel(char level[ROWS][COLS], char *filename);

// Desenhar o nível
void DrawLevel(char level[ROWS][COLS]);

// Carrega o nível atual baseado no progresso do jogo
void LoadCurrentLevel(Game *game, GameState *currentScreen);

#endif