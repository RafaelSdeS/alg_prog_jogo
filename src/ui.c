 /*
  * Sistema de interface do usuário (UI) e renderização de telas do jogo.
  *
  * Responsável por:
  * - Renderização do HUD durante o gameplay (vidas, score, nível)
  * - Desenho dos elementos principais do jogo (level, bolas, paddle, power-ups)
  * - Exibição de telas de estado (Gameover, Vitória, Entrada de Nome)
  * - Coordenação visual entre sistemas de gameplay e estado do jogo
  */

#include <raylib.h>

#include "ui.h"
#include "game.h"
#include "power_up.h"
#include "ball.h"
#include "ranking.h"

// Renderização principal do jogo (HUD + elementos visuais)
void DrawGame(Game *game, PowerUp powerUps[MAX_POWERUPS]) {

    DrawText(TextFormat("Vidas: %d", game->lives), 20, 10, 20, WHITE);
    DrawText(TextFormat("Score: %d", game->score), 200, 10, 20, WHITE);
    DrawText(TextFormat("Nível: %d", game->currentLevel), 400, 10, 20, WHITE);

    DrawLevel(game->level);

    DrawPowerUp(powerUps);

    for (int i = 0; i < MAX_BALLS; i++) {
        if (game->balls[i].active) {
            DrawSingleBall(&game->balls[i], game);
        }
    }

    DrawRectangle(game->paddleX, game->paddleY, game->paddleWidth, game->paddleHeight, WHITE);
}

// Tela de game over
void DrawGameOverScreen(Game *game) {

    DrawText("GAME OVER", 220, 200, 60, RED);
    DrawText(TextFormat("SCORE FINAL: %d", game->score), 220, 300, 30, WHITE);
    DrawText("Pressione ENTER", 220, 400, 30, GRAY);
}

// Tela de vitória
void DrawWinScreen(Game *game) {

    DrawText("YOU WON!", 220, 200, 60, YELLOW);
    DrawText(TextFormat("SCORE FINAL: %d", game->score), 220, 300, 30, WHITE);
    DrawText("Pressione ENTER", 220, 400, 30, GRAY);
}

// Tela de entrada de nome
void DrawEnterNameScreen(Game *game) {

    DrawText("NOVO TOP SCORE!", 180, 150, 40, YELLOW);
    DrawText("Digite seu nome:", 180, 250, 30, WHITE);
    DrawRectangleLines(180, 300, 400, 50, WHITE);
    DrawText(game->playerName, 190, 310, 30, WHITE);
    DrawText("ENTER para confirmar", 180, 400, 25, GRAY);
}

// Desenhar tela de ranking
void DrawRanking() {
    RankingEntry ranking[TOP_SCORES];

    LoadRanking(ranking);

    DrawText("TOP 5 SCORES", 220, 80, 40, WHITE);

    for (int i = 0; i < TOP_SCORES; i++) {

        // Lógica para exibir o nome ou apenas uma linha tracejada caso o arquivo de ranking não esteja completo
        DrawText(
            TextFormat(
                "%d. %s - %d",
                i + 1,
                ranking[i].name[0] != '\0' ? ranking[i].name : "---",
                ranking[i].score
            ), 180, 180 + i * 50, 30, WHITE);
    }

    DrawText("ESC - Voltar", 250, 500, 25, GRAY);
}