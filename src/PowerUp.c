#include "..\include\PowerUp.h"
#include "..\include\game.h"
#include <raylib.h>
#include <time.h>
#include <stdlib.h>


#define MAX_POWERUPS 10
#define POWERUP_SIZE 20
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define POWERUP_SPEED 3
#define POWERUP_DURATION 10000 // Duração do power-up em milissegundos
#define PLATAFOROMAY 550
#define POWERUP_CHANCE 6 // Chance de spawnar um power-up (em milésimos)

void DesativaPowerUp(PowerUp *powerUp, Game *game);
void SpawnPowerUp(PowerUp powerUps[MAX_POWERUPS], int Row, int Col);
void DrawPowerUp(PowerUp powerUps[MAX_POWERUPS]);
void UpdatePowerUp(PowerUp *powerUp, Game *game);
void CheckPowerUpCollision(PowerUp powerUps[MAX_POWERUPS], Game *game);
void AtivaPowerUp(PowerUp *powerUp, Game *game);
int SorteioPowerUps();


int SorteioPowerUps() {
    srand(time(NULL));
    int randomPowerUp = 1 + rand() % 10000; // Gera um número aleatório entre 1 e 10000
        return randomPowerUp;
 // nenhum power-up é gerado (10000 - POWERUP_CHANCE)% das vezes
}

void CheckPowerUpCollision(PowerUp powerUps[MAX_POWERUPS], Game *game) {
    // Verificar colisão entre o jogador e os power-ups
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerUps[i].active) {
            // Verificar colisão com a plataforma
            if (powerUps[i].y + POWERUP_SIZE >= PLATAFOROMAY && 
                powerUps[i].y <= PLATAFOROMAY + game->paddleHeight &&
                powerUps[i].x + POWERUP_SIZE >= game->paddleX && 
                powerUps[i].x <= game->paddleX + game->paddleWidth) {
                
                // Ativar o power-up correspondente
                powerUps[i].active = 0; // Desativar o power-up após a colisão
                AtivaPowerUp(&powerUps[i], game);
                } 
            }
        else if (powerUps[i].y > SCREEN_HEIGHT) {
                // Desativar o power-up se ele sair da tela
                powerUps[i].active = 0;
            }
        }
    }

void DrawPowerUp(PowerUp powerUps[MAX_POWERUPS]) {
    // Desenhar o power-up na tela
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerUps[i].active) {
            switch (powerUps[i].type)
            {
            case 1:
                DrawCircle(powerUps[i].x, powerUps[i].y, POWERUP_SIZE / 2, BLUE);
                break;
            case 2:
                DrawCircle(powerUps[i].x, powerUps[i].y, POWERUP_SIZE / 2, GREEN);
                break;
            case 3:
                DrawCircle(powerUps[i].x, powerUps[i].y, POWERUP_SIZE / 2, YELLOW);
                break;
            case 4:
                DrawCircle(powerUps[i].x, powerUps[i].y, POWERUP_SIZE / 2, ORANGE);
                break;
            case 5:
                DrawCircle(powerUps[i].x, powerUps[i].y, POWERUP_SIZE / 2, PURPLE);
                break;
            case 6:
                DrawCircle(powerUps[i].x, powerUps[i].y, POWERUP_SIZE / 2, PINK);
                break;
            default:
                break;
            }
        }
    }
}

void UpdatePowerUp(PowerUp *powerUp, Game *game) {
    // Atualizar a posição do power-up
    powerUp->y += POWERUP_SPEED; // Move o power-up para baixo
        switch (powerUp->type){
            case 1:
            case 2:
            case 4:if (powerUp->duracao <= 0) {
                DesativaPowerUp(powerUp, game);
                }
                break;
            default:
                break;
            }
            // Desativar o power-up após a duração
}

void AtivaPowerUp(PowerUp *powerUp, Game *game) {
    // Ativar o efeito do power-up com base no tipo
    switch (powerUp->type) {
        case 1:
            // Exemplo: Aumentar a velocidade da bola
            game->ballSpeedX *= 1.5;
            game->ballSpeedY *= 1.5;
            powerUp->duracao = POWERUP_DURATION; // Definir a duração do power-up
            break;
        case 2:
            // Exemplo: Aumentar o tamanho da plataforma
            game->paddleWidth *= 1.5;
            powerUp->duracao = POWERUP_DURATION; // Definir a duração do power-up
            break;
        case 3:
            // Exemplo: Vidas extras
            game->lives += 1;
            break;
        case 4:
            // Exemplo: Bola de fogo (destroi blocos com dobro de eficiencia)
                powerUp->duracao = POWERUP_DURATION; // Definir a duração do power-up
            //blocos com rigidez 2 ou 3 são destruídos com um hit

            break;
        case 5:
            
            break;
        case 6:
            // Exemplo: Multiplicador de pontos (pontos ganhos são multiplicados por
            game->score*=1.2;
            break;
        default:
            break;
    }
}
void DesativaPowerUp(PowerUp *powerUp, Game *game) {
    // Desativar o efeito do power-up após a duração
        switch (powerUp->type) {
            case 1:
                // Reverter a velocidade da bola ao normal
                game->ballSpeedX = 5;
                game->ballSpeedY = -5;
                powerUp->active = 0; // Desativar o power-up
                break;
            case 2:
                // Reverter o tamanho da plataforma ao normal
                game->paddleWidth = 100;
                powerUp->active = 0; // Desativar o power-up
                break;
            case 4:
                // Reverter a bola de fogo ao normal
             //blocos com rigidez 2 ou 3 voltam a precisar de dois hits para serem destruídos
                break;
            default:
                break;
        }
}

void SpawnPowerUp(PowerUp powerUps[MAX_POWERUPS], int Row, int Col) {
    // Gerar um novo power-up onde o bloco é quebrado (essa função deve ser chamada dentro da lógica de quebra de blocos)
    int sorteio = SorteioPowerUps();
    if (sorteio > 0 && sorteio <= POWERUP_CHANCE) { // Verificar se foi sorteado um power up
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (!powerUps[i].active) {
                powerUps[i].x = Col * (SCREEN_WIDTH / COLS) + (SCREEN_WIDTH / COLS) / 2; // Centraliza o power-up no bloco
                powerUps[i].y = Row * (SCREEN_HEIGHT / ROWS) + (SCREEN_HEIGHT / ROWS) / 2; // Centraliza o power-up no bloco
                powerUps[i].type = sorteio; // Supondo que existam 6 tipos de power-ups
                powerUps[i].active = 1;
                powerUps[i].spawnTime = GetTime(); // Armazena o tempo de spawn
                break;
            }
        }
    }
}