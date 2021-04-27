// Required for OpenGL, glad must be included before glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL Math libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// For loading textures, #define STB_IMAGE_IMPLEMENTATION
// must be defined.
#define STB_IMAGE_IMPLEMENTATION
#include "Utility/stb_image.h"

// STL libraries
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

// My headers
#include "Utility/Shader.h"
#include "Utility/TextureLoader.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include "KeyboardHandler.h"

#include "Render/Camera.h"

#include "Debug.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;

constexpr const char* vertexShaderPath = "shaders/vertexShader.glsl";
constexpr const char* fragmentShaderPath = "shaders/fragmentShader.glsl";

constexpr const char* texturePath1 = "res/pattern.jpg";
constexpr const char* texturePath2 = "res/awesome.png";

unsigned int* textures[2];
float textureAlpha = 0.5f;

Shader* shader = nullptr;

// Projection type
glm::mat4 proj;

// Model matrix
glm::mat4 model;

// View matrix
glm::mat4 view;

VertexBuffer* buffer;
VertexArray* array;
ElementBuffer* element;

// Vertex stuff
/*
float verticiesTriangle1[] = {
	// Positions		// Colours			// Texture Coordinates
	-.6f, -.5f, .0f,	1.f, 0.f, 0.f,		0.f, 0.f,
	 .4f, -.5f, .0f,	0.f, 1.f, 0.f,		1.f, 0.f,
	-.6f,  .5f, .0f,	0.f, 0.f, 1.f,		.5f, 1.f
};

float verticiesTriangle2[] = {
	 .6f, -.5f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
	 .6f,  .5f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f,
	-.4f,  .5f, 0.f, 0.f, 0.f, 1.f, .5f, 1.f
};
*/

float rectangle[] =
{	// Positions				// Texture Coordinates
	 0.5f,  0.5f,  0.0f,		1.0f, 1.0f,	// Top right
	 0.5f, -0.5f,  0.0f,		1.0f, 0.0f, // Bottom right
	-0.5f, -0.5f,  0.0f,		0.0f, 0.0f, // Bottom left
	-0.5f,  0.5f,  0.0f,		0.0f, 1.0f // Top left
};

unsigned int indicies[] =
{
	0, 1, 3, // First triangle
	1, 2, 3 // Second triangle
};

float cube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

std::vector<glm::vec3> cubePositions =
{
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

// Keyboard stuff
KeyboardHandler* keyboardHandler;
void bindKeys();

bool showWireFrame = false;

Camera camera(glm::vec3(0.f, 0.f, 4.f));

float yaw = 0;
float pitch = 0;
bool firstMouse = true;

// Handlers
void toggleWireframe();
void increaseAlpha();
void decreaseAlpha();

// Mouse stuff
float lastX = 400;
float lastY = 300;

// For GLFW
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double mouseX, double mouseY);
//void processInput(GLFWwindow* window);

void update(double deltaTime)
{
	camera.update(deltaTime);
	view = camera.getViewMatrix();
}

void render(GLFWwindow* window)
{
	glClearColor(.2f, .3f, .3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	array->bindTextures();

	shader->use();
	shader->setFloat("alpha", textureAlpha);	
	
	shader->setMat4fv("view", view);
	shader->setMat4fv("projection", proj);
	
	int i = 0;
	for (const auto& pos : cubePositions)
	{
		glm::mat4 m(1.0f);
		m = glm::translate(m, pos);

		const float angle = 20.f * i;
		m = glm::rotate(m, (glm::radians(angle) + 1), glm::vec3(1.0f, 1.0f, 1.0f));
		m = glm::rotate(m, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));
		
		shader->setMat4fv("model", m);

		array->render();
		++i;
	}
	
	glfwSwapBuffers(window);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", nullptr, nullptr);
	if (window == nullptr) 
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return EXIT_FAILURE;
	}

	keyboardHandler = new KeyboardHandler(window);
	bindKeys();
	
	glViewport(0, 0, windowWidth, windowHeight);

	proj = glm::perspective(glm::radians(90.0f), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 100.0f);
	
	// Load textures
	textures[0] = TextureLoader::loadTexture(texturePath1, false);
	textures[1] = TextureLoader::loadTexture(texturePath2, true);

	// Create shaders
	shader = new Shader(vertexShaderPath, fragmentShaderPath);
	shader->setInt("texture0", 0);
	shader->setInt("texture1", 1);
	
	array = new VertexArray();
	buffer = new VertexBuffer(cube, sizeof(cube));
	//element = new ElementBuffer(indicies, sizeof(indicies));
	
	array->setAttributes({ 3, 2 });
	array->addTexture(textures[0], GL_TEXTURE0);
	array->addTexture(textures[1], GL_TEXTURE1);

	glEnable(GL_DEPTH_TEST);
	
	const int fps = 60; 
	const double targetTime = 1.0 / fps;
	
	double lastTime = glfwGetTime();
	double timer = lastTime;
	double currentTime = 0;
	double deltaTime = 0;
	int frames = 0;
	int updates = 0;

	LOG("Starting main loop.");
	while (!glfwWindowShouldClose(window)) 
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.moveForward();
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.moveBackward();
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.moveRight();
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.moveLeft();
		
		keyboardHandler->processInput(); //processInput(window);
		
		// Measure time
		currentTime = glfwGetTime();
		deltaTime += (currentTime - lastTime) / targetTime;
		lastTime = currentTime;

		// Update at 60fps
		while (deltaTime >= 1.0) {
			update(deltaTime);
			updates++;
			deltaTime--;
		}

		// Render as many frames as possible
		render(window);
		frames++;

		// Reset update and frame count after 1 second
		if (glfwGetTime() - timer > 1.0) {
			timer++;
			std::cout << updates << " updates at " << frames << "fps" << std::endl;
			updates = 0;
			frames = 0;
		}

		glfwPollEvents();
	}

	delete shader;
	TextureLoader::unloadTexture(textures[0]);

	//delete element;
	delete buffer;
	delete array;

	delete keyboardHandler;
	
	glfwTerminate();

	return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double mouseX, double mouseY)
{
	camera.mouseCallback(window, mouseX, mouseY);
}

void bindKeys()
{
	keyboardHandler->bindKey(GLFW_KEY_T, &toggleWireframe, nullptr, false);
}

void toggleWireframe()
{
	showWireFrame = !showWireFrame;
	showWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void increaseAlpha()
{
	textureAlpha += 0.1f;
}

void decreaseAlpha()
{
	textureAlpha -= 0.1f;
}

/* OLD VAO & VBO CODE FOR REFERENCE
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticiesTriangle2), verticiesTriangle2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
*/
