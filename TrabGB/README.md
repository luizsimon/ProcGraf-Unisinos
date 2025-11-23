- Nomes: Gustavo Cortezia e Luiz Simon

# Explicação Completa do Código

Este documento fornece uma explicação detalhada do código fornecido, que é um aplicativo em Python utilizando a biblioteca OpenCV para aplicar filtros e stickers (adesivos) em imagens e vídeos capturados pela webcam. O programa permite que o usuário interaja em tempo real com a imagem, aplicando diferentes efeitos visuais e adicionando stickers em posições específicas.

- cv2: Biblioteca OpenCV para processamento de imagens e vídeos.
- numpy: Biblioteca para manipulação de arrays e operações numéricas.

## Filtros Disponíveis:

1. Canny Edge Detection (c):

   - Detecta bordas na imagem.
   - Parâmetros: Limite inferior de 50 e superior de 100.
   - Função: cv2.Canny(frame, 50, 100)

2. Gaussian Blur (b):

   - Aplica um desfoque gaussiano para suavizar a imagem.
   - Parâmetros: Kernel de tamanho (15, 15) e desvio padrão 0.
   - Função: cv2.GaussianBlur(frame, (15, 15), 0)

3. Filtro Magenta (m):

   - Aplica um efeito magenta personalizado.
   - Função: apply_magenta(frame)

4. Filtro Sobel (s):

   - Calcula gradientes da imagem para detecção de bordas.
   - Parâmetros: Profundidade de saída cv2.CV_64F, derivada em X, kernel de tamanho 5.
   - Função: cv2.Sobel(frame, cv2.CV_64F, 1, 0, ksize=5)

5. Filtro Laplaciano (l):

   - Detecta bordas e detalhes finos.
   - Parâmetros: Profundidade de saída cv2.CV_64F.
   - Função: cv2.Laplacian(frame, cv2.CV_64F)

6. Equalização de Histograma (h):

   - Melhora o contraste da imagem em escala de cinza.
   - Função: cv2.equalizeHist(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY))

7. Binarização (t):

   - Converte a imagem em uma imagem binária (preto e branco).
   - Parâmetros: Limite de 127, valor máximo de 255.
   - Função: cv2.threshold(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY), 127, 255, cv2.THRESH_BINARY)[1]

8. Erosão (e):

   - Erode áreas brancas em uma imagem binária.
   - Parâmetros: Kernel de tamanho (5, 5), 1 iteração.
   - Função: cv2.erode(frame, np.ones((5, 5), np.uint8), iterations=1)

9. Dilatação (d):

   - Expande áreas brancas em uma imagem binária.
   - Parâmetros: Kernel de tamanho (5, 5), 1 iteração.
   - Função: cv2.dilate(frame, np.ones((5, 5), np.uint8), iterations=1)

10. Inversão de Cores (n):
    - Inverte as cores da imagem.
    - Função: cv2.bitwise_not(frame)

## Resumo do Fluxo do Programa

1.  Inicialização:

    - Importa bibliotecas necessárias.
    - Define dicionários de filtros e stickers.
    - Declara variáveis globais.

2.  Definição de Funções:

    - apply_magenta(): Aplica o filtro magenta personalizado.
    - display_instructions(): Exibe instruções de comandos.
    - overlay_sticker(): Sobrepõe stickers na imagem.
    - mouse_callback(): Gerencia eventos do mouse para posicionar stickers.
    - edit_image(): Permite edição de imagens estáticas.
    - edit_webcam(): Permite edição do vídeo da webcam em tempo real.
    - main(): Gerencia o menu principal e a execução geral do programa.

3.  Execução:

    - O programa inicia chamando main().
    - O usuário interage através do menu e comandos exibidos.
    - O programa aplica filtros e stickers com base nas interações do usuário.
    - O usuário pode salvar as imagens editadas ou sair do programa.
