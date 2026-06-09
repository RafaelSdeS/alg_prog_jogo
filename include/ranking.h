#ifndef RANKING_H
#define RANKING_H

#define TOP_SCORES 5
#define NAME_SIZE 50

// Struct para salvar o ranking no arquivo
typedef struct {
    char name[NAME_SIZE];
    int score;

} RankingEntry;

// Carregar ranking do arquivo
void LoadRanking(RankingEntry ranking[]);

// Salvar ranking no arquivo
void SaveRanking(RankingEntry ranking[]);

// Inserir nova pontuação
void UpdateRanking(char *name, int score);

// Verifica se a pontuação entra no Top 5
int IsTopScore(int score);

// Desenhar tela de ranking
void DrawRanking();

#endif