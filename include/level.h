#ifndef LEVEL_H
#define LEVEL_H
#include "..\include\config.h"

// Carregar o nível
void LoadLevel(char level[ROWS][COLS], char *filename);

// Desenhar o nível
void DrawLevel(char level[ROWS][COLS]);

#endif