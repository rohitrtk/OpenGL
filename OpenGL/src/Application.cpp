#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Utility/stb_image.h"

#include "Application.h"

#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Render/ElementBuffer.h"
#include "Render/Camera.h"

#include "Utility/Shader.h"
#include "Utility/TextureLoader.h"

#include "Input/KeyboardHandler.h"
#include "Input/Mouse.h"

#include <iostream>

Application::Application()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
	if(!this->window)
	{
		glfwTerminate();
		throw std::exception("ERROR: Failed to create GLFW window");
	}

	glfwMakeContextCurrent(this->window);

	if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		throw std::exception("ERROR: Failed to initialize GLAD!");
	}

	this->initViewport();
	this->initTextures();
	this->initShader();
	this->initCamera();
	this->initKeyboardHandler();
	this->initMouse();

	this->vArray = std::make_unique<VertexArray>();
	this->vBuffer = std::make_unique<VertexBuffer>(appCube, sizeof(appCube));
	//this->eBuffer = std::make_unique<ElementBuffer>(indicies, sizeof(indicies));

	this->vArray->setAttributes({ 3, 2 });
	this->vArray->addTexture(this->texture, GL_TEXTURE0);
	
	this->projectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 100.0f);

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
	for (const auto& pos : appCubePositions)
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

void Application::handleEvents(GLFWwindow* window)
{
	this->keyboardHandler->processInput();
	this->processMouse();
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
		if(glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(this->window, true);
		}
		
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

void Application::initCamera()
{
	this->camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));
}

void Application::initMouse()
{
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	this->mouse = std::make_unique<Mouse>();
	this->mouse->x = 400.0;
	this->mouse->y = 300.0;
	this->mouse->px = 0;
	this->mouse->py = 0;
	
	this->mouse->firstUse = false;
}

void Application::processMouse()
{
	glfwGetCursorPos(this->window, &this->mouse->x, &this->mouse->y);
	this->camera->handleMouse(*this->mouse);
	//this->camera->handleMouseCallback(this->window, this->mouse->x, this->mouse->y);
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
	keyboardHandler->bindKey(GLFW_KEY_W, false, std::bind(&Camera::moveForward, this->camera.get()), nullptr);
	keyboardHandler->bindKey(GLFW_KEY_S, false, std::bind(&Camera::moveBackward, this->camera.get()), nullptr);
	keyboardHandler->bindKey(GLFW_KEY_D, false, std::bind(&Camera::moveRight, this->camera.get()), nullptr);
	keyboardHandler->bindKey(GLFW_KEY_A, false, std::bind(&Camera::moveLeft, this->camera.get()), nullptr);
	keyboardHandler->bindKey(GLFW_KEY_T, true, std::bind(&Application::toggleWireframe, this), nullptr);
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

void Application::toggleWireframe()
{
	this->showWireframe = !this->showWireframe;
	this->showWireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

