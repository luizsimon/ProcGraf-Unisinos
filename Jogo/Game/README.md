# 🐵 Monkey Game

Monkey Game é um jogo 2D simples desenvolvido usando OpenGL e C++, onde o objetivo é controlar um macaco para coletar bananas e evitar espadas que caem do céu. O projeto foi criado para demonstrar conceitos básicos de manipulação de sprites, renderização 2D, transformações de objetos e interações com o usuário utilizando OpenGL.

Jogo Desenvolvido por Luiz Simon e Gustavo Cortezia

## Links Gravação:

- https://www.youtube.com/watch?v=kfp1l2S9rFo
- https://www.youtube.com/watch?v=9q1cnj7DtOM

## 📜 Sumário

- [Instalação](#instalação)
- [Fluxo de Execução do Jogo](#fluxo-de-execução-do-jogo)
- [Como Jogar](#como-jogar)
- [Controles](#controles)
- [Estrutura do Código](#estrutura-do-código)
- [Detalhes Técnicos](#detalhes-técnicos)
- [Créditos](#créditos)

## 📥 Instalação

Para compilar e executar o jogo Monkey Game, siga os passos abaixo:

### Pré-requisitos

Certifique-se de que seu ambiente tenha as seguintes bibliotecas instaladas:

- **GLFW** - Biblioteca para criação de janelas e gerenciamento de contexto OpenGL.
- **GLAD** - Biblioteca para carregar extensões OpenGL.
- **STB_IMAGE** - Biblioteca para carregamento de imagens como texturas.
- **GLM** - Biblioteca para manipulação de vetores e matrizes.
- **OpenGL** - Necessário para renderização gráfica.

### Compilação

Clone o repositório do projeto:

```bash
git clone https://github.com/StukerVitor/AtividadesPG
```

Entre no diretório do projeto:

```bash
cd '.\Jogo\HelloTriangle - VSCode\'
```

Rode o jogo através do botão play do VSCode.

Certifique-se de que as bibliotecas necessárias estejam corretamente instaladas e linkadas no seu ambiente de desenvolvimento.

## 🔄 Fluxo de Execução do Jogo

A execução do jogo Monkey Game segue um fluxo estruturado que pode ser dividido em várias etapas, desde a inicialização dos componentes até a renderização dos elementos na tela e a atualização contínua do estado do jogo.

### 1. Inicialização do Ambiente e Configuração de Janelas

### Inicialização do GLFW

O jogo começa inicializando a biblioteca GLFW com `glfwInit()`, que é essencial para criar janelas e contextos OpenGL.

### Criação da Janela Principal

Uma janela principal é criada usando `glfwCreateWindow()` com as dimensões definidas (800x600) e um título personalizado `"##### Monkey Game #####"`.

### Configuração do Contexto OpenGL

O contexto OpenGL é associado à janela recém-criada usando `glfwMakeContextCurrent(window)`.

### Carregamento de Extensões OpenGL com GLAD

A biblioteca GLAD é inicializada com `gladLoadGLLoader()` para carregar todas as funções OpenGL necessárias para renderização.

### Configuração de Callbacks

Funções de callback são registradas para gerenciar eventos de entrada:

- `glfwSetKeyCallback(window, key_callback)`: Para eventos de teclado.
- `glfwSetMouseButtonCallback(window, mouse_button_callback)`: Para eventos de mouse.

### Configuração de Parâmetros OpenGL

- **Teste de Profundidade**: Habilitado com `glEnable(GL_DEPTH_TEST)` e configurado com `glDepthFunc(GL_ALWAYS)`.
- **Blending (Transparência)**: Habilitado com `glEnable(GL_BLEND)` e configurado com `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)` para permitir transparência nas texturas.

### Configuração dos Shaders

Os shaders são compilados e configurados usando a função `setupShader()`, que retorna o identificador do programa de shader (`shaderID`). O shader é então ativado com `glUseProgram(shaderID)`.

### Configuração da Matriz de Projeção Ortográfica

Uma matriz de projeção ortográfica é criada usando `ortho()` para renderizar objetos 2D, e é enviada ao shader com `glUniformMatrix4fv()`.

### 2. Carregamento de Texturas

### Configuração da Biblioteca STB Image

A biblioteca STB Image é configurada para inverter as imagens carregadas verticalmente com `stbi_set_flip_vertically_on_load(true)`, garantindo que as texturas sejam mapeadas corretamente.

### Carregamento de Imagens

As texturas são carregadas a partir dos arquivos de imagem correspondentes usando a função `loadTexture()`:

- **Banana**: `../Textures/Banana/banana.png`
- **Macaco (direita e esquerda)**: `../Textures/Macaco/macacoRight.png` e `../Textures/Macaco/macacoLeft.png`
- **Espadas**: `../Textures/craftpix/PNG/Transperent/Icon5.png` e `Icon17.png`
- **Background**: `../Textures/PixelArt/BackGround/nature_5/orig.png`
- **Placar (números de 0 a 3)**: `../Textures/Placar/0.png` a `3.png`
- **Botões**: `../Textures/Botoes/start.png`, `playAgain.png`, `exit.png`
- **Telas de Game Over e Vitória**: `../Textures/gameOver/gameOver.png`, `vitoria.png`

### Armazenamento dos Identificadores de Textura

Cada chamada a `loadTexture()` retorna um identificador de textura (`GLuint`), que é armazenado em variáveis para uso posterior na criação dos sprites.

### 3. Criação e Configuração de Sprites

### Instanciação de Sprites

São criadas instâncias da classe `Sprite` para cada elemento do jogo:

- **Personagem Principal**: `macaco`
- **Obstáculos**: `espada_1` e `espada_2`
- **Objetivo**: `banana`
- **Elementos de Interface**: `background`, `gameOver`, `vitoria`, `placar`, `placarZero`, `placarUm`, `placarDois`, `placarTres`
- **Botões**: `startButton`, `playAgainButton`, `exitButton`

### Configuração dos Sprites

- **Shader Associado**: Cada sprite tem o shader configurado com `setShader(shaderID)`.
- **Inicialização**: Os sprites são inicializados com `inicializar(texID, posição, escala, ângulo)`:
  - Exemplo: `macaco.inicializar(texID_macacoR, vec3(300.0f, 120.0f, 0.0f), vec3(65.0f, 70.0f, 1.0f))`
- **Velocidade de Queda**: Para sprites que caem (espadas e banana), a velocidade de queda é definida com `setFallSpeed(valor)`.

### Configuração Específica de Sprites

- **Macaco**: Inicia na posição `(300.0f, 120.0f)` com textura `macacoRight`.
- **Espadas**: Iniciam acima da tela e caem em direção ao macaco.
- **Banana**: Também cai do topo da tela e deve ser coletada pelo macaco.
- **Placar**: Exibe a pontuação atual do jogador, atualizando a textura conforme o jogador coleta bananas.

### 4. Loop Principal do Jogo

O loop principal é responsável por atualizar o estado do jogo, processar entradas e renderizar os sprites. Este loop continua até que a janela seja fechada.

### 4.1. Processamento de Eventos

- **Entrada de Usuário**: Eventos de teclado e mouse são processados com `glfwPollEvents()`.
- **Movimento do Macaco**: Verifica se as teclas `D` ou `Seta Direita` e `A` ou `Seta Esquerda` estão pressionadas para mover o macaco.
- **Atualização da Textura do Macaco**: Dependendo da direção do movimento, a textura do macaco é atualizada para `macacoRight` ou `macacoLeft` usando `macaco.switchSide(texID_macacoR)`.

### 4.2. Atualização de Estados e Lógica de Jogo

#### Controle de Estados (`gameState`):

- **0 - Menu Inicial**:
  - Exibe o fundo e o botão "Start".
  - Aguarda o clique do usuário no botão "Start" para iniciar o jogo.
- **1 - Jogo em Andamento**:
  - O macaco pode ser movido pelo jogador.
  - As espadas e a banana caem em direção ao macaco.
  - Verifica colisões:
    - **Colisão com Banana**:
      - Incrementa a pontuação (`pontos++`).
      - Atualiza o placar exibido.
      - Verifica se o jogador alcançou a pontuação necessária para vencer (`pontos >= quantPont`).
    - **Colisão com Espadas**:
      - Define o estado de Game Over (`gmOver = 1`).
      - Transita para o estado de Game Over (`gameState = 2`).
- **2 - Tela de Game Over**:
  - Exibe a tela de Game Over e os botões "Play Again" e "Exit".
  - Aguarda a interação do usuário para reiniciar o jogo ou sair.
- **3 - Tela de Vitória**:
  - Exibe a tela de Vitória e os botões "Play Again" e "Exit".
  - Aguarda a interação do usuário para reiniciar o jogo ou sair.

#### Movimento dos Sprites

- **Espadas e Banana**: Atualizam suas posições chamando `cair(false)`, fazendo com que caiam continuamente.
- **Macaco**: Movido pelo usuário através das entradas de teclado.

#### Verificação de Colisões

- A função `colisao()` é utilizada para detectar colisões entre o macaco e as espadas ou banana.
- **Colisão com Banana**:
  - A banana é reposicionada para cair novamente.
- **Colisão com Espadas**:
  - O jogo é interrompido e transita para a tela de Game Over.

### 4.3. Renderização

- **Limpeza da Tela**: Limpa o buffer de cor com `glClear(GL_COLOR_BUFFER_BIT)`.

- **Renderização dos Sprites**:

  - **Menu Inicial**:
    - Desenha o `background` e o `startButton`.
  - **Jogo em Andamento**:
    - Desenha o `background`, `espada_1`, `espada_2`, `banana`, `placar` e o `macaco`.
  - **Tela de Game Over**:
    - Desenha o `background`, `gameOver`, `playAgainButton` e `exitButton`.
  - **Tela de Vitória**:
    - Desenha o `background`, `vitoria`, `playAgainButton` e `exitButton`.

- **Troca de Buffers**: Os buffers são trocados com `glfwSwapBuffers(window)` para atualizar a tela com as renderizações do frame atual.

### 5. Finalização

### Encerramento do Jogo

Quando o usuário fecha a janela ou seleciona "Exit", o loop principal é interrompido.

### Liberação de Recursos

- **Terminação do GLFW**: `glfwTerminate()` é chamado para finalizar a biblioteca GLFW e liberar recursos alocados.

### Retorno da Função Principal

O programa retorna `0` indicando que foi encerrado com sucesso.

## 🎮 Como Jogar

O objetivo do jogo é controlar o macaco para coletar bananas e evitar as espadas que caem do topo da tela. A cada banana coletada, o jogador ganha 1 ponto. Se o jogador atingir 3 pontos, ele ganha o jogo. Se colidir com uma espada, o jogo termina.

### Objetivo

- **Coletar Bananas**: O macaco deve coletar bananas para ganhar pontos.
- **Evitar Espadas**: O jogador deve desviar das espadas para não perder.

## 🕹️ Controles

- **Seta Direita** ou **Tecla D**: Move o macaco para a direita.
- **Seta Esquerda** ou **Tecla A**: Move o macaco para a esquerda.
- **Tecla ESC**: Fecha o jogo.

### Interações com o Mouse

- Clique no Botão **"Start"**: Inicia o jogo.
- Clique no Botão **"Play Again"**: Reinicia o jogo após Game Over ou Vitória.
- Clique no Botão **"Exit"**: Fecha o jogo.

## 📂 Estrutura do Código

Cada arquivo no projeto tem um papel específico:

- **`Sprite.h`**: Define a classe Sprite para representar objetos 2D.
- **`Sprite.cpp`**: Implementa as funções de transformação, desenho e movimento dos sprites.
- **`Source.cpp`**: Contém a lógica principal do jogo, como a inicialização, manipulação de eventos, lógica de colisão e renderização.

## 📜 Detalhes Técnicos

- **Linguagem de Programação**: C++
- **Frameworks**: OpenGL, GLFW, GLAD, GLM
- **Bibliotecas**: STB Image para carregamento de texturas
- **Shaders**: Shaders personalizados para renderização dos sprites.

## 🙌 Créditos

- **Desenvolvedores**: Gustavo Cortezia e Luiz Simon
- **Texturas**: CraftPix, OpenGameArt, e outras fontes de arte livre.
- **Bibliotecas**: GLFW, GLAD, STB Image, GLM.
