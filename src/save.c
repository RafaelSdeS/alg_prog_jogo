/*
 * Sistema de persistência de saves e seleção de arquivos.
 *
 * Responsável por:
 * - Listagem de arquivos de save no diretório (RefreshSaveList)
 * - Serialização e gravação do estado completo do jogo em disco (SaveGame)
 * - Deserialização e carregamento do estado do jogo (LoadGame)
 * - Interface de seleção de saves com navegação e paginação
 * - Renderização da tela de seleção de saves (DrawSaveSelection)
 *
 * Este módulo encapsula toda a lógica de persistência do estado do jogo,
 * incluindo interação com sistema de arquivos e UI associada.
 *
 * O estado salvo é um dump direto da struct Game, permitindo restauração
 * completa do jogo em um ponto anterior.
 */

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

    // Reseta estado da lista de saves
    saveCount = 0;
    selectedSave = 0;
    firstVisibleSave = 0;

    // Abre o diretório onde os arquivos de save estão armazenados
    DIR *dir = opendir("saves");

    // Se o diretório não existir ou não puder ser acessado, encerra a função
    if (dir == NULL) {
        return;
    }

    // Estrutura usada para representar cada entrada (arquivo/pasta) do diretório
    struct dirent *entry;

    // Percorre todas as entradas do diretório
    while ((entry = readdir(dir)) != NULL) {

        // Filtra apenas arquivos de save com extensão ".dat"
        if (strstr(entry->d_name, ".dat") != NULL) {

            // Copia o nome do arquivo para o array de saves
            strcpy(saveFiles[saveCount], entry->d_name);

            // Incrementa contador de saves encontrados
            saveCount++;

            // Evita ultrapassar o limite máximo de saves suportados
            if (saveCount >= MAX_SAVES) {
                break;
            }
        }
    }

    // Fecha o diretório após a leitura
    closedir(dir);

    // Ordena os saves em ordem alfabética decrescente para ordenar de acordo com a data mais recente
    for (int i = 0; i < saveCount - 1; i++) {
        for (int j = i + 1; j < saveCount; j++) {

            // Compara os nomes dos arquivos
            if (strcmp(saveFiles[i], saveFiles[j]) < 0) {

                // Buffer temporário para troca de strings
                char temp[256];

                // Troca dos nomes dos arquivos
                strcpy(temp, saveFiles[i]);
                strcpy(saveFiles[i], saveFiles[j]);
                strcpy(saveFiles[j], temp);
            }
        }
    }
}

// Salvar jogo
void SaveGame(Game *game) {

    // Estruturas usadas para gerar timestamp do arquivo de save
    //https://en.cppreference.com/c/chrono/time_t
    time_t now;

    // https://en.cppreference.com/c/chrono/tm
    struct tm *t;

    // Buffer para nome do arquivo gerado
    char filename[128];

    // Caminho completo do arquivo dentro da pasta de saves
    char path[300];

    FILE *file;

    // Tempo atual do sistema
    now = time(NULL);

    // Converte o tempo para estrutura local (ano, mês, dia, hora, etc.)
    t = localtime(&now);

    // Gera nome único baseado em data e hora
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

    // Monta caminho completo do arquivo dentro da pasta "saves"
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

    // Lógica para colocar a cor no save selecionado
    for (int i = firstVisibleSave; i < lastVisibleSave; i++) {

        Color color;
        if (i == selectedSave) {
            color = YELLOW;
        } else {
            color = WHITE;
        }

        DrawText(saveFiles[i],
                120,
                180 + (i - firstVisibleSave) * 50,
                30,
                color);
    }

    // Indicador de saves acima
    if (firstVisibleSave > 0) {
        DrawText("^", 720, 130, 30, GRAY);
    }

    // Indicador de saves abaixo
    if (firstVisibleSave + VISIBLE_SAVES < saveCount) {
        DrawText("v", 720, 500, 30, GRAY);
    }

    // Se não encontrar saves
    if (saveCount == 0) {
        DrawText("NO SAVES FOUND", 250, 250, 30, RED);
    }

    DrawText("ENTER - Carregar", 20, 560, 20, GRAY);
    DrawText("ESC - Voltar", 220, 560, 20, GRAY);
}