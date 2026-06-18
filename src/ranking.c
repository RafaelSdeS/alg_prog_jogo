#include <stdio.h>
#include <string.h>
#include <raylib.h>

#include "..\include\ranking.h"

// Carregar ranking do arquivo
void LoadRanking(RankingEntry ranking[]) {

    int i = 0;
    FILE *file = fopen("ranking.txt", "r");

    // Caso haja erro abrindo o arquivo, igualar o vetor a valores nulos para evitar problemas em outras partes do código
    if (file == NULL) {
        for (int i = 0; i < TOP_SCORES; i++) {
            ranking[i].name[0] = '\0';
            ranking[i].score = 0;
        }

        return;
    }


    // Lê uma linha no formato "nome;score" e verifica se os 2 campos foram carregados
    while (i < TOP_SCORES && fscanf(file,"%49[^;];%d\n", ranking[i].name, &ranking[i].score) == 2) {
        i++;
    }

    while (i < TOP_SCORES) {
        ranking[i].name[0] = '\0';
        ranking[i].score = 0;
        
        i++;
    }

    fclose(file);
}

// Salvar ranking no arquivo
void SaveRanking(RankingEntry ranking[]) {

    FILE *file = fopen("ranking.txt", "w");

    if (file == NULL) {
        return;
    }

    for (int i = 0; i < TOP_SCORES; i++) {
        if (ranking[i].score > 0) {
            fprintf(file, "%s;%d\n", ranking[i].name, ranking[i].score);
        }
    }

    fclose(file);
}

// Inserir uma nova pontuação no ranking
void UpdateRanking(char *name, int score) {
    RankingEntry ranking[TOP_SCORES + 1];

    LoadRanking(ranking);

    strcpy(ranking[TOP_SCORES].name, name);
    ranking[TOP_SCORES].score = score;

    // Ordenar do maior para o menor score
    for (int i = 0; i < TOP_SCORES; i++) {
        for (int j = i + 1; j < TOP_SCORES + 1; j++) {
            if (ranking[j].score > ranking[i].score) {
                RankingEntry temp = ranking[i];

                ranking[i] = ranking[j];
                ranking[j] = temp;
            }
        }
    }

    SaveRanking(ranking);
}

// Verifica se a pontuação entra no top 5 e faz a ordenação
int IsTopScore(int score) {

    RankingEntry ranking[TOP_SCORES];

    LoadRanking(ranking);

    // Se ainda houver espaço vazio no ranking
    for (int i = 0; i < TOP_SCORES; i++) {

        if (ranking[i].score == 0) {
            return 1;
        }
    }

    // Compara com o último colocado
    return score > ranking[TOP_SCORES - 1].score;
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