/*
 * Sistema de powerups do jogo.
 *
 * Responsável por:
 * - Carregamento e gerenciamento de texturas dos powerups
 * - Definição de comportamento e regras de cada tipo de powerup
 * - Spawn de power-ups a partir de blocos destruídos
 * - Detecção de colisão entre power-ups e o jogador
 * - Atualização de estado (movimento, duração e expiração de efeitos)
 * - Aplicação e reversão de efeitos no estado global do jogo
 * - Renderização visual dos powerups na tela
 */

#include <stdlib.h>
#include <raylib.h>

#include "power_up.h"
#include "game.h"
#include "config.h"

// Inicialização da struct global das texturas (carregar campos inicialmente com 0 para verificação)
PowerUpTextures gPowerUpTextures = {0};

// Imagens para os powerups
char *powerUpTexturePaths[MAX_POWERUP_TYPES + 1] = {
    NULL, // Alinhar as texturas com o array dos tipos
    "assets/images/powerup-speed.png",
    "assets/images/powerup-size.png",
    "assets/images/powerup-life.png",
    "assets/images/powerup-fire.png",
    "assets/images/powerup-multiball.png",
    "assets/images/powerup-score.png"
};

// Carrega texturas dos power-ups
void LoadPowerUpTextures(void) {
    // Loop para acessar todos os tipos de powerups
    for (int type = 1; type <= MAX_POWERUP_TYPES; type++) {
        //Acesso apenas se o arquivo existe
        if (FileExists(powerUpTexturePaths[type])) {
            gPowerUpTextures.textures[type] = LoadTexture(powerUpTexturePaths[type]);
        } 
    }
}

// Libera memória das texturas dos power-ups
void UnloadPowerUpTextures(void) {
    // Loop entre tipos de powerups
    for (int type = 1; type <= MAX_POWERUP_TYPES; type++) {
        // Se o powerup existe
        if (gPowerUpTextures.textures[type].id != 0) {
            // Descarrega a textura
            UnloadTexture(gPowerUpTextures.textures[type]);
            gPowerUpTextures.textures[type] = (Texture2D){0};
        }
    }
}

// Retorna a textura associada ao tipo do power-up
Texture2D GetPowerUpTexture(PowerUpType type) {

    // Validação de segurança:
    // verifica se o tipo está dentro do intervalo válido do enum
    // (evita acessar índices inválidos do array de texturas)
    if (type < SPEED_POWERUP || type > SCORE_POWERUP) {
        return (Texture2D){0}; // Caso inválido, retorne nulo (id = 0)
    }

    // Retorna a textura carregada correspondente ao tipo
    // O índice do array é o próprio valor do enum (mapeamento direto)
    return gPowerUpTextures.textures[type];
}

// Definir se o powerup tem duração
int PowerUpHasDuration(int type) {
    return type == SPEED_POWERUP || type == FIREBALL_POWERUP || type == SIZE_POWERUP;
}

// Cor de fundo de cada powerup
Color GetPowerUpBackgroundColor(int type) {
    switch (type) {
        case SPEED_POWERUP: return SKYBLUE;
        case SIZE_POWERUP: return LIME;
        case EXTRA_LIFE_POWERUP: return YELLOW;
        case FIREBALL_POWERUP: return ORANGE;
        case MULTIBALL_POWERUP: return VIOLET;
        case SCORE_POWERUP: return PINK;
        default: return WHITE;
    }
}

// Cria um powerup na posição do bloco destruído
void SpawnPowerUp(PowerUp powerUps[MAX_POWERUPS], int row, int col, PowerUpType type) {
    for (int i = 0; i < MAX_POWERUPS; i++) {

        // Procura uma posição livre no vetor de power-ups
        if (powerUps[i].active) {
            continue;
        }

        // Posiciona o powerup no centro do bloco destruído
        powerUps[i].x = col * (SCREEN_WIDTH / COLS) + (SCREEN_WIDTH / COLS) / 2;
        powerUps[i].y = row * (SCREEN_HEIGHT / ROWS) + (SCREEN_HEIGHT / ROWS) / 2;

        // Usa o tipo informado ou sorteia um tipo aleatório válido
        if (type >= SPEED_POWERUP && type <= SCORE_POWERUP) {
            powerUps[i].type = type;
        }

        // Carrega a textura correspondente ao tipo do power-up
        powerUps[i].texture = GetPowerUpTexture(powerUps[i].type);

        // Inicializa estado do power-up
        powerUps[i].active = 1;
        powerUps[i].collected = 0;
        powerUps[i].duracao = 0;
        powerUps[i].effectStartTime = 0.0;

        // Encerra a busca após criar o power-up
        break;
    }
}

// Verifica colisão entre power-up e paddle
void CheckPowerUpCollision(PowerUp powerUps[MAX_POWERUPS], Game *game) {
    for (int i = 0; i < MAX_POWERUPS; i++) {

        // Ignora power-ups inativos ou já coletados
        if (!powerUps[i].active || powerUps[i].collected) {
            continue;
        }

        // Checagem de colisão
        if (powerUps[i].y + POWERUP_SIZE >= PLATAFOROMAY &&
            powerUps[i].y <= PLATAFOROMAY + game->paddleHeight &&
            powerUps[i].x + POWERUP_SIZE >= game->paddleX &&
            powerUps[i].x <= game->paddleX + game->paddleWidth) {

            // Aplica o efeito do power-up no jogo
            AtivaPowerUp(&powerUps[i], game);

            // Se o power-up tiver duração, ele fica coletado
            // e passa a ser controlado pelo sistema de tempo
            if (PowerUpHasDuration(powerUps[i].type)) {
                powerUps[i].collected = 1;
                powerUps[i].effectStartTime = GetTime();
            } else {
                // Se não tem duração, é removido imediatamente após ativação
                powerUps[i].active = 0;
                powerUps[i].collected = 0;
            }
        }

        // Remove power-up se sair da tela (não foi coletado)
        if (powerUps[i].y > SCREEN_HEIGHT) {
            powerUps[i].active = 0;
            powerUps[i].collected = 0;
        }
    }
}

// Desenha power-ups na tela
void DrawPowerUp(PowerUp powerUps[MAX_POWERUPS]) {
    for (int i = 0; i < MAX_POWERUPS; i++) {

        // Ignora slots vazios ou powerups já coletados
        if (!powerUps[i].active || powerUps[i].collected) {
            continue;
        }

        // Recupera a textura correspondente ao tipo do power-up
        Texture2D texture = GetPowerUpTexture(powerUps[i].type);

        // Define cor de fundo usada como indicação visual do tipo
        Color bg = GetPowerUpBackgroundColor(powerUps[i].type);

        // Desenha círculo de fundo do power-up
        // Serve como base visual mesmo quando não há textura carregada
        DrawCircle(powerUps[i].x, powerUps[i].y, POWERUP_SIZE / 2 + 4, bg);

        // Desenha contorno do power-up para destacar na tela
        DrawCircleLines(powerUps[i].x, powerUps[i].y, POWERUP_SIZE / 2 + 4, WHITE);

        // Se existir textura válida, desenha por cima do círculo
        if (texture.id != 0) {

            // Região da textura original (imagem completa)
            Rectangle source = {
                0.0f,
                0.0f,
                (float)texture.width,
                (float)texture.height
            };

            // Região onde a textura será desenhada na tela
            // Ajustada para centralizar dentro do círculo do power-up
            Rectangle dest = {
                powerUps[i].x - POWERUP_SIZE / 2 + 2,
                powerUps[i].y - POWERUP_SIZE / 2 + 2,
                POWERUP_SIZE - 4,
                POWERUP_SIZE - 4
            };

            // Desenha a textura escalada dentro do círculo
            DrawTexturePro(texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
        }
    }
}

// Atualiza estado do power-up (movimento e duração)
void UpdatePowerUp(PowerUp *powerUp, Game *game) {

    // Se o power-up não estiver ativo, não faz nada
    if (!powerUp->active) {
        return;
    }

    // Caso o power-up já tenha sido coletado
    if (powerUp->collected) {

        // Power-ups sem duração são removidos imediatamente após coleta
        // (efeito é aplicado uma única vez no momento da colisão)
        if (powerUp->type == EXTRA_LIFE_POWERUP ||
            powerUp->type == MULTIBALL_POWERUP ||
            powerUp->type == SCORE_POWERUP) {

            powerUp->active = 0;
            powerUp->collected = 0;
            return;
        }

        // Para power-ups com duração, verifica se o tempo de efeito expirou
        if (GetTime() - powerUp->effectStartTime >= (double)POWERUP_DURATION / 1000.0) {
            DesativaPowerUp(powerUp, game);
        }

        return;
    }

    // Movimento vertical do power-up (queda na tela)
    powerUp->y += POWERUP_SPEED;

    // Remove o power-up se ele sair da tela sem ser coletado
    if (powerUp->y > SCREEN_HEIGHT) {
        powerUp->active = 0;
        powerUp->collected = 0;
    }
}

// Ativa/Inicializa os powerups
void AtivaPowerUp(PowerUp *powerUp, Game *game) {
    switch (powerUp->type) {

        // Aumenta velocidade da bola
        case SPEED_POWERUP:
            ApplyBallSpeedMultiplier(game, 1.5f);
            game->paddleSpeed = INITIAL_PADDLE_SPEED * 1.5f;
            powerUp->duracao = POWERUP_DURATION;
            break;

        // Aumenta tamanho da plataforma
        case SIZE_POWERUP:
            game->paddleWidth = INITIAL_PADDLE_WIDTH * 2.0f;
            powerUp->duracao = POWERUP_DURATION;
            break;

        // Vida extra
        case EXTRA_LIFE_POWERUP:
            game->lives += 1;
            break;

        // Modo bola de fogo
        case FIREBALL_POWERUP:
            game->fireBallActive = 1;
            game->fireBallEndTime = GetTime() + (float)POWERUP_DURATION / 1000.0;
            powerUp->duracao = POWERUP_DURATION;
            break;

        // Multibola
        case MULTIBALL_POWERUP:
            ActivateMultiBall(game);
            break;

        // Multiplicador de score
        case SCORE_POWERUP:
            game->score = (int)(game->score * 1.2f);
            break;

        default:
            break;
    }
}

// Desativa efeito dos powerups (que têm tempo)
void DesativaPowerUp(PowerUp *powerUp, Game *game) {

    // Identifica qual efeito deve ser revertido com base no tipo do power-up
    switch (powerUp->type) {

        // Remove o aumento de velocidade da bola
        case SPEED_POWERUP:
            ApplyBallSpeedMultiplier(game, 1.0f / 1.5f);
            game->paddleSpeed = INITIAL_PADDLE_SPEED;
            break;

        // Desativa o modo bola de fogo
        case FIREBALL_POWERUP:
            game->fireBallActive = 0;
            game->fireBallEndTime = 0.0;
            break;

        // Restaura o tamanho original da plataforma
        case SIZE_POWERUP:
            game->paddleWidth = INITIAL_PADDLE_WIDTH;
            break;

        // Caso o tipo não tenha efeito reversível ou não seja reconhecido
        default:
            break;
    }

    // Remove completamente o power-up do jogo após o fim do efeito
    powerUp->active = 0;
    powerUp->collected = 0;
}