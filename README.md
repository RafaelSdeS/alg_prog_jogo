# Trabalho Final - Algoritmos e Programação (INF01202)

Projeto final da disciplina INF01202 - Algoritmos e Programação da UFRGS.

O jogo é inspirado no jogo Breakout, utilizando a biblioteca Raylib em C.

## Compilação

Da root do projeto, rode o seguinte comando para criar o arquivo executável:

```bash
gcc src/*.c -Iinclude -o bin/jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Execução

Da root do projeto, rode o seguinte comando:

```bash
./bin/jogo
```

## Requisitos

- Linguagem C
- GCC
- Raylib