#include <iostream>
#include "Sprite.h"

// GLAD para carregamento de extensões OpenGL
#include <glad/glad.h>

// GLFW para criação e gerenciamento de janelas
#include <GLFW/glfw3.h>

// GLM para manipulação de vetores e matrizes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

// Construtor padrão do Sprite, inicializa variáveis e buffers
Sprite::Sprite()
{
	fallSpeed = 1.0f; // Define a velocidade de queda padrão
	VBO = 0;		  // Inicializa o VBO com 0
}

// Destrutor do Sprite, limpa recursos do OpenGL
Sprite::~Sprite()
{
	// Libera o VAO e VBO associados ao Sprite
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

// Método para inicializar o sprite com textura, posição, escala e ângulo
void Sprite::inicializar(GLuint texID, vec3 pos, vec3 escala, float angulo)
{
	this->texID = texID;   // Define o identificador da textura
	this->pos = pos;	   // Define a posição inicial
	this->escala = escala; // Define a escala inicial
	this->angulo = angulo; // Define o ângulo de rotação inicial

	// Dados dos vértices: posição (x, y, z) e coordenadas de textura (s, t)
	GLfloat vertices[] = {
		// Triângulo 0
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,	// v0
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // v1
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,	// v2
		// Triângulo 1
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // v1
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// v3
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f	// v2
	};

	// Gera buffers para os vértices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Gera o Vertex Array Object (VAO)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Configura os atributos de vértice para posição e coordenadas de textura
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Desvincula os buffers para evitar modificações acidentais
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Retorna a posição X do sprite
float Sprite::GetXPos()
{
	return pos.x;
}

// Retorna a posição Y do sprite
float Sprite::GetYPos()
{
	return pos.y;
}

// Método para desenhar o sprite na tela
void Sprite::desenhar()
{
	// Atualiza a matriz de transformação e envia para o shader
	atualizar();

	// Ativa o programa de shader
	glUseProgram(shaderID);

	// Vincula a textura associada ao sprite
	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO);

	// Desenha o sprite utilizando os vértices configurados
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Desvincula a textura e o VAO
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

// Movimenta o sprite para a direita com o deslocamento especificado
void Sprite::moveRight(float offset)
{
	if (pos.x >= 800) // Se o sprite ultrapassar a borda direita da tela
	{
		pos.x = 1; // Reposiciona no início da tela
	}
	else
		pos.x += offset; // Move para a direita
}

// Movimenta o sprite para a esquerda com o deslocamento especificado
void Sprite::moveLeft(float offset)
{
	if (pos.x <= 0) // Se o sprite ultrapassar a borda esquerda da tela
	{
		pos.x = 799; // Reposiciona no final da tela
	}
	else
		pos.x -= offset; // Move para a esquerda
}

// Aplica movimento de queda ao sprite e redefine se necessário
void Sprite::cair(bool reset)
{
	static bool rotateRight = true;		   // Direção inicial de rotação
	static float rotationSpeed = 2.0f;	   // Velocidade de rotação
	static float maxRotationAngle = 30.0f; // Ângulo máximo para rotação

	// Se o sprite estiver abaixo do limite ou precisar ser resetado
	if (pos.y < 100 || reset)
	{
		pos.x = 10.0f + static_cast<float>(rand() % 781); // Reposiciona em X aleatoriamente
		pos.y = 1000.0f;								  // Reposiciona no topo da tela
		angulo = 0.0f;									  // Reseta o ângulo de rotação
	}
	else
	{
		// Movimento de queda
		pos.y -= fallSpeed;

		// Animação de rotação enquanto cai
		if (rotateRight)
		{
			angulo += rotationSpeed * 0.016f; // Incrementa o ângulo se estiver rotacionando para a direita
			if (angulo > maxRotationAngle)
			{
				angulo = maxRotationAngle; // Limita o ângulo para não passar do máximo
				rotateRight = false;	   // Muda a direção para a esquerda
			}
		}
		else
		{
			angulo -= rotationSpeed * 0.016f; // Decrementa o ângulo se estiver rotacionando para a esquerda
			if (angulo < -maxRotationAngle)
			{
				angulo = -maxRotationAngle; // Limita o ângulo para não passar do mínimo
				rotateRight = true;			// Muda a direção para a direita
			}
		}
	}
}

// Atualiza a matriz de transformação do sprite e envia para o shader
void Sprite::atualizar()
{
	mat4 model = mat4(1.0f);										// Cria a matriz de modelo como identidade
	model = translate(model, pos);									// Aplica translação
	model = rotate(model, radians(angulo), vec3(0.0f, 0.0f, 1.0f)); // Aplica rotação
	model = scale(model, escala);									// Aplica escala

	// Ativa o shader
	glUseProgram(shaderID);

	// Envia a matriz de modelo para o shader
	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
}

// Troca a textura do sprite
void Sprite::switchSide(GLuint texID_Ref)
{
	texID = texID_Ref;
}

// Define a velocidade de queda do sprite
void Sprite::setFallSpeed(float speed)
{
	fallSpeed = speed;
}