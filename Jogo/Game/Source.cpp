#include <iostream>
#include <string>

// STB Image para carregamento de imagens como texturas
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// GLAD para carregamento de extensões OpenGL
#include <glad/glad.h>

// GLFW para criação e gerenciamento de janelas
#include <GLFW/glfw3.h>

// GLM para manipulação de vetores e matrizes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Inclusão da classe Sprite para manipulação de objetos 2D
#include "Sprite.h"

using namespace glm;
using namespace std;

// Prototipação de funções
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);												// Gerencia eventos de teclado
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);												// Gerencia eventos de mouse
GLuint loadTexture(const string &texturePath);																					// Carrega uma textura de uma imagem
bool colisao(float perX, float perY, float widthPer, float heightPer, float obsX, float obsY, float widthObs, float heightObs); // Verifica colisão entre dois objetos
GLuint setupShader();																											// Compila e configura os shaders
bool isButtonClicked(double mouseX, double mouseY, Sprite &button);																// Verifica se um botão foi clicado

// Definição das dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 600;

// Variáveis globais para gerenciamento dos sprites e estado do jogo
Sprite macaco;			// Sprite do macaco no jogo
Sprite startButton;		// Sprite para o botão de início
Sprite playAgainButton; // Sprite para o botão de jogar novamente
Sprite exitButton;		// Sprite para o botão de sair
Sprite placar;			// Sprite para o placar de pontos
Sprite placarZero;		// Sprite para o placar inicial com zero pontos
int gmOver = 0;			// Estado de game over (0 = não, 1 = sim)
int pontos = 0;			// Pontuação do jogador
int quantPont = 3;		// Quantidade de pontos necessários para vencer
int gameState = 0;		// Estado do jogo (0 = Menu, 1 = Jogando, 2 = Game Over, 3 = Vitória)

// Código-fonte do Vertex Shader (Shader de vértice)
const GLchar *vertexShaderSource = R"glsl(
    #version 400 core
    layout(location = 0) in vec3 aPos;      // Posição do vértice
    layout(location = 1) in vec2 aTexCoord; // Coordenada de textura

    out vec2 TexCoord; // Variável de saída para coordenadas de textura

    uniform mat4 model;       // Matriz de modelo (transformações locais do objeto)
    uniform mat4 projection;  // Matriz de projeção (transformações globais na cena)

    void main()
    {
        // Calcula a posição final do vértice aplicando as transformações
        gl_Position = projection * model * vec4(aPos, 1.0);
        TexCoord = aTexCoord; // Passa as coordenadas de textura para o fragment shader
    }
)glsl";

// Código-fonte do Fragment Shader (Shader de fragmentos)
const GLchar *fragmentShaderSource = R"glsl(
    #version 400 core
    out vec4 FragColor;  // Cor de saída do fragmento

    in vec2 TexCoord; // Coordenadas de textura recebidas do vertex shader

    uniform sampler2D texBuffer; // Buffer de textura

    void main()
    {
        // Aplica a textura ao fragmento usando as coordenadas recebidas
        FragColor = texture(texBuffer, TexCoord);
    }
)glsl";

// Função principal que executa a aplicação
int main()
{
	// Inicializa o GLFW para criação de janelas
	if (!glfwInit())
	{
		cout << "Falha ao inicializar o GLFW" << endl;
		return -1;
	}

	// Cria a janela principal do jogo com as dimensões especificadas
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "##### Monkey Game #####", nullptr, nullptr);
	if (!window)
	{
		cout << "Falha ao criar a janela GLFW" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Define o contexto atual como a janela criada

	// Registra a função de callback para eventos de teclado
	glfwSetKeyCallback(window, key_callback);

	// Inicializa o GLAD para carregar funções OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Falha ao inicializar o GLAD" << endl;
		return -1;
	}

	// Obtém informações sobre a versão e o renderizador OpenGL
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "Renderizador: " << renderer << endl;
	cout << "Versão OpenGL suportada " << version << endl;

	// Configura a biblioteca STB para inverter a textura ao carregar
	stbi_set_flip_vertically_on_load(true);

	// Registra a função de callback para eventos de clique de mouse
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Habilitar teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS); // a cada ciclo

	// Habilita o blending para permitir transparência nas texturas
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Compila e configura os shaders do OpenGL
	GLuint shaderID = setupShader();

	// Ativa a unidade de textura 0 (primeira unidade de textura)
	glActiveTexture(GL_TEXTURE0);

	// Cria uma matriz de projeção ortográfica para renderização 2D
	mat4 projection = ortho(0.0f, 800.0f, 0.0f, 600.0f);

	// Variáveis para controle de tempo no jogo
	float deltaTime = 0.0f; // Tempo entre o quadro atual e o anterior
	float lastFrame = 0.0f; // Armazena o tempo do último quadro processado

	// Carrega as texturas utilizadas no jogo a partir dos arquivos
	GLuint texID_backGround = loadTexture("../Jogo/Textures/PixelArt/BackGround/beach.jpg");
	GLuint texID_coco_1 = loadTexture("../Jogo/Textures/PixelArt/BackGround/coconut.png");
	GLuint texID_coco_2 = loadTexture("../Jogo/Textures/PixelArt/BackGround/coconut.png");
	GLuint texID_gameOver = loadTexture("../Jogo/Textures/gameOver/gameOver.png");
	GLuint texID_vitoria = loadTexture("../Jogo/Textures/gameOver/vitoria.png");
	GLuint texID_macacoR = loadTexture("../Jogo/Textures/Macaco/macacoRight.png");
	GLuint texID_macacoL = loadTexture("../Jogo/Textures/Macaco/macacoLeft.png");
	GLuint texID_banana = loadTexture("../Jogo/Textures/banana/banana.png");
	GLuint texID_placar_zero = loadTexture("../Jogo/Textures/Placar/0.png");
	GLuint texID_placar_um = loadTexture("../Jogo/Textures/Placar/1.png");
	GLuint texID_placar_dois = loadTexture("../Jogo/Textures/Placar/2.png");
	GLuint texID_placar_tres = loadTexture("../Jogo/Textures/Placar/3.png");
	GLuint texID_startButton = loadTexture("../Jogo/Textures/Botoes/start.png");
	GLuint texID_playAgainButton = loadTexture("../Jogo/Textures/Botoes/playAgain.png");
	GLuint texID_exitButton = loadTexture("../Jogo/Textures/Botoes/exit.png");

	// Criação dos sprites utilizando as texturas carregadas
	Sprite background; // Sprite para o fundo do jogo
	background.setShader(shaderID);
	background.inicializar(texID_backGround, vec3(400.0f, 300.0f, 0.0f), vec3(1920.0f / 2.0f, 1200.0f / 2.0f, 1.0f));

	Sprite coco_1; // Sprite para a coco 1
	coco_1.setShader(shaderID);
	coco_1.inicializar(texID_coco_1, vec3(400.0f, 900.0f, 0.0f), vec3(32.0f, 32.0f, 1.0f));

	Sprite coco_2; // Sprite para a coco 2
	coco_2.setShader(shaderID);
	coco_2.inicializar(texID_coco_2, vec3(600.0f, 1080.0f, 0.0f), vec3(32.0f, 32.0f, 1.0f));

	Sprite banana; // Sprite para a banana
	banana.setShader(shaderID);
	banana.inicializar(texID_banana, vec3(200.0f, 1200.0f, 0.0f), vec3(45.0f, 52.0f, 1.0f));

	// Configura e inicializa o sprite do macaco
	macaco.setShader(shaderID);
	macaco.inicializar(texID_macacoR, vec3(300.0f, 120.0f, 0.0f), vec3(65.0f, 70.0f, 1.0f));

	// Sprite para a tela de Game Over
	Sprite gameOver;
	gameOver.setShader(shaderID);
	gameOver.inicializar(texID_gameOver, vec3(400.0f, 300.0f, 0.0f), vec3(350.0f, 205.0f, 1.0f));

	// Sprite para a tela de vitória
	Sprite vitoria;
	vitoria.setShader(shaderID);
	vitoria.inicializar(texID_vitoria, vec3(400.0f, 300.0f, 0.0f), vec3(350.0f, 205.0f, 1.0f));

	// Sprite para o placar inicial
	placar.setShader(shaderID);
	placar.inicializar(texID_placar_zero, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	// Sprite para o placar com zero pontos
	placarZero.setShader(shaderID);
	placarZero.inicializar(texID_placar_zero, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	// Sprites para cada pontuação do placar
	Sprite placarUm;
	placarUm.setShader(shaderID);
	placarUm.inicializar(texID_placar_um, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	Sprite placarDois;
	placarDois.setShader(shaderID);
	placarDois.inicializar(texID_placar_dois, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	Sprite placarTres;
	placarTres.setShader(shaderID);
	placarTres.inicializar(texID_placar_tres, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	// Criação dos botões de menu
	startButton.setShader(shaderID);
	startButton.inicializar(texID_startButton, vec3(400.0f, 300.0f, 0.0f), vec3(212.0f, 78.0f, 1.0f));

	playAgainButton.setShader(shaderID);
	playAgainButton.inicializar(texID_playAgainButton, vec3(200.0f, 50.0f, 0.0f), vec3(200.0f, 80.0f, 1.0f));

	exitButton.setShader(shaderID);
	exitButton.inicializar(texID_exitButton, vec3(600.0f, 50.0f, 0.0f), vec3(200.0f, 80.0f, 1.0f));

	// Define a velocidade de queda para cada sprite que precisa de movimento
	coco_1.setFallSpeed(0.12f);
	coco_2.setFallSpeed(0.12f);
	banana.setFallSpeed(0.1f);

	// Ativa o programa de shader para enviar a matriz de projeção
	glUseProgram(shaderID);

	// Envia a matriz de projeção para o shader
	GLint projLoc = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

	// Configura o buffer de textura no shader
	GLint texBufferLoc = glGetUniformLocation(shaderID, "texBuffer");
	glUniform1i(texBufferLoc, 0);

	// Loop principal do jogo
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // Processa todos os eventos de entrada

		// Calcula o tempo decorrido desde o último quadro
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Atualiza o estado do jogo se estiver jogando e não for Game Over ou Vitória
		if (gameState == 1 && (gmOver == 0) && (pontos < quantPont))
		{
			float macacoSpeed = 200.0f; // Velocidade de movimento do macaco

			// Verifica se a tecla D ou a seta para a direita foi pressionada
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				macaco.switchSide(texID_macacoR);		   // Muda a textura do macaco para a direção direita
				macaco.moveRight(macacoSpeed * deltaTime); // Move o macaco para a direita
			}
			// Verifica se a tecla A ou a seta para a esquerda foi pressionada
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				macaco.switchSide(texID_macacoL);		  // Muda a textura do macaco para a direção esquerda
				macaco.moveLeft(macacoSpeed * deltaTime); // Move o macaco para a esquerda
			}
		}

		// Obtém as dimensões atuais da janela
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height); // Define a área de visualização

		// Limpa o buffer de cores
		glClear(GL_COLOR_BUFFER_BIT);

		// Ativa o programa de shader
		glUseProgram(shaderID);

		// Desenha a cena de acordo com o estado atual do jogo
		if (gameState == 0) // Menu inicial
		{
			background.desenhar();	// Desenha o fundo
			startButton.desenhar(); // Desenha o botão de início
		}
		else if (gameState == 1) // Jogo em andamento
		{
			background.desenhar(); // Desenha o fundo
			coco_1.desenhar();   // Desenha a coco 1
			coco_2.desenhar();   // Desenha a coco 2
			banana.desenhar();	   // Desenha a banana
			placar.desenhar();	   // Desenha o placar
			macaco.desenhar();	   // Desenha o sprite do macaco

			// Atualiza a posição dos sprites que caem se o jogo não estiver em estado de Game Over e não alcançou a pontuação máxima
			if ((gmOver == 0) && (pontos < quantPont))
			{
				coco_1.cair(false);
				coco_2.cair(false);
				banana.cair(false);
			}

			// Verifica colisão entre o macaco e os objetos
			bool auxcoco_1 = colisao(macaco.GetXPos(), macaco.GetYPos(), 65.0f, 70.0f,
									   coco_1.GetXPos(), coco_1.GetYPos(), 32.0f, 32.0f);
			bool auxcoco_2 = colisao(macaco.GetXPos(), macaco.GetYPos(), 65.0f, 70.0f,
									   coco_2.GetXPos(), coco_2.GetYPos(), 32.0f, 32.0f);
			bool aux_banana = colisao(macaco.GetXPos(), macaco.GetYPos(), 65.0f, 70.0f,
									   banana.GetXPos(), banana.GetYPos(), 45.0f, 52.0f);

			if (aux_banana) // Se colidir com a banana
			{
				pontos++; // Incrementa a pontuação

				// Atualiza o sprite do placar de acordo com a pontuação
				switch (pontos)
				{
				case 1:
					placar = placarUm;
					break;
				case 2:
					placar = placarDois;
					break;
				case 3:
					placar = placarTres;
					break;
				}

				if (pontos >= quantPont) // Se atingir a pontuação máxima
				{
					gameState = 3; // Define o estado como Vitória
				}
				else
					banana.cair(true); // Reinicia a posição da banana
			}
			if (auxcoco_1 || auxcoco_2) // Se colidir com as cocos
			{
				gmOver = 1;	   // Define como Game Over
				gameState = 2; // Altera o estado do jogo para Game Over
			}
		}
		else if (gameState == 2) // Tela de Game Over
		{
			background.desenhar();		// Desenha o fundo
			gameOver.desenhar();		// Desenha a tela de Game Over
			playAgainButton.desenhar(); // Desenha o botão de jogar novamente
			exitButton.desenhar();		// Desenha o botão de sair
		}
		else if (gameState == 3) // Tela de Vitória
		{
			background.desenhar();		// Desenha o fundo
			vitoria.desenhar();			// Desenha a tela de vitória
			playAgainButton.desenhar(); // Desenha o botão de jogar novamente
			exitButton.desenhar();		// Desenha o botão de sair
		}

		// Troca os buffers e atualiza a janela
		glfwSwapBuffers(window);
	}

	// Finaliza e libera os recursos do GLFW
	glfwTerminate();
	return 0;
}

// Função para verificar se o botão foi clicado com base nas coordenadas do mouse
bool isButtonClicked(double mouseX, double mouseY, Sprite &button)
{
	// Calcula os limites do botão com base na sua posição e escala
	float xmin = button.GetXPos() - button.GetScale().x / 2.0f; // Limite esquerdo
	float xmax = button.GetXPos() + button.GetScale().x / 2.0f; // Limite direito
	float ymin = button.GetYPos() - button.GetScale().y / 2.0f; // Limite inferior
	float ymax = button.GetYPos() + button.GetScale().y / 2.0f; // Limite superior

	// Verifica se o clique do mouse está dentro dos limites do botão
	return mouseX >= xmin && mouseX <= xmax && mouseY >= ymin && mouseY <= ymax;
}

// Função de callback para eventos de clique de mouse
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	// Obtém a posição atual do cursor
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Ajusta as coordenadas para o sistema de coordenadas do OpenGL
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ypos = height - ypos; // Inverte o eixo Y para correspondência com o OpenGL

	// Verifica se o botão esquerdo do mouse foi pressionado
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (gameState == 0) // Verifica se está na tela de menu
		{
			// Verifica se o botão de início foi clicado
			if (isButtonClicked(xpos, ypos, startButton))
			{
				gameState = 1; // Inicia o jogo
			}
		}
		else if (gameState == 2 || gameState == 3) // Verifica se está na tela de Game Over ou Vitória
		{
			// Verifica se o botão de jogar novamente foi clicado
			if (isButtonClicked(xpos, ypos, playAgainButton))
			{
				// Reinicia as variáveis do jogo para começar de novo
				pontos = 0;
				gmOver = 0;
				macaco.SetPosition(vec3(300.0f, 120.0f, 0.0f));
				placar = placarZero;
				gameState = 1; // Retorna ao estado de jogo em andamento
			}
			// Verifica se o botão de sair foi clicado
			else if (isButtonClicked(xpos, ypos, exitButton))
			{
				glfwSetWindowShouldClose(window, GL_TRUE); // Fecha o jogo
			}
		}
	}
}

// Função de callback para eventos de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // Verifica se a tecla ESC foi pressionada
		glfwSetWindowShouldClose(window, GL_TRUE);		// Fecha a janela
}

// Função para verificar colisão entre dois retângulos
bool colisao(float perX, float perY, float widthPer, float heightPer, float obsX, float obsY, float widthObs, float heightObs)
{
	// Posições dos vértices do personagem
	float xmaxPer = perX + widthPer / 3.0f;
	float ymaxPer = perY + heightPer / 3.0f;
	float xminPer = perX - widthPer / 3.0f;
	float yminPer = perY - heightPer / 3.0f;

	// Posições dos vértices do obstáculo
	float xmaxObs = obsX + widthObs / 3.0f;
	float ymaxObs = obsY + heightObs / 3.0f;
	float xminObs = obsX - widthObs / 3.0f;
	float yminObs = obsY - heightObs / 3.0f;

	// Testes de colisão nos eixos X e Y
	bool collisionX = xmaxPer >= xminObs && xmaxObs >= xminPer;
	bool collisionY = ymaxPer >= yminObs && ymaxObs >= yminPer;

	return collisionX && collisionY; // Retorna verdadeiro se houver colisão
}

// Função para carregar uma textura de uma imagem usando STB Image
GLuint loadTexture(const string &texturePath)
{
	GLuint texID;

	// Gera um identificador de textura
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Parâmetros da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	  // Envelopamento na coordenada S
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	  // Envelopamento na coordenada T
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtro para minimização
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtro para magnificação

	int width, height, nrChannels;
	unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0); // Carrega a imagem

	// Carrega a textura de acordo com o número de canais
	if (data)
	{
		if (nrChannels == 3) // Para imagens JPG, BMP (RGB)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) // Para imagens PNG (RGBA)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			cout << "Número desconhecido de canais: " << nrChannels << endl;
		}
		glGenerateMipmap(GL_TEXTURE_2D); // Gera mipmaps para a textura
	}
	else
	{
		cout << "Erro ao carregar a textura: " << texturePath << endl;
	}

	stbi_image_free(data);			 // Libera a memória usada para a imagem
	glBindTexture(GL_TEXTURE_2D, 0); // Desvincula a textura

	return texID; // Retorna o identificador da textura
}

// Função para configurar e compilar os shaders (Vertex e Fragment)
GLuint setupShader()
{
	// Compila os shaders
	GLuint vertexShader, fragmentShader;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Verifica erros de compilação no Vertex Shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		cout << "ERRO::SHADER::VERTEX::COMPILACAO_FALHOU\n"
			 << infoLog << endl;
	}

	// Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Verifica erros de compilação no Fragment Shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		cout << "ERRO::SHADER::FRAGMENT::COMPILACAO_FALHOU\n"
			 << infoLog << endl;
	}

	// Linka os shaders no programa
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Verifica erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		cout << "ERRO::SHADER::PROGRAMA::LINKAGEM_FALHOU\n"
			 << infoLog << endl;
	}

	// Deleta os shaders após o link
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram; // Retorna o identificador do programa de shaders
}
