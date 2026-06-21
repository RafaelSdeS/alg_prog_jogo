/*
 * Sistema de gerenciamento de bolas do jogo.
 *
 * Responsável por:
 * - Controle do ciclo de vida das bolas (criação, atualização e remoção)
 * - Implementação de multiball e clonagem de estados
 * - Física básica (movimento e colisões com paredes, paddle e limites da tela)
 * - Integração com colisão de blocos e sistema de power-ups
 * - Controle de progressão de dificuldade via aumento de velocidade
 *
 * Este módulo mantém também um "cache" de estado da primeira bola ativa
 * dentro da struct Game para facilitar acesso por outros sistemas (UI e lógica).
 */

#include <math.h>
#include <stdlib.h>

#include "ball.h"
#include "game.h"

// Atualiza os campos principais da primeira bola ativa
// Essa função mantém um "cache" de conveniência dentro da struct Game
// para evitar precisar buscar a primeira bola ativa o tempo todo
void SyncPrimaryBallFields(Game *game) {

    // Percorre todas as bolas procurando a primeira ativa
    for (int i = 0; i < MAX_BALLS; i++) {
        if (game->balls[i].active) {

            // Copia posição atual da bola ativa para variáveis globais do Game
            // Essas variáveis são usadas por outros sistemas (UI, lógica, etc.)
            game->ballX = game->balls[i].x;
            game->ballY = game->balls[i].y;

            // Copia velocidade da bola principal (primeira ativa encontrada)
            game->ballSpeedX = game->balls[i].speedX;
            game->ballSpeedY = game->balls[i].speedY;

            // Copia raio (caso power-ups alterem tamanho da bola)
            game->ballRadius = game->balls[i].radius;

            // Interrompe no primeiro resultado válido
            return;
        }
    }

    // Caso nenhuma bola esteja ativa, reseta estado derivado
    // Isso evita inconsistência de valores antigos na UI ou lógica
    game->ballX = 0.0f;
    game->ballY = 0.0f;
    game->ballSpeedX = 0.0f;
    game->ballSpeedY = 0.0f;
    game->ballRadius = INITIAL_BALL_RADIUS;
}

// Conta quantas bolas estão ativas no jogo
// Usado principalmente para multiball e controle de vida/estado
int CountActiveBalls(const Game *game) {

    int count = 0;

    // Percorre todas as slots de bolas e soma as ativas
    for (int i = 0; i < MAX_BALLS; i++) {
        if (game->balls[i].active) {
            count++;
        }
    }

    return count;
}

// Reseta o sistema para uma única bola inicial no centro
// Remove qualquer multiball e restaura estado padrão do jogo
void ResetSingleBall(Game *game) {

    // Desativa todas as bolas existentes no sistema
    for (int i = 0; i < MAX_BALLS; i++) {
        game->balls[i].active = 0;
    }

    // Define posição inicial da bola principal
    game->balls[0].x = 400.0f;
    game->balls[0].y = 300.0f;

    // Define velocidade inicial padrão
    game->balls[0].speedX = INITIAL_BALL_SPEED_X;
    game->balls[0].speedY = INITIAL_BALL_SPEED_Y;

    // Define tamanho padrão da bola
    game->balls[0].radius = INITIAL_BALL_RADIUS;

    // Ativa a bola principal
    game->balls[0].active = 1;

    // Atualiza contador de bolas no Game
    game->ballCount = 1;

    // Sincroniza estado derivado do Game com a bola ativa
    SyncPrimaryBallFields(game);
}

// Retorna ponteiro para a primeira bola ativa encontrada
// Útil para criar multiball ou replicar comportamento da bola principal
Ball *GetFirstActiveBall(Game *game) {

    for (int i = 0; i < MAX_BALLS; i++) {
        if (game->balls[i].active) {
            return &game->balls[i];
        }
    }

    // Se nenhuma bola estiver ativa, retorna NULL
    return NULL;
}

// Desenha uma bola individual na tela e muda a cor se o powerup de fire estiver ativo
void DrawSingleBall(const Ball *ball, const Game *game) {
    Color color = WHITE;

    if (game->fireBallActive) {
        color = ORANGE; // ou RED, GOLD, etc.
    }

    DrawCircle(ball->x, ball->y, ball->radius, color);
}

// Reinicia completamente o sistema de bolas
// Wrapper para ResetSingleBall (ponto de abstração)
void ResetBallSystem(Game *game) {
    ResetSingleBall(game);
}

// Aplica multiplicador de velocidade em todas as bolas ativas
// Usado por power-ups de speed e eventos globais de dificuldade
void ApplyBallSpeedMultiplier(Game *game, float factor) {

    for (int i = 0; i < MAX_BALLS; i++) {

        // Ignora slots vazios
        if (!game->balls[i].active) {
            continue;
        }

        // Multiplica velocidade atual da bola
        game->balls[i].speedX *= factor;
        game->balls[i].speedY *= factor;
    }

    // Atualiza cache da bola principal
    SyncPrimaryBallFields(game);
}

// Retornar valor absoluto
float AbsoluteValue(float value) {
    return value < 0 ? -value : value;
}

// Cria sistema de multiball a partir da primeira bola ativa
// Duplica a bola original com variações de direção e posição
void ActivateMultiBall(Game *game) {

    // Evita ultrapassar limite máximo de bolas
    if (CountActiveBalls(game) >= MAX_BALLS) {
        return;
    }

    // Obtém bola base para clonagem
    Ball *templateBall = GetFirstActiveBall(game);

    int added = 0;

    // Cria até 2 novas bolas a partir da base
    for (int i = 0; i < MAX_BALLS && added < 2; i++) {

        // Procura slot livre no array de bolas
        if (game->balls[i].active) {
            continue;
        }

        // Copia estado da bola base
        Ball newBall = *templateBall;

        // Ativa nova bola
        newBall.active = 1;

        // Ajusta posição vertical para evitar sobreposição imediata
        newBall.y = templateBall->y - newBall.radius - 1.0f;

        // Define direção horizontal diferente para cada nova bola
        // https://en.cppreference.com/c/numeric/math/fabs
        if (added == 0) {
            newBall.x = templateBall->x - 14.0f;
            newBall.speedX = -AbsoluteValue(templateBall->speedX) * 0.85f;
        } else {
            newBall.x = templateBall->x + 14.0f;
            newBall.speedX = AbsoluteValue(templateBall->speedX) * 0.85f;
        }

        // Sempre inicia movimento vertical para cima
        newBall.speedY = -AbsoluteValue(templateBall->speedY);

        // Insere nova bola no sistema
        game->balls[i] = newBall;
        added++;
    }

    // Atualiza contagem e sincroniza estado global
    game->ballCount = CountActiveBalls(game);
    SyncPrimaryBallFields(game);
}

// Aumenta gradualmente a velocidade global das bolas ao longo do tempo
// Mantém dificuldade crescente controlada por limite máximo
void UpdateBallSpeedIncrease(Game *game) {

    float maxSpeed = 0.0f;

    // Calcula velocidade máxima atual entre todas as bolas ativas
    for (int i = 0; i < MAX_BALLS; i++) {

        if (!game->balls[i].active) {
            continue;
        }

        // Calcula magnitude da velocidade (vetor 2D)
        float speed = sqrtf(
            game->balls[i].speedX * game->balls[i].speedX +
            game->balls[i].speedY * game->balls[i].speedY
        );

        // Mantém maior velocidade encontrada
        if (speed > maxSpeed) {
            maxSpeed = speed;
        }
    }

    // Se já atingiu limite máximo, não aplica mais aumento
    if (maxSpeed >= BALL_SPEED_MAX) {
        return;
    }

    // Aplica multiplicador de aumento em todas as bolas ativas
    for (int i = 0; i < MAX_BALLS; i++) {

        if (!game->balls[i].active) {
            continue;
        }

        game->balls[i].speedX *= BALL_SPEED_INCREASE_FACTOR;
        game->balls[i].speedY *= BALL_SPEED_INCREASE_FACTOR;
    }

    // Sincroniza estado global após alteração de velocidade
    SyncPrimaryBallFields(game);
}

// Atualiza física e comportamento de uma única bola
// Inclui movimento, colisão com paredes, paddle e blocos
void UpdateSingleBall(Game *game, Ball *ball, PowerUp powerUps[MAX_POWERUPS]) {

    // Atualiza posição com base na velocidade atual
    ball->x += ball->speedX;
    ball->y += ball->speedY;

    // Colisão com paredes laterais (inverte direção horizontal)
    if (ball->x <= ball->radius || ball->x >= SCREEN_WIDTH - ball->radius) {
        ball->speedX *= -1;
    }

    // Colisão com teto (inverte direção vertical)
    if (ball->y <= ball->radius) {
        ball->speedY *= -1;
    }

    // Colisão com blocos (destruição e score)
    HandleBallBrickCollision(game, ball, powerUps);

    // Caso a bola saia da tela inferior, ela é removida
    if (ball->y > SCREEN_HEIGHT) {
        ball->active = 0;
        return;
    }

    // Colisão com paddle do jogador
    if (CheckCollisionCircleRec(
        (Vector2){ball->x, ball->y},
        ball->radius,
        (Rectangle){
            game->paddleX,
            game->paddleY,
            game->paddleWidth,
            game->paddleHeight
        })) {

        // Inverte direção vertical ao bater no paddle
        ball->speedY *= -1;

        // Ajusta posição para evitar "grudar" no paddle
        ball->y = game->paddleY - ball->radius;
    }
}