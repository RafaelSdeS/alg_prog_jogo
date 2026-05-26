#ifndef LEVEL_H
#define LEVEL_H

#define ROWS 15
#define COLS 25

// Carregar o nível
void LoadLevel(char level[ROWS][COLS], char *filename);

// Desenhar o nível
void DrawLevel(char level[ROWS][COLS]);

#endif