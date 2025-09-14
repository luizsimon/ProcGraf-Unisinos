# Lista 2

## 1. Modifique a janela do mundo (window/ortho) para os limites: xmin=-10, xmax=10, ymin=-10, ymax=10.

![Desenho](/Listas/Lista2/2_1/Prints/Ex2_1.png)

## 2. Agora modifique para: xmin=0, xmax=800, ymin=600, ymax=0.

![Desenho](/Listas/Lista2/2_2/Prints/Ex2_2.png)

## 3. Utilizando a câmera 2D do exercício anterior, desenhe algo na tela. O que acontece quando posicionamos os objetos? Por que é útil essa configuração?

Com o aumento das proporções, a figura (triângulo) deixa de ser visível no plano mundo. No entanto, essa configuração é útil para definir os limites mínimo e máximo dos eixos x e y.

## 4. Modifique o viewport para desenhar a cena apenas no seguinte quadrante da janela da aplicação:

![Desenho](/Listas/Lista2/2_4/Prints/Ex2_4.png)

## 5. Agora, desenhe a mesma cena nos 4 quadrantes.

![Desenho](/Listas/Lista2/2_5/Prints/Ex2_5.png)

## 5. Agora, desenhe a mesma cena nos 4 quadrantes.

![Desenho](/Listas/Lista2/2_5/Prints/Ex2_5.png)

## 6. Crie triângulos a partir do clique do mouse

Seu programa deve seguir os seguintes requisitos:

- A cada clique na tela, apenas **1 vértice** será criado.  
- Quando **3 vértices** forem definidos, um **triângulo** será formado.  
- Cada novo triângulo deve possuir uma cor diferente.

**Dicas:**  
- É possível usar um único VAO ou múltiplos VAOs.  
- Para simplificar o mapeamento, utilize projeção ortográfica com as mesmas dimensões da tela (por exemplo, 800 × 600). Assim, cada unidade do mundo corresponderá a um pixel da tela.  
- Consulte o [GLFW Input Guide](https://www.glfw.org/docs/latest/input_guide.html) para manipular eventos de clique do mouse.
 
![Triângulos com cliques](/Listas/Lista2/2_6/Prints/Ex2_6.png)
