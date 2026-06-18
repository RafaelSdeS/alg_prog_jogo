#ifndef PowerUp_h
#define PowerUp_h
#define MAX_POWERUPS 10

#include "..\include\PowerUp.h"
#include <raylib.h>
#include <time.h>
#include <stdlib.h>


typedef struct {
    float x, y;
    int type;
    int active;
    int spawnTime;
    int duracao;
} PowerUp;

typedef struct Game Game;

void DesativaPowerUp(PowerUp *powerUp, Game *game);
void SpawnPowerUp(PowerUp powerUps[MAX_POWERUPS], int Row, int Col);
void DrawPowerUp(PowerUp powerUps[MAX_POWERUPS]);
void UpdatePowerUp(PowerUp *powerUp, Game *game);
void CheckPowerUpCollision(PowerUp powerUps[MAX_POWERUPS], Game *game);
void AtivaPowerUp(PowerUp *powerUp, Game *game);
int SorteioPowerUps();

#endif