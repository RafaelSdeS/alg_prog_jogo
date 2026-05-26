#ifndef GAME_H
#define GAME_H

// Inicializar o jogo
void InitGame(int *currentScreen);

// Atualizar o jogo (60 fps; cuida da lógica)
void UpdateGame(int *currentScreen);

// Desenhar o jogo (cuida apenas do visual, lógica fica em UpdataGame)
void DrawGame();

// Carregar nível atual e passar de fase
void LoadCurrentLevel();

extern int score;

#endif