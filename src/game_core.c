/*
 * Núcleo principal de controle do jogo.
 *
 * Responsável por:
 * - Inicialização do estado global do jogo (InitGame)
 * - Loop principal de atualização (UpdateGame)
 * - Coordenação de subsistemas (bolas, power-ups, níveis e colisões)
 * - Controle de progressão entre fases e condição de game over
 * - Gerenciamento de entrada do jogador (movimento da plataforma)
 * - Sincronização de estado global derivado (ball cache)
 * - Persistência de progresso (save manual)
 * - Reset de estado entre níveis (ResetPositions)
 *
 * Este módulo atua como orquestrador central, delegando lógica específica
 * para sistemas especializados (ball, level, power_up, save).
 */

#include <raylib.h>

#include "game_core.h"
#include "game.h"
#include "level.h"
#include "brick.h"
#include "power_up.h"
#include "ball.h"
#include "save.h"
#include "audio.h"

// Tempo do último aumento global de velocidade das bolas
float lastBallSpeedIncreaseTime = 0.0;

// Inicialização de estado básico do jogo
void InitGameState(Game *game) {

    // Limpa nome do jogador
    game->playerName[0] = '\0';

    // Estado inicial da partida
    game->lives = 3;
    game->score = 0;
    game->currentLevel = 1;
    game->fireBallActive = 0;
    game->fireBallEndTime = 0.0;
    game->ballCount = 1;

    // Configuração inicial da bola
    game->ballRadius = INITIAL_BALL_RADIUS;

    // Configuração inicial da plataforma
    game->paddleWidth = INITIAL_PADDLE_WIDTH;
    game->paddleHeight = INITIAL_PADDLE_HEIGHT;
    game->paddleSpeed = INITIAL_PADDLE_SPEED;

    game->paddleX = INITIAL_PADDLE_X;
    game->paddleY = INITIAL_PADDLE_Y;
}

// Inicialização de matriz de power-ups por nível
void InitLevelPowerUps(Game *game) {

    // Limpa mapa auxiliar de power-ups do nível
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            game->levelPowerUps[row][col] = 0;
        }
    }
}

// Inicialização do array de power-ups ativos
// Garantir que não há lixo na memória
void InitPowerUps(PowerUp powerUps[MAX_POWERUPS]) {

    // Inicializa todos os slots como vazios
    for(int i = 0; i < MAX_POWERUPS; i++) {
        powerUps[i].active = 0;
        powerUps[i].collected = 0;
        powerUps[i].duracao = 0;
        powerUps[i].type = 0;
        powerUps[i].x = 0;
        powerUps[i].y = 0;
        powerUps[i].effectStartTime = 0.0;
    }
}

// Reseta sistemas de jogo
void ResetSystems(Game *game) {

    // Reinicia sistema de bolas
    ResetBallSystem(game);

    // Reinicia contador do aumento automático de velocidade
    lastBallSpeedIncreaseTime = GetTime();
}

// Atualiza fireball
void UpdateFireBall(Game *game, float currentTime) {

    // Desativa o efeito quando o tempo terminar
    if (game->fireBallActive && currentTime >= game->fireBallEndTime) {
        game->fireBallActive = 0;
    }
}

// Atualiza velocidade global das bolas
void UpdateBallSpeed(Game *game, float currentTime) {

    // Verifica se chegou o momento de aumentar a dificuldade
    if (currentTime - lastBallSpeedIncreaseTime >= BALL_SPEED_INCREASE_INTERVAL) {
        UpdateBallSpeedIncrease(game);
        lastBallSpeedIncreaseTime = currentTime;
    }
}

// Atualiza movimento da plataforma
void UpdatePaddle(Game *game) {

    // Movimento para direita
    if (IsKeyDown(KEY_RIGHT)) {
        game->paddleX += game->paddleSpeed;
    }

    // Movimento para esquerda
    if (IsKeyDown(KEY_LEFT)) {
        game->paddleX -= game->paddleSpeed;
    }

    // Impede sair da tela pela esquerda
    if (game->paddleX < 0) {
        game->paddleX = 0;
    }

    // Impede sair da tela pela direita
    if (game->paddleX + game->paddleWidth > SCREEN_WIDTH) {
        game->paddleX = SCREEN_WIDTH - game->paddleWidth;
    }
}

// Reseta power-ups ativos
void ResetActivePowerUps(PowerUp powerUps[MAX_POWERUPS]) {

    // Remove todos os power-ups da fase atual
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerUps[i].active = 0;
        powerUps[i].collected = 0;
        powerUps[i].duracao = 0;
        powerUps[i].type = 0;
        powerUps[i].effectStartTime = 0.0;
    }
}

// Inicializa estado completo do jogo
void InitGame(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]) {

    // Inicializa estruturas básicas do jogo
    InitGameState(game);

    // Limpa mapa de power-ups por bloco
    InitLevelPowerUps(game);

    // Inicializa slots de power-ups ativos
    InitPowerUps(powerUps);

    // Reinicia sistemas auxiliares
    ResetSystems(game);

    // Carrega a primeira fase
    LoadCurrentLevel(game, currentScreen);
}

// Loop principal de atualização do jogo
void UpdateGame(Game *game, GameState *currentScreen, PowerUp powerUps[MAX_POWERUPS]) {

    float currentTime = GetTime();

    // Atualiza efeitos temporários
    UpdateFireBall(game, currentTime);

    // Atualiza dificuldade progressiva
    UpdateBallSpeed(game, currentTime);

    // Atualiza todas as bolas ativas
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!game->balls[i].active) {
            continue;
        }

        UpdateSingleBall(game, &game->balls[i], powerUps);
    }

    // Reconta bolas após atualizações
    game->ballCount = CountActiveBalls(game);

    // Jogador perdeu todas as bolas
    if (game->ballCount <= 0) {

        game->lives--;

        // Fim de jogo
        if (game->lives <= 0) {
            PlayGameoverSound();
            *currentScreen = GAMEOVER;
            return;
        }

        // Reinicia uma nova bola
        ResetBallSystem(game);
    }

    // Atualiza power-ups ativos e coletados
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerUps[i].active) {
            UpdatePowerUp(&powerUps[i], game);
        }
    }

    // Atualiza movimentação do jogador
    UpdatePaddle(game);

    // Verifica coleta de power-ups pela plataforma
    CheckPowerUpCollision(powerUps, game);

    // Verifica conclusão da fase
    if (LevelCompleted(game)) {

        // Remove power-ups remanescentes da fase anterior
        ResetActivePowerUps(powerUps);

        // Garante que fireball não seja carregado para a próxima fase
        game->fireBallActive = 0;
        game->fireBallEndTime = 0.0;

        // Reinicia posições básicas
        ResetPositions(game);

        // Avança para próxima fase
        game->currentLevel++;

        // Carrega novo layout
        LoadCurrentLevel(game, currentScreen);
    }

    // Salvar progresso (F5)
    if (IsKeyPressed(KEY_F5)) {
        SaveGame(game);
    }

    // Atualiza cache da bola principal
    SyncPrimaryBallFields(game);
}

// Reseta posição e estado entre fases
void ResetPositions(Game *game) {

    // Reinicia sistema de bolas
    ResetBallSystem(game);

    // Restaura configuração padrão da plataforma
    game->paddleX = INITIAL_PADDLE_X;
    game->paddleY = INITIAL_PADDLE_Y;
    game->paddleWidth = INITIAL_PADDLE_WIDTH;
    game->paddleHeight = INITIAL_PADDLE_HEIGHT;
    game->paddleSpeed = INITIAL_PADDLE_SPEED;
}