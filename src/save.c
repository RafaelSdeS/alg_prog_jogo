#include <time.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <raylib.h>

#include <stdio.h>

#include "game.h"
#include "save.h"
#include "game_state.h"

char saveFiles[MAX_SAVES][256];

int saveCount = 0;
int selectedSave = 0;

// Atualizar lista de saves
void RefreshSaveList() {

    saveCount = 0;
    selectedSave = 0;

    DIR *dir = opendir("saves");

    if (dir == NULL) {
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {

        if (strstr(entry->d_name, ".dat") != NULL) {

            strcpy(saveFiles[saveCount], entry->d_name);
            saveCount++;

            if (saveCount >= MAX_SAVES) {
                break;
            }
        }
    }

    closedir(dir);
}

// Salvar jogo
void SaveGame(Game *game) {

    // GERAR NOME COM DATA/HORA

    time_t now = time(NULL);

    struct tm *t = localtime(&now);

    char filename[128];

    sprintf(
        filename,
        "save_%04d-%02d-%02d_%02d-%02d-%02d.dat",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec
    );

    char path[300];

    sprintf(path, "saves/%s", filename);

    FILE *file = fopen(path, "wb");

    if (file == NULL) {
        return;
    }

    fwrite(
        game,
        sizeof(Game),
        1,
        file
    );

    fclose(file);
}

// Carregar jogo
int LoadGame(Game *game, const char *filename) {
    printf("Entrando no LoadGame\n");

    char path[300];

    sprintf(path, "saves/%s", filename);

    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        return 0;
    }

    fread(
        game,
        sizeof(Game),
        1,
        file
    );

    fclose(file);
    printf("Logo antes do retorno no LoadGame\n");

    return 1;
}

// Atualizar tela de saves
void UpdateSaveSelection(Game *game, GameState *currentScreen) {

    if (IsKeyPressed(KEY_DOWN)) {
        selectedSave++;
    }

    if (IsKeyPressed(KEY_UP)) {
        selectedSave--;
    }

    if (selectedSave < 0) {
        selectedSave = saveCount - 1;
    }

    if (selectedSave >= saveCount) {
        selectedSave = 0;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        printf("ENTER detectado na tela de save\n");
    }

    if (IsKeyPressed(KEY_ENTER) &&
        saveCount > 0
    ) {

        if (
            LoadGame(
                game,
                saveFiles[selectedSave]
            )
        ) {

            *currentScreen = GAME;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        *currentScreen = MENU;
    }
}

// Desenhar tela de saves
void DrawSaveSelection() {

    DrawText(
        "SELECT SAVE",
        250,
        80,
        40,
        WHITE
    );

    for (int i = 0; i < saveCount; i++) {

        DrawText(
            saveFiles[i],
            250,
            180 + i * 50,
            30,
            i == selectedSave ? YELLOW : WHITE
        );
    }

    if (saveCount == 0) {

        DrawText(
            "NO SAVES FOUND",
            250,
            250,
            30,
            RED
        );
    }
}