#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Protótipos das funções
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
int setupShader();
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = "#version 400\n"
											  "layout (location = 0) in vec3 position;\n"
											  "void main()\n"
											  "{\n"
											  //...pode ter mais linhas de código aqui!
											  "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
											  "}\0";

// Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = "#version 400\n"
												 "uniform vec4 inputColor;\n"
												 "out vec4 color;\n"
												 "void main()\n"
												 "{\n"
												 "color = inputColor;\n"
												 "}\n\0";

// Função MAIN
int main() {
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 9", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	 /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de dois triângulos
	GLuint VAO = setupGeometry();

	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	glUseProgram(shaderID);

	// Loop da aplicação - "game loop"
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(2);
		glPointSize(10);

		glBindVertexArray(VAO); // Conectando ao buffer de geometria

		// Casa - Amarelo
		glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		// Telhado - Marrom
		glUniform4f(colorLoc, 0.6f, 0.3f, 0.2f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 4, 3);

		// Porta - Marrom escuro
		glUniform4f(colorLoc, 0.3f, 0.15f, 0.05f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 7, 4);

		// Janela esquerda - Cinza claro
		glUniform4f(colorLoc, 0.8f, 0.8f, 0.8f, 1.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

		// Janela direita - Cinza claro
		glUniform4f(colorLoc, 0.8f, 0.8f, 0.8f, 1.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

		// Contorno da Casa - Preto
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		// Chão - Verde escuro
		glUniform4f(colorLoc, 0.0f, 0.5f, 0.0f, 1.0f);
		glDrawArrays(GL_LINES, 19, 20);

		glBindVertexArray(0); // Desconectando o buffer de geometria

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Esta função está bastante hardcoded - objetivo é compilar e "buildar" um programa de
//  shader simples e único neste exemplo de código
//  O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
//  fragmentShader source no iniçio deste arquivo
//  A função retorna o identificador do programa de shader
int setupShader() {
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
					 << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
					 << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
					 << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupGeometry() {
	// Aqui setamos as coordenadas x, y e z
	GLfloat vertices[] = {
		 // x   y   z
		 // Casa
		 0.5, 0.0, 0.0,	 // v0
		 -0.5, 0.0, 0.0,	 // v1
		 -0.5, -0.99, 0.0, // v2
		 0.5, -0.99, 0.0,	 // v3

		 // Telhado
		 0.0, 0.5, 0.0,  // v4
		 0.5, 0.0, 0.0,  // v5
		 -0.5, 0.0, 0.0, // v6

		 // Porta
		 0.15, -0.5, 0.0,	  // v7
		 0.15, -0.99, 0.0,  // v8
		 -0.15, -0.5, 0.0,  // v9
		 -0.15, -0.99, 0.0, // v10

		 // Janela esquerda
		 -0.2, -0.1, 0.0, // v11
		 -0.2, -0.3, 0.0, // v12
		 -0.4, -0.3, 0.0, // v13
		 -0.4, -0.1, 0.0, // v14

		 // Janela direita
		 0.2, -0.1, 0.0, // v15
		 0.2, -0.3, 0.0, // v16
		 0.4, -0.3, 0.0, // v17
		 0.4, -0.1, 0.0, // v18

		 // Chão
		 -0.5, -1.0, 0.0, // v19
		 0.5, -1.0, 0.0,	// v20
	};

	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);

	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	//  Tipo do dado
	//  Se está normalizado (entre zero e um)
	//  Tamanho em bytes
	//  Deslocamento a partir do byte zero
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}
