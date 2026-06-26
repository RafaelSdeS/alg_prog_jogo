// Arquivo com todas as configurações globais do jogo

#ifndef CONFIG_H
#define CONFIG_H

// Tamanho da matriz de tijolos
#define ROWS 15
#define COLS 25

// Configurações globais do jogo
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_POWERUPS 10
#define MAX_POWERUP_TYPES 6
#define MAX_BALLS 3

// Configurações iniciais
#define INITIAL_BALL_SPEED_X 5.0f
#define INITIAL_BALL_SPEED_Y -5.0f
#define INITIAL_BALL_RADIUS 10.0f
#define INITIAL_PADDLE_X 350.0f
#define INITIAL_PADDLE_Y 550.0f
#define INITIAL_PADDLE_WIDTH 100.0f
#define INITIAL_PADDLE_HEIGHT 20.0f
#define INITIAL_PADDLE_SPEED 7.0f

// Configurações dos power-ups
#define POWERUP_SIZE 20
#define POWERUP_SPEED 1
#define POWERUP_DURATION 10000
#define PLATAFOROMAY 550
#define POWERUPS_PER_LEVEL 30

// Incremento global da velocidade da bola
#define BALL_SPEED_INCREASE_INTERVAL 5.0
#define BALL_SPEED_INCREASE_FACTOR 1.02f
#define BALL_SPEED_MAX 12.0f

#endif