#include <assert.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

// Protótipos das funções
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, int button, int action, int mods);
int setupShader();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Variáveis globais para controlar os vértices e a geometria
std::vector<GLfloat> vertices;
int vertexCount = 0;
GLuint VAO, VBO;

// Cor de cada triângulo (mantenha como global)
GLfloat colors[3][4] = {
	 {1.0f, 0.0f, 0.0f, 1.0f}, // Vermelho
	 {0.0f, 1.0f, 0.0f, 1.0f}, // Verde
	 {0.0f, 0.0f, 1.0f, 1.0f}	// Azul
};

// Código fonte do Vertex Shader (em GLSL)
const GLchar *vertexShaderSource = "#version 400\n"
											  "layout (location = 0) in vec3 position;\n"
											  "void main()\n"
											  "{\n"
											  "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
											  "}\0";

// Código fonte do Fragment Shader (em GLSL)
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
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Exercício 6", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro das funções de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);

	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Configurar a geometria inicial (VAO e VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Aloca uma quantidade inicial de memória para o VBO, que será atualizada dinamicamente
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	glUseProgram(shaderID);

	// Loop da aplicação - "game loop"
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(2);
		glPointSize(10);

		// Desenhar os triângulos
		if(vertexCount > 0) {
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());

			for(int i = 0; i < vertexCount / 3; ++i) {
				int colorIndex = i % 3;
				glUniform4f(colorLoc, colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2], colors[colorIndex][3]);
				glDrawArrays(GL_TRIANGLES, i * 3, 3);
			}
		}

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Função de callback do mouse para criar vértices
void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Mapeia as coordenadas do mouse para as coordenadas do mundo (-1.0 a 1.0)
		float x = (xpos / (double)WIDTH) * 2.0 - 1.0;
		float y = 1.0 - (ypos / (double)HEIGHT) * 2.0;

		// Adiciona o novo vértice ao vetor
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertexCount++;
	}
}

// Esta função está bastante hardcoded - objetivo é compilar e "buildar" um programa de
//  shader simples e único neste exemplo de código
//  O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
//  fragmentShader source no início deste arquivo
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