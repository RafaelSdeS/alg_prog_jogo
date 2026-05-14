#include <raylib.h>

int main(void)
{
    // Configurações da janela
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Primeiro teste Raylib");

    // FPS do jogo
    SetTargetFPS(60);

    // Loop principal
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(WHITE);

        DrawText("Raylib funcionando!", 250, 280, 30, DARKBLUE);

        EndDrawing();
    }

    // Finalização
    CloseWindow();

    return 0;
}