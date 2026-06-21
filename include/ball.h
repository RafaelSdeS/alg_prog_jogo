#ifndef BALL_H
#define BALL_H

#include <raylib.h>

#include "config.h"
#include "power_up.h"

// Forward declaration -> Permitir que o compilador saiba a existência de uma struct
// https://www.delftstack.com/howto/c/forward-declaration-in-c
// https://en.wikipedia.org/wiki/Forward_declaration
typedef struct Game Game;

// Estrutura da bola (possivelmente criar múltiplas bolas ao mesmo tempo)
typedef struct Ball {
    float x;
    float y;
    float speedX;
    float speedY;
    float radius;
    int active;
} Ball;

// Atualiza campos derivados da primeira bola ativa
void SyncPrimaryBallFields(Game *game);

// Conta quantas bolas estão ativas
int CountActiveBalls(const Game *game);

// Reseta sistema para uma única bola inicial
void ResetSingleBall(Game *game);

// Retorna ponteiro para a primeira bola ativa
Ball *GetFirstActiveBall(Game *game);

// Desenha uma bola individual
void DrawSingleBall(const Ball *ball, const Game *game);

// Reinicia sistema de bolas
void ResetBallSystem(Game *game);

// Aplica multiplicador de velocidade em todas as bolas
void ApplyBallSpeedMultiplier(Game *game, float factor);

// Cria multiball a partir da bola principal
void ActivateMultiBall(Game *game);

// Atualiza aumento global de velocidade das bolas
void UpdateBallSpeedIncrease(Game *game);

// Atualiza uma bola individual
void UpdateSingleBall(Game *game, Ball *ball, PowerUp powerUps[MAX_POWERUPS]);

#endif
