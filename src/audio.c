/*
 * Sistema de áudio.
 *
 * Responsável por:
 * - Inicialização do sistema de áudio da Raylib
 * - Carregamento e controle da música principal
 * - Carregamento e reprodução de efeitos sonoros
 * - Atualização contínua da música durante o loop do jogo
 * - Liberação de recursos de áudio ao encerrar o programa
 */

#include <raylib.h>
#include "audio.h"

// Música do jogo e efeitos sonoros
Music mainTheme;
Sound brickBreakSound;
Sound gameOverSound;
Sound victorySound;

// Inicializa áudio e carrega música
void InitAudioSystem(void) {

    InitAudioDevice();

    // Credits to wyver9 (https://opengameart.org/content/arcade-level-tracks-8-bitchiptune)
    mainTheme = LoadMusicStream("assets/sounds/main_theme.wav");

    // Corrugated Iron Crash.wav by ReyDros -- https://freesound.org/s/327054/ -- License: Attribution NonCommercial 4.0
    brickBreakSound = LoadSound("assets/sounds/brick_break.wav");

    // Credits to 0new4y (https://opengameart.org/content/game-over-trumpet-sfx)
    gameOverSound = LoadSound("assets/sounds/gameover.wav");

    // Credits to spuispuin (https://opengameart.org/content/won-orchestral-winning-jingle)
    victorySound = LoadSound("assets/sounds/victory.wav");

    SetMusicVolume(mainTheme, 0.5f);

    PlayMusicStream(mainTheme);
}

// Atualiza stream da música (chamar todo frame)
void UpdateAudioSystem(void) {

    UpdateMusicStream(mainTheme);

    // garante loop contínuo
    if (!IsMusicStreamPlaying(mainTheme)) {
        PlayMusicStream(mainTheme);
    }
}

// Libera memória de áudio
void UnloadAudioSystem(void) {

    UnloadMusicStream(mainTheme);
    UnloadSound(brickBreakSound);
    CloseAudioDevice();
}

// Som do tijolo quebrando
void PlayBrickBreakSound(void) {
    PlaySound(brickBreakSound);
}

// Som de gameover
void PlayGameoverSound(void){
    PlaySound(gameOverSound);
}

// Som de vitória
void PlayVictorySound(void){
    PlaySound(victorySound);
}