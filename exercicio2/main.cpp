/* Hello Triangle - cÛdigo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr·fico - Unisinos
 * Vers„o inicial: 7/4/2017
 * ⁄ltima atualizaÁ„o em 14/08/2023
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sprite.h"

//STB IMAGE
#include <stb_image/stb_image.h>

using namespace std;
//using namespace glm; //para n„o usar glm::

//Classe para manipulaÁ„o dos shaders
#include "shader.h"

// ProtÛtipo da funÁ„o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// ProtÛtipos das funÁıes
GLuint loadTexture(string texturePath);

// Dimensıes da janela (pode ser alterado em tempo de execuÁ„o)
const GLuint WIDTH = 1200, HEIGHT = 1000;


//Variaveis globais
Sprite spr;

enum GameState {
    START_SCREEN,
    GAMEPLAY
};

GameState currentState = START_SCREEN;

// FunÁ„o MAIN
int main()
{
	
	// InicializaÁ„o da GLFW
	glfwInit();

	// InicializaÁ„o da semente para geraÁ„o de nros aleatÛrios
	srand((int)glfwGetTime());

	//Muita atenÁ„o aqui: alguns ambientes n„o aceitam essas configuraÁıes
	//VocÍ deve adaptar para a vers„o do OpenGL suportada por sua placa
	//Sugest„o: comente essas linhas de cÛdigo para desobrir a vers„o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho GA", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
	
	//Habilitar teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS); //a cada ciclo

	//Habilitar o modo de transparância (blend - mistura de cores)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Compilando e buildando o programa de shader
	//Shader shader("../shaders/helloTriangle.vs", "../shaders/helloTriangle.fs");
	Shader shader("../shaders/tex.vs", "../shaders/tex.fs");

	// Textura de botoes
	GLuint startTexID = loadTexture("../Textures/atributos/start.png"); // Nova textura para o botão "start"

	// Textura do fundo
	GLuint fundoTextID = loadTexture("../Textures/background/fundo-jardim.png");

	// Textura do personagem
	GLuint meninaTextID = loadTexture("../Textures/personagens/menina.png");
	
	// Textura dos itens
	GLuint bombaTextID = loadTexture("../Textures/itens/bomba.png");
	GLuint bananaTextID = loadTexture("../Textures/itens/banana.png");
	GLuint boloTextID = loadTexture("../Textures/itens/bolo.png");
	GLuint macaTextID = loadTexture("../Textures/itens/maca.png");
	GLuint sanduicheTextID = loadTexture("../Textures/itens/sanduiche.png");
	GLuint sucoTextID = loadTexture("../Textures/itens/suco.png");
	GLuint lataLixoTextID = loadTexture("../Textures/itens/lata-lixo.png");
	GLuint macaLixoTextID = loadTexture("../Textures/itens/maca-lixo.png");
	
	//Criação de uma sprite
	spr.setShader(&shader);
	spr.inicializar(meninaTextID, glm::vec3(400.0, 150.0, 0.0), glm::vec3(128, 128, 1.0));
	
	// Sprite do background
	Sprite background;
	background.setShader(&shader);
	background.inicializar(fundoTextID, glm::vec3(400.0, 300.0, 0.0), glm::vec3(1920.0/2.0, 1080.0/2.0, 1.0));

	// Sprite dos botoes
	Sprite startButton;
    startButton.setShader(&shader);
    startButton.inicializar(startTexID, glm::vec3(400.0, 300.0, 0.0), glm::vec3(200.0, 100.0, 1.0)); // Ajuste a posição e escala conforme necessário

	// Sprite dos itens que dao ponto
	Sprite banana;
	banana.setShader(&shader);
	banana.inicializar(bananaTextID, glm::vec3(400.0, 900.0, 0.0), glm::vec3(32.0, 32.0, 1.0));

	Sprite bolo;
	bolo.setShader(&shader);
	bolo.inicializar(boloTextID, glm::vec3(400.0, 900.0, 0.0), glm::vec3(32.0, 32.0, 1.0));

	Sprite maca;
	maca.setShader(&shader);
	maca.inicializar(macaTextID, glm::vec3(400.0, 900.0, 0.0), glm::vec3(32.0, 32.0, 1.0));

	Sprite sanduiche;
	sanduiche.setShader(&shader);
	sanduiche.inicializar(sanduicheTextID, glm::vec3(400.0, 900.0, 0.0), glm::vec3(32.0, 32.0, 1.0));

	Sprite suco;
	suco.setShader(&shader);
	suco.inicializar(sucoTextID, glm::vec3(400.0, 900.0, 0.0), glm::vec3(32.0, 32.0, 1.0));

	// Sprite itens que dao dano
	Sprite bomba;
	bomba.setShader(&shader);
	bomba.inicializar(bombaTextID, glm::vec3(400.0, 900.0, 0.0), glm::vec3(32.0, 32.0, 1.0));

	Sprite lataLixo;
	lataLixo.setShader(&shader);
	lataLixo.inicializar(lataLixoTextID, glm::vec3(400.0, 900.0, 0.0), glm::vec3(32.0, 32.0, 1.0));

	Sprite macaLixo;
	macaLixo.setShader(&shader);
	macaLixo.inicializar(macaLixoTextID, glm::vec3(400.0, 900.0, 0.0), glm::vec3(32.0, 32.0, 1.0));

	//Ativando o buffer de textura 0 da opengl
	glActiveTexture(GL_TEXTURE0);

	shader.Use();

	//Matriz de projeção paralela ortográfica
	glm::mat4 projection = glm::ortho(0.0,800.0,0.0,600.0,-1.0,1.0);
	//Enviando para o shader a matriz como uma var uniform
	shader.setMat4("projection", glm::value_ptr(projection));
	
	//Matriz de transformação do objeto (matriz de modelo)
	shader.setInt("texBuffer", 0);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height); //unidades de tela: pixel
		
		// Limpa o buffer de cor
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		if (currentState == START_SCREEN) {
            background.desenhar();
            startButton.desenhar();
        } else if (currentState == GAMEPLAY) {
            background.desenhar();
            spr.desenhar();
            bomba.cair();
            bomba.desenhar();
			banana.cair();
			banana.desenhar();
        }

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
		if (currentState == START_SCREEN) {
            currentState = GAMEPLAY;
		}

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		spr.moverParaDireita();
	}
	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		spr.moverParaEsquerda();
	}
	
}

GLuint loadTexture(string texturePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Configuração do parâmetro WRAPPING nas coords s e t
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Confirugação do parâmetro FILTERING na minificação e magnificação da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}