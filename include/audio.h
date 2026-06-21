#ifndef AUDIO_H
#define AUDIO_H

#include <raylib.h>

// Inicializa sistema de áudio
void InitAudioSystem(void);

// Atualiza música (obrigatório no loop)
void UpdateAudioSystem(void);

// Libera recursos de áudio
void UnloadAudioSystem(void);

// Som do tijolo quebrando
void PlayBrickBreakSound(void);

// Som de gameover
void PlayGameoverSound(void);

// Som de vitória
void PlayVictorySound(void);

#endif