# Brick Breaker

Projeto desenvolvido para a disciplina **INF01202 – Algoritmos e Programação** da **Universidade Federal do Rio Grande do Sul (UFRGS)**.

O Brick Breaker é um jogo arcade inspirado no clássico Breakout. O jogador controla uma plataforma localizada na parte inferior da tela e deve rebater uma bola para destruir todos os tijolos presentes em cada fase. O objetivo é completar todas as fases sem perder todas as vidas disponíveis.

O projeto foi desenvolvido em linguagem C utilizando a biblioteca gráfica Raylib e aplica conceitos fundamentais de programação, como modularização, estruturas de dados, manipulação de arquivos, controle de estados, gerenciamento de áudio e detecção de colisões.

Várias fontes foram pesquisadas para a construção do projeto, dentre elas StackOverflow, GeeksForGeeks, as documentações oficiais do RayLib e do próprio C. Toda funcionalidade não vista em aula teve a sua origem referenciada diretamente no código.

A música e efeitos sonoros foram retirados de sites com linceça gratuita para uso. Créditos aos criadores:

- wyver9 — https://opengameart.org/content/arcade-level-tracks-8-bitchiptune
- ReyDros — https://freesound.org/s/327054/
- 0new4y — https://opengameart.org/content/game-over-trumpet-sfx
- spuispuin — https://opengameart.org/content/won-orchestral-winning-jingle

## Funcionalidades

- Movimentação da plataforma por teclado;
- Sistema de colisões entre bola, plataforma, paredes e tijolos;
- Sistema de vidas;
- Sistema de pontuação;
- Sistema de powerups;
- Fases carregadas a partir de arquivos;
- Transição entre fases;
- Sistema de salvamento e carregamento com arquivos binários (`.dat`);
- Sistema de ranking com armazenamento e ordenação das melhores pontuações;
- Efeitos sonoros e trilha sonora;
- Interface gráfica completa desenvolvida com menus, HUD e feedbacks visuais.

## Estrutura do Projeto

- **bin/** : executável gerado após a compilação
- **fases/** : arquivos de texto contendo os mapas das fases (`fase1.txt`, etc.)
- **include/** : arquivos de cabeçalho (`.h`)
- **saves/** : arquivos binários contendo o progresso salvo das partidas (`.dat`)
- **src/** : código-fonte da aplicação (`.c`)
- **ranking.txt** : arquivo para armazenamento do ranking
- **TODO.md** : controle de tarefas e pendências do desenvolvimento
- **README.md**
- **Doxyfile**

## Organização dos Módulos

| Módulo                                   | Responsabilidade                                                             |
| ---------------------------------------- | ---------------------------------------------------------------------------- |
| `main.c` / `game_core.c` / `game_core.h` | Inicialização da aplicação, utilitários e controle do loop principal         |
| `game.h`                                 | Dados globais do jogo (bola, plataforma, level, powerups do nível, etc)      |
| `game_state.h`                           | Lógica central da partida e estados do jogo (Menu, Gameplay, Gameover)       |
| `ball.c`                                 | Lógica, movimentação, propriedades físicas e vetores de velocidade da bola   |
| `brick.c`                                | Comportamento, resistência, renderização e tipos de blocos                   |
| `power_up.c`                             | Gerenciamento, comportamento e aplicação dos efeitos dos powerups            |
| `level.c`                                | Leitura, interpretação de matrizes de arquivos externos e transição de fases |
| `menu.c`                                 | Interface, navegação e interações dos menus principais                       |
| `ui.c`                                   | Renderização da Interface de Usuário, HUD de jogo e textos                   |
| `audio.c`                                | Inicialização do som, controle de efeitos sonoros e músicas                  |
| `save.c`                                 | Salvamento e carregamento de arquivos de save                                |
| `ranking.c`                              | Leitura, ordenação e escrita do ranking de pontuações no disco               |

## Requisitos

- Linguagem C
- GCC (Compilador)
- Biblioteca Raylib

## Compilação

A partir da raiz do projeto no Ubuntu/Linux, execute:

```bash
gcc src/*.c -Iinclude -o bin/jogo \
-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```
e em seguida rode usando:

```bash
./bin/jogo
```

## Geração da Documentação (Doxygen)

O projeto utiliza **Doxygen** para geração automática da documentação técnica a partir dos comentários presentes no código-fonte.

Na raiz do projeto (onde está o `Doxyfile`), execute:

```bash
doxygen Doxyfile
```

A documentação gerada pode ser visualizadas no Linux usando os comandos:

```bash
xdg-open html/index.html
```

ou

```bash
cd latex
make
```

e em seguida, após a compilação:

```bash
xdg-open refman.pdf
```
