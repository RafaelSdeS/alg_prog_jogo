/*
 * Sistema de ranking do jogo.
 *
 * Responsável por:
 * - Leitura e escrita do ranking persistente em arquivo (ranking.txt)
 * - Inserção de novas pontuações com ordenação por score
 * - Verificação de elegibilidade para o top ranking (IsTopScore)
 * - Renderização da tela de ranking na interface
 */

#include <stdio.h>
#include <string.h>
#include <raylib.h>

#include "ranking.h"

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
    // https://www.geeksforgeeks.org/c/scansets-in-c/
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

    // Array temporário com espaço extra para inserir o novo score
    RankingEntry ranking[TOP_SCORES + 1];

    // Carrega ranking atual do arquivo
    LoadRanking(ranking);

    // Insere novo jogador na última posição do array temporário
    strcpy(ranking[TOP_SCORES].name, name);
    ranking[TOP_SCORES].score = score;

    // Ordenar do maior para o menor score
    for (int i = 0; i < TOP_SCORES; i++) {
        for (int j = i + 1; j < TOP_SCORES + 1; j++) {

            // Se encontrar um score maior, troca as posições
            if (ranking[j].score > ranking[i].score) {

                // Swap entre duas entradas do ranking
                RankingEntry temp = ranking[i];

                ranking[i] = ranking[j];
                ranking[j] = temp;
            }
        }
    }

    // Salva novamente apenas os TOP_SCORES melhores
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