#include <time.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <raylib.h>

#include "game.h"
#include "save.h"
#include "game_state.h"

// Número de saves visíveis na tela a qualquer momento (limite da paginação)
#define VISIBLE_SAVES 6

// Tamanho máximo do nome do save
char saveFiles[MAX_SAVES][256];

int saveCount = 0;
int selectedSave = 0;
int firstVisibleSave = 0;

// Atualizar lista de saves
void RefreshSaveList() {

    saveCount = 0;
    selectedSave = 0;
    firstVisibleSave = 0;

    // Abrir diretório de saves
    DIR *dir = opendir("saves");

    if (dir == NULL) {
        return;
    }

    // Arquivo atual que está sendo lido da pasta
    struct dirent *entry;

    // Carregar arquivos de save
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

    // Ordena os saves em ordem decrescente (alfabética) pelo nome do arquivo
    for (int i = 0; i < saveCount - 1; i++) {
        for (int j = i + 1; j < saveCount; j++) {
            if (strcmp(saveFiles[i], saveFiles[j]) < 0) {

                char temp[256];

                strcpy(temp, saveFiles[i]);
                strcpy(saveFiles[i], saveFiles[j]);
                strcpy(saveFiles[j], temp);
            }
        }
    }
}

// Salvar jogo
void SaveGame(Game *game) {

    time_t now;
    struct tm *t;
    char filename[128];
    char path[300];
    FILE *file;

    // Gerar nome com data/hora
    now = time(NULL);
    t = localtime(&now);

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

    // Montar o nome completo do arquivo
    sprintf(path, "saves/%s", filename);

    file = fopen(path, "wb");

    if (file == NULL) {
        return;
    }

    fwrite(game, sizeof(Game), 1, file);

    fclose(file);
}

// Carregar jogo
int LoadGame(Game *game, const char *filename) {

    char path[300];

    // Monta o caminho completo do arquivo de save
    sprintf(path, "saves/%s", filename);

    FILE *file = fopen(path, "rb");

    // Verifica se o arquivo existe e foi aberto corretamente
    if (file == NULL) {
        return 0;
    }

    // Lê o conteúdo do arquivo e carrega no struct Game
    fread(game, sizeof(Game), 1, file);

    fclose(file);

    // Indicar sucesso
    return 1;
}

// Atualizar tela de saves
void UpdateSaveSelection(Game *game, GameState *currentScreen) {

    // Menu de saves que "rotaciona" e com paginação
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

    // Ajustar janela visível da lista
    if (selectedSave < firstVisibleSave) {
        firstVisibleSave = selectedSave;
    }

    if (selectedSave >= firstVisibleSave + VISIBLE_SAVES) {
        firstVisibleSave = selectedSave - VISIBLE_SAVES + 1;
    }

    // Carregar save selecionado
    if (IsKeyPressed(KEY_ENTER) && saveCount > 0) {
        if (LoadGame(game, saveFiles[selectedSave])) {
            *currentScreen = GAME;
        }
    }

    // Voltar ao menu
    if (IsKeyPressed(KEY_ESCAPE)) {
        *currentScreen = MENU;
    }
}

// Desenhar tela de saves
void DrawSaveSelection() {

    DrawText("SELECT SAVE", 250, 80, 40, WHITE);

    int lastVisibleSave = firstVisibleSave + VISIBLE_SAVES;

    if (lastVisibleSave > saveCount) {
        lastVisibleSave = saveCount;
    }

    for (int i = firstVisibleSave; i < lastVisibleSave; i++) {
        DrawText(saveFiles[i], 120, 180 + (i - firstVisibleSave) * 50, 30, i == selectedSave ? YELLOW : WHITE);
    }

    // Indicador de saves acima
    if (firstVisibleSave > 0) {
        DrawText("^", 720, 130, 30, GRAY);
    }

    // Indicador de saves abaixo
    if (firstVisibleSave + VISIBLE_SAVES < saveCount) {
        DrawText("v", 720, 500, 30, GRAY);
    }

    if (saveCount == 0) {
        DrawText("NO SAVES FOUND", 250, 250, 30, RED);
    }

    DrawText("ENTER - Carregar", 20, 560, 20, GRAY);
    DrawText("ESC - Voltar", 220, 560, 20, GRAY);
}