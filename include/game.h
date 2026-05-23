#ifndef GAME_H
#define GAME_H

void InitGame(int *currentScreen);
void UpdateGame(int *currentScreen);
void DrawGame();
void LoadCurrentLevel();

extern int score;

#endif