/*
 * Definição dos estados possíveis do jogo.
 *
 * Este header existe para centralizar o controle de fluxo da aplicação
 * e evitar dependência direta de strings ou valores mágicos no código.
 * NÃO DELETE ESTE ARQUIVO SE NÃO VAI TUDO DAR ERRADO (EXPERIÊNCIA PESSOAL)!!!
 * SE NÃO ESTÁ QUEBRADO, NÃO ARRUME!!!
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

// Estados do jogo
typedef enum {
    MENU,
    GAME,
    GAMEOVER,
    WINSCREEN,
    SELECTSAVE,
    RANKING,
    ENTERNAME
} GameState;

#endif