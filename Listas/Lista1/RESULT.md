# Lista 1

## 1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?

OpenGL Shading Language (GLSL) é uma linguagem de programação de alto nível, semelhante ao C, usada para escrever shaders em OpenGL. Na versão do OpenGL 4 que usamos, os dois shaders essenciais são:

- **Vertex Shader**: responsável pelo processamento de vértices.
- **Fragment Shader**: lida com a forma como as áreas das superfícies são renderizadas.

## 2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?

As primitivas gráficas representam os elementos mais básicos na construção de formas em OpenGL:

- **GL_POINTS**: desenha pontos individuais.
- **GL_LINES**: cria uma linha para cada par de pontos.
- **GL_LINE_STRIP**: conecta todos os pontos em uma sequência de linhas.
- **GL_LINE_LOOP**: similar ao GL_LINE_STRIP, mas fecha o loop ao conectar o último ponto ao primeiro.
- **GL_TRIANGLES**: forma triângulos a partir de grupos de três pontos.
- **GL_TRIANGLE_STRIP**: cria uma série de triângulos conectados.
- **GL_TRIANGLE_FAN**: forma triângulos a partir de um ponto central e dois pontos adjacentes.

Na OpenGL, os vértices são geralmente armazenados usando Vertex Buffer Objects (VBOs).

## 3. Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).

- **Vertex Buffer Object (VBO)**: é um buffer que armazena dados de vértices, como posições e cores, diretamente na memória da GPU, acelerando o processo de renderização.
- **Vertex Array Object (VAO)**: é utilizado para configurar os atributos dos vértices, determinando qual VBO será usado, além de especificar o formato e a localização desses dados.
- **Element Buffer Object (EBO)**: também conhecido como Index Buffer Object (IBO), armazena índices que definem quais vértices formarão cada primitiva gráfica, permitindo reutilizar vértices e economizar memória.

## 4. Analise o código fonte do projeto Hello Triangle. Localize e relacione os conceitos de shaders, VBOs e VAO apresentados até então. Não precisa entregar nada neste exercício.

## 5. Faça o desenho de 2 triângulos na tela. Desenhe-os:

a. **Apenas com o polígono preenchido**
![Apenas com o polígono preenchido](/Listas/Lista1/5.Triangulo/Prints/5a.png)

b. **Apenas com contorno**  
![Apenas com contorno](/Listas/Lista1/5.Triangulo/Prints/5b.png)

c. **Apenas como pontos**  
![Apenas como pontos](/Listas/Lista1/5.Triangulo/Prints/5c.png)

d. **Com as 3 formas de desenho juntas**
![Com as 3 formas de desenho juntas](/Listas/Lista1/5.Triangulo/Prints/5d.png)

## 6. Faça o desenho de um círculo na tela

Utilize a equação paramétrica do círculo para gerar os vértices. Depois disso:
![Círculo](/Listas/Lista1/6.Circulo/Prints/Ex6.png)

a. **Desenhe um octágono**  
![Octágono](/Listas/Lista1/6.Circulo/Prints/Ex6a.png)

b. **Desenhe um pentágono**
![Pentágono](/Listas/Lista1/6.Circulo/Prints/Ex6b.png)

c. **Desenhe um pac-man!**
![Pac-man](/Listas/Lista1/6.Circulo/Prints/Ex6c.png)

d. **Desenhe uma fatia de pizza**
![Pizza](/Listas/Lista1/6.Circulo/Prints/Ex6d.png)

## 7. Desenhe uma espiral:

![Espiral](/Listas/Lista1/7.Espiral/Prints/Ex7.png)

## 8. Considerando o seguinte triângulo abaixo, formado pelos vértices P1, P2 e P3, respectivamente com as cores vermelho, verde e azul.

a. **Descreva uma possível configuração dos buffers (VBO, VAO e EBO) para representá-lo.**

1. **VBO (Vertex Buffer Object):**

   - Armazena os dados dos vértices do triângulo, incluindo posições e cores. Exemplo de dados no VBO:
     ```cpp
     GLfloat vertices[] = {
       0.0,  0.6, 0.0, 1.0, 0.0, 0.0,  // v0 - posição (0.0, 0.6, 0.0), cor (1.0, 0.0, 0.0)
      -0.6, -0.5, 0.0, 0.0, 1.0, 0.0,  // v1 - posição (-0.6, -0.5, 0.0), cor (0.0, 1.0, 0.0)
       0.6, -0.3, 0.0, 0.0, 0.0, 1.0   // v2 - posição (0.6, -0.3, 0.0), cor (0.0, 0.0, 1.0)
     };
     ```

2. **VAO (Vertex Array Object):**

   - Armazena os ponteiros para os atributos dos vértices. Exemplo de configuração:

   ```cpp
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);   // Atributo posição
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));  // Atributo cor
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
   ```

3. **EBO (Element Buffer Object):**
   - Não é necessário neste caso, já que o triângulo é desenhado com `glDrawArrays()` e os vértices estão organizados em ordem.

b. **Como estes atributos seriam identificados no vertex shader?**

1. **Posição (location = 0):**

   - Associado ao atributo `position` no shader:
     ```glsl
     layout (location = 0) in vec3 position;
     ```

2. **Cor (location = 1):**
   - Associado ao atributo `color` no shader:
     ```glsl
     layout (location = 1) in vec3 color;
     ```

![Triângulo](/Listas/Lista1/8.Triangulo/Prints/Ex8.png)

## 9. Faça um desenho em um papel quadriculado (pode ser no computador mesmo) e reproduza-o utilizando primitivas em OpenGL. Neste exercício você poderá criar mais de um VAO e fazer mais de uma chamada de desenho para poder utilizar primitivas diferentes, se necessário.

![Casa](/Listas/Lista1/9.Casa/Prints/Ex9.png)
