/*
 * Sistema de gerenciamento de nível e blocos do jogo.
 *
 * Responsável por:
 * - Geração e associação de power-ups aos blocos do nível
 * - Controle do mapa auxiliar de power-ups por célula (levelPowerUpMap)
 * - Detecção de colisão entre bola e blocos
 * - Aplicação da lógica de destruição e degradação dos blocos
 * - Cálculo de pontuação ao atingir blocos
 * - Spawn de power-ups ao destruir blocos associados
 * - Verificação de conclusão do nível
 *
 * Este módulo conecta diretamente o estado do level (game->level)
 * com sistemas de física (ball) e recompensas (power-ups).
 */

#include <raylib.h>
#include <stdlib.h>

#include "brick.h"
#include "game.h"
#include "power_up.h"
#include "ball.h"
#include "audio.h"

// Struct para armazenar coordenadas de blocos candidatos a terem powerups
typedef struct {
    int row;
    int col;
} Position;

// Mapa auxiliar que guarda quais blocos possuem power-up associado
int levelPowerUpMap[ROWS][COLS];

// Zera completamente o mapa de powerups do nível atual
void ClearLevelPowerUps(void) {

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            levelPowerUpMap[row][col] = 0;
        }
    }
}

// Define posições de powerups no nível com base nos blocos existentes
// Escolhe aleatoriamente quais blocos irão conter power-ups
void PrepareLevelPowerUps(Game *game) {

    ClearLevelPowerUps();

    Position candidates[ROWS * COLS];
    int candidateCount = 0;

    // Coleta todos os blocos válidos como candidatos
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            char brick = game->level[row][col];

            // Apenas blocos destrutíveis entram como candidatos
            if (brick == '1' || brick == '2' || brick == '3') {
                candidates[candidateCount].row = row;
                candidates[candidateCount].col = col;
                candidateCount++;
            }
        }
    }

    int powerUpsToPlace = POWERUPS_PER_LEVEL;

    // Ajusta caso haja menos candidatos do que powerups desejados
    if (powerUpsToPlace > candidateCount) {
        powerUpsToPlace = candidateCount;
    }

    // Seleção aleatória -> Chance de um tijolo qualquer ter powerup = candidateCount/powerUpsToPlace
    for (int i = 0; i < powerUpsToPlace; i++) {

        int selectedIndex = rand() % candidateCount;

        int row = candidates[selectedIndex].row;
        int col = candidates[selectedIndex].col;

        // Troca com último elemento e reduz pool
        candidates[selectedIndex] = candidates[candidateCount - 1];
        candidateCount--;

        // Define tipo aleatório de powerup para essa posição
        levelPowerUpMap[row][col] = 1 + rand() % MAX_POWERUP_TYPES;
    }
}

// Detecta colisão entre a bola e os blocos do nível atual e aplica a lógica de destruição, pontuação e powerups.
void HandleBallBrickCollision(Game *game, Ball *ball, PowerUp powerUps[MAX_POWERUPS]) {

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            char brick = game->level[row][col];

            // Ignora células que não representam blocos válidos
            if (brick != '1' &&
                brick != '2' &&
                brick != '3' &&
                brick != 'X') {
                continue;
            }

            int brickX = col * 30;
            int brickY = row * 20 + 50;

            // Verifica colisão da bola com o bloco
            if (!CheckCollisionCircleRec(
                    (Vector2){ball->x, ball->y},
                    ball->radius,
                    (Rectangle){brickX, brickY, 30, 20})) {
                continue;
            }

            // destroyed = 0 -> Tijolo ainda existe
            // Destroyed = 1 -> Tijolo destruído
            int destroyed = 0;
            int scoreGain = 0;

            // Define comportamento do bloco conforme seu tipo
            if (brick == '1') {
                destroyed = 1;
                scoreGain = 100;
                game->level[row][col] = '0';
            } else if (brick == '2') {
                scoreGain = 50;

                // Fireball destrói diretamente blocos intermediários
                if (game->fireBallActive) {
                    destroyed = 1;
                    game->level[row][col] = '0';
                } else {
                    game->level[row][col] = '1';
                }
            } else if (brick == '3') {
                scoreGain = 25;

                if (game->fireBallActive) {
                    destroyed = 1;
                    game->level[row][col] = '0';
                } else {
                    game->level[row][col] = '2';
                }
            }

            // Atualiza pontuação do jogador
            game->score += scoreGain;

            // Se o bloco foi destruído, consome e gera powerup apenas se o powerup type existir naquele bloco (powerUpType > 0)
            if (destroyed) {
                PlayBrickBreakSound();
                int powerUpType = ConsumeLevelPowerUp(game, row, col);
                if (powerUpType > 0) {
                    SpawnPowerUp(powerUps, row, col, powerUpType);
                }
            }

            // Calcula resposta da colisão -> Determina se o impacto foi vertical ou horizontal
            if (ball->x > brickX && ball->x < brickX + 30) {

                ball->speedY *= -1;

                // Ajusta posição para evitar "grudar" no bloco
                if (ball->speedY > 0) {
                    ball->y = brickY + 20 + ball->radius;
                } else {
                    ball->y = brickY - ball->radius;
                }

            } else {

                ball->speedX *= -1;

                // Ajuste horizontal pós-colisão
                if (ball->speedX > 0) {
                    ball->x = brickX + 30 + ball->radius;
                } else {
                    ball->x = brickX - ball->radius;
                }
            }

            return;
        }
    }
}

// Consome power-up associado a um tijolo
int ConsumeLevelPowerUp(Game *game, int row, int col) {

    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        return 0;
    }

    int powerUpType = levelPowerUpMap[row][col];
    levelPowerUpMap[row][col] = 0;

    return powerUpType;
}

// Verifica se todos os tijolo destrutíveis foram eliminados
int LevelCompleted(Game *game) {

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (game->level[row][col] == '1' ||
                game->level[row][col] == '2' ||
                game->level[row][col] == '3') {
                return 0;
            }
        }
    }

    return 1;
}