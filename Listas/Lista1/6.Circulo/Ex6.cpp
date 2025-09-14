#include <assert.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Protótipos das funções
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
int setupShader();
int setupGeometry(int nPoints, float radius = 0.5);

const float Pi = 3.14159;

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

// Código fonte do Fragment Shader (em GLSL): ainda hardcoded
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

	// Exercício 6
	// nPoints = 30 --> Círculo
	// nPoints = 8  --> Octógono
	// nPoints = 5  --> Pentágono
	// nPoints = 30 --> pac-man -> Alterar linha 190(habilitar) e 188(comentar)
	// nPoints = 30 --> Pizza -> Alterar linha 103(comentar), 104(habilitar) e verificar linha 188(deve estar habilitada)
	int nPoints = 30;
	int nVertices = nPoints + 2; // Código para exercício 6 - Círculo, Octógono, Pentágono, Pizza
	GLuint VAO = setupGeometry(nPoints);

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

		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); // Cor azul
		//glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices);	  // Código para Círculo, Octógono, Pentágono, Pac-man
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6); // Código para Fatia de pizza

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

int setupGeometry(int nPoints, float radius) {
	vector<GLfloat> vertices;

	float angle = 0.0;
	float slice = 2 * Pi / (float)nPoints;

	// Adicionar o ponto da origem (0.0,0.0,0.0) como centro do círculo.
	// push_back-- > função de adicionar no final do array
	vertices.push_back(0.0); // Xc
	vertices.push_back(0.0); // Yc
	vertices.push_back(0.0); // Zc

	for(int i = 0; i < nPoints + 1; i++) // Código para exercício 6 - Círculo, Octógono, Pentágono, Fatia de Pizza
	{
		// for (int i = 0; i < nPoints - 2; i++)
		//{ // Código para exercício 6 - c) Pac-man

		float x = radius * cos(angle);
		float y = radius * sin(angle);
		float z = 0.0;

		vertices.push_back(x); // X
		vertices.push_back(y); // Y
		vertices.push_back(z); // Z

		angle += slice;
	}

	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGL
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);

	// Para cada atributo do vértice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Número de valores que o atributo tem (por ex, 3 coordenadas xyz)
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
