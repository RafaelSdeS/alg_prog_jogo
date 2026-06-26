#include <raylib.h>

#ifndef POWER_UP_H
#define POWER_UP_H

#include "config.h"

// Enum do tipo de powerups
typedef enum {
    SPEED_POWERUP = 1,
    SIZE_POWERUP = 2,
    EXTRA_LIFE_POWERUP = 3,
    FIREBALL_POWERUP = 4,
    MULTIBALL_POWERUP = 5,
    SCORE_POWERUP = 6
} PowerUpType;

// Estrutura que armazena todas as texturas dos power-ups carregados. Docs usados:
// https://www.raylib.com/examples/textures/loader.html?name=textures_to_image
// https://docs.rs/raylib/latest/raylib/core/texture/struct.Texture2D.html
typedef struct PowerUpTextures {
    Texture2D textures[MAX_POWERUP_TYPES + 1];
} PowerUpTextures;

// Estrutura que representa um power-up individual no jogo
typedef struct PowerUp {
    float x, y;                 // Posição na tela
    PowerUpType type;           // Tipo do power-up (define efeito)
    int active;                 // Se está ativo no jogo (spawnado)
    int collected;              // Se já foi coletado pelo jogador
    float duracao;              // Duração do efeito (quando aplicável)
    double effectStartTime;     // Tempo em que o efeito foi ativado
    Texture2D texture;          // Textura específica do power-up
} PowerUp;

// Forward declaration da struct Game (evita dependência circular de header)
typedef struct Game Game;

// Instância global das texturas dos power-ups
// Fica acessível em todo o sistema de power-ups
// https://www.geeksforgeeks.org/c/understanding-extern-keyword-in-c/
// https://stackoverflow.com/questions/496448/how-to-correctly-use-the-extern-keyword-in-c
extern PowerUpTextures gPowerUpTextures;

// Carrega todas as texturas dos power-ups
void LoadPowerUpTextures(void);

// Libera memória das texturas carregadas
void UnloadPowerUpTextures(void);

// Retorna a textura associada ao tipo de power-up
Texture2D GetPowerUpTexture(PowerUpType type);

// Desativa um power-up e aplica lógica de remoção de efeito
void DesativaPowerUp(PowerUp *powerUp, Game *game);

// Cria um power-up na posição de um bloco destruído
void SpawnPowerUp(PowerUp powerUps[MAX_POWERUPS], int Row, int Col, PowerUpType type);

// Desenha todos os power-ups ativos na tela
void DrawPowerUp(PowerUp powerUps[MAX_POWERUPS]);

// Atualiza estado individual de um power-up (movimento/duração)
void UpdatePowerUp(PowerUp *powerUp, Game *game);

// Verifica colisão entre power-ups e o jogador (paddle)
void CheckPowerUpCollision(PowerUp powerUps[MAX_POWERUPS], Game *game);

// Ativa o efeito de um power-up ao ser coletado
void AtivaPowerUp(PowerUp *powerUp, Game *game);

#endif