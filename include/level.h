#ifndef LEVEL_H
#define LEVEL_H

#define ROWS 15
#define COLS 25

void LoadLevel(char level[ROWS][COLS], char *filename);

void DrawLevel(char level[ROWS][COLS]);

#endif