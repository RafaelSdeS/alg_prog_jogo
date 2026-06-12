# Brick Breaker

Projeto desenvolvido para a disciplina **INF01202 – Algoritmos e Programação** da **Universidade Federal do Rio Grande do Sul (UFRGS)**.

O Brick Breaker é um jogo arcade inspirado no clássico Breakout. O jogador controla uma plataforma localizada na parte inferior da tela e deve rebater uma bola para destruir todos os tijolos presentes em cada fase. O objetivo é completar todas as fases sem perder todas as vidas disponíveis.

O projeto foi desenvolvido em linguagem C utilizando a biblioteca gráfica Raylib e aplica conceitos fundamentais de programação, como modularização, estruturas de dados, manipulação de arquivos, controle de estados e detecção de colisões.

## Funcionalidades

- Movimentação da plataforma por teclado;
- Sistema de colisões entre bola, plataforma, paredes e tijolos;
- Sistema de vidas;
- Sistema de pontuação;
- Múltiplas fases carregadas a partir de arquivos externos;
- Progressão automática entre fases;
- Sistema de salvamento e carregamento de partidas;
- Sistema de ranking com armazenamento das melhores pontuações;
- Interface gráfica desenvolvida com a biblioteca Raylib.

## Estrutura do Projeto

- bin/ : executável gerado após a compilação
- fases/ : arquivos contendo os mapas das fases
- include/ : arquivos de cabeçalho (.h)
- saves/ : arquivos de jogos salvos
- src/ : código-fonte da aplicação
- ranking.txt : armazenamento do ranking
- README.md
- Doxyfile


## Organização dos Módulos

| Módulo | Responsabilidade |
|---------|------------------|
| `main.c` | Inicialização da aplicação e controle dos estados do jogo |
| `game.c` | Lógica principal da partida, colisões, pontuação e progressão de fases |
| `level.c` | Carregamento e renderização das fases |
| `menu.c` | Interface e navegação dos menus |
| `save.c` | Salvamento e carregamento de partidas |
| `ranking.c` | Gerenciamento e exibição do ranking de pontuações |

## Requisitos

- Linguagem C
- GCC
- Biblioteca Raylib

## Compilação

A partir da raiz do projeto, execute:

```bash
gcc src/*.c -Iinclude -o bin/jogo \
-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Execução

Após a compilação, execute:

```bash
./bin/jogo
```

## Controles

| Tecla | Ação |
|---------|---------|
| ← | Mover a plataforma para a esquerda |
| → | Mover a plataforma para a direita |
| ENTER | Confirmar opções dos menus |
| F5 | Salvar partida |
| ESC | Voltar ao menu |

## Documentação

O projeto possui suporte à geração automática de documentação utilizando o Doxygen.

Para gerar a documentação:

```bash
doxygen Doxyfile
```

Após a geração, a documentação HTML pode ser acessada através do arquivo:

```text
html/index.html
```

## Desenvolvido por

- Rafael Silva de Souza
- Felipe Portinho de Souza

**Universidade Federal do Rio Grande do Sul (UFRGS)**  
**Instituto de Informática**  
**INF01202 – Algoritmos e Programação**