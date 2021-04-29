#include "Application.h"

#include "Render/VertexArray.h"
#include "Render/Camera.h"

#include "Utility/Shader.h"
#include "Utility/TextureLoader.h"

#include "Input/Mouse.h"
#include "Input/KeyboardHandler.h"

#include <iostream>

Application::Application()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if(!this->window)
	{
		glfwTerminate();
		throw std::exception("ERROR: Failed to create GLFW window");
	}

	glfwMakeContextCurrent(this->window);

	this->initViewport();

	if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		throw std::exception("ERROR: Failed to initialize GLAD!");
	}

	this->initKeyboardHandler();
	this->initTextures();
	this->initShader();

	this->vArray = std::make_unique<VertexArray>();
	this->vBuffer = std::make_unique<VertexBuffer>(cube, sizeof(cube));
	//this->eBuffer = std::make_unique<ElementBuffer>(indicies, sizeof(indicies));

	this->vArray->setAttributes({ 3, 2 });
	this->vArray->addTexture(this->texture, GL_TEXTURE0);
	
	this->projectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100);

	glEnable(GL_DEPTH_TEST);

	this->start();
}

Application::~Application()
{
	TextureLoader::unloadTexture(texture);
}

void Application::update(double deltaTime)
{
	this->camera->update(deltaTime);
	this->viewMatrix = this->camera->getViewMatrix();
}

void Application::render(GLFWwindow* window)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->vArray->bindTextures();

	this->shader->use();
	this->shader->setMat4fv("view", this->viewMatrix);
	this->shader->setMat4fv("projection", this->projectionMatrix);

	int i = 0;
	for (const auto& pos : cubePositions)
	{
		glm::mat4 m(1.0f);
		m = glm::translate(m, pos);

		const float angle = 20.f * i;
		m = glm::rotate(m, (glm::radians(angle) + 1), glm::vec3(1.0f, 1.0f, 1.0f));
		m = glm::rotate(m, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));

		this->shader->setMat4fv("model", m);

		this->vArray->render();
		++i;
	}
	
	glfwSwapBuffers(this->window);
}

void Application::start()
{
	double lastTime = glfwGetTime();
	double timer = lastTime;
	double currentTime = 0;
	double deltaTime = 0;

	int frames = 0;
	int updates = 0;

	while(!glfwWindowShouldClose(this->window))
	{
		this->handleEvents(this->window);

		currentTime = glfwGetTime();
		deltaTime += (currentTime - lastTime) / TARGET_TIME;
		lastTime = currentTime;

		while(deltaTime >= 1.0)
		{
			this->update(deltaTime);
			updates++;
			deltaTime--;
		}

		this->render(window);
		frames++;

		if(glfwGetTime() - timer > 1.0)
		{
			timer++;
			std::cout << updates << " updates at " << frames << "fps" << std::endl;
			updates = 0;
			frames = 0;
		}

		glfwPollEvents();
	}

	glfwTerminate();
}

void Application::initViewport()
{
	int width, height;
	glfwGetFramebufferSize(this->window, &width, &height);
	glViewport(0, 0, width, height);
}

void Application::initKeyboardHandler()
{
	this->keyboardHandler = std::make_unique<KeyboardHandler>(this->window);
	/*
	keyboardHandler->bindKey(GLFW_KEY_W, false, std::bind(&Camera::moveForward, &camera), nullptr);
	keyboardHandler->bindKey(GLFW_KEY_S, false, std::bind(&Camera::moveBackward, &camera), nullptr);
	keyboardHandler->bindKey(GLFW_KEY_D, false, std::bind(&Camera::moveRight, &camera), nullptr);
	keyboardHandler->bindKey(GLFW_KEY_A, false, std::bind(&Camera::moveLeft, &camera), nullptr);
	keyboardHandler->bindKey(GLFW_KEY_T, true, toggleWireframe, nullptr);*/
}

void Application::initTextures()
{
	this->texture = TextureLoader::loadTexture(texturePath1, false);
}

void Application::initShader()
{
	this->shader = std::make_unique<Shader>(vShaderPath, fShaderPath);
	this->shader->setInt("shaderTexture", 0);
}
