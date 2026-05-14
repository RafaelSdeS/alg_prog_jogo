#include "menu.h"
#include <raylib.h>

void DrawMenu() {
    DrawText("MENU", 300, 100, 40, WHITE);
    DrawText("Novo Jogo", 300, 200, 40, WHITE);
    DrawText("Carregar Save", 300, 300, 40, WHITE);
    DrawText("Ranking", 300, 400, 40, WHITE);
    DrawText("Sair", 300, 500, 40, WHITE);

}