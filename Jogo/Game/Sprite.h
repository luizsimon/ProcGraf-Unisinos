// Sprite.h

#pragma once

// Biblioteca GLAD para carregamento de extensões OpenGL
#include <glad/glad.h>

// GLFW para criação e gerenciamento de janelas
#include <GLFW/glfw3.h>

// GLM para manipulação de vetores e matrizes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// Classe Sprite que representa um objeto 2D com transformações e texturas
class Sprite
{
public:
	Sprite();  // Declaração do construtor padrão
	~Sprite(); // Destrutor para liberar recursos

	// Método para inicializar o sprite com uma textura, posição, escala e ângulo
	void inicializar(GLuint texID, vec3 pos = vec3(0.0, 0.0, 0.0), vec3 escala = vec3(1.0, 1.0, 1.0), float angulo = 0.0);

	// Métodos para obter a posição em X e Y do sprite
	float GetXPos();
	float GetYPos();

	// Método para desenhar o sprite na tela
	void desenhar();

	// Métodos para mover o sprite para a direita ou para a esquerda
	void moveRight(float offset);
	void moveLeft(float offset);

	// Método para aplicar movimento de queda ao sprite
	void cair(bool reset);

	// Método para alterar a textura do sprite
	void switchSide(GLuint texID_Ref);

	// Método para definir a velocidade de queda do sprite
	void setFallSpeed(float speed);

	// Métodos Getters para obter a posição e escala do sprite
	vec3 GetPosition() const { return pos; }
	vec3 GetScale() const { return escala; }

	// Métodos Setters para modificar a posição e escala do sprite
	void SetPosition(const vec3 &newPos) { pos = newPos; }
	void SetScale(const vec3 &newScale) { escala = newScale; }

	// Método para definir o shader utilizado pelo sprite
	void setShader(GLuint shaderID) { this->shaderID = shaderID; }

protected:
	// Método para atualizar a matriz de transformação do sprite
	void atualizar();

	// Identificadores para o VAO (Vertex Array Object) e VBO (Vertex Buffer Object)
	GLuint VAO;
	GLuint VBO;

	// Identificadores para a textura e shader do sprite
	GLuint texID;
	GLuint shaderID;

	// Variáveis para armazenar posição, escala, ângulo de rotação e velocidade de queda do sprite
	vec3 pos, escala;
	float angulo;
	float fallSpeed; // Velocidade de queda do sprite
};
