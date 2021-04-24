#include "Utility/Shader.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "Utility/stb_image.h"

#include <iostream>
#include <algorithm>
#include <cmath>

#include "Debug.h"

#include "Utility/TextureLoader.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;

constexpr const char* vertexShaderPath = "shaders/vertexShader.glsl";
constexpr const char* fragmentShaderPath = "shaders/fragmentShader.glsl";

constexpr const char* texturePath1 = "res/brick.png";
constexpr const char* texturePath2 = "res/awesome.png";

unsigned int* textures[2];
float textureAlpha = 0.5f;

Shader* shader = nullptr;

VertexBuffer* buffer;
VertexArray* array;
ElementBuffer* element;

// Keyboard stuff
bool tPressed = false;
bool wPressed = false;
bool sPressed = false;

bool showWireFrame = false;

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void update(GLFWwindow* window) {}

void render(GLFWwindow* window, const double deltaTime)
{
	glClearColor(.2f, .3f, .3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	shader->use();
	shader->setFloat("alpha", textureAlpha);

	array->render();

	glfwSwapBuffers(window);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	if (window == NULL) 
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, windowWidth, windowHeight);

	// Create shaders
	shader = new Shader(vertexShaderPath, fragmentShaderPath);

	// Load textures
	textures[0] = TextureLoader::loadTexture(texturePath1, false);
	textures[1] = TextureLoader::loadTexture(texturePath2, true);

	array = new VertexArray();
	buffer = new VertexBuffer(rectangle, sizeof(rectangle));
	element = new ElementBuffer(indicies, sizeof(indicies));
	
	array->setAttributes(*buffer, { 3, 2 });
	array->addTexture(textures[0], GL_TEXTURE0);
	array->addTexture(textures[1], GL_TEXTURE1);
	
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
		processInput(window);
		
		// Measure time
		currentTime = glfwGetTime();
		deltaTime += (currentTime - lastTime) / targetTime;
		lastTime = currentTime;

		// Update at 60fps
		while (deltaTime >= 1.0) {
			update(window);
			updates++;
			deltaTime--;
		}

		// Render as many frames as possible
		render(window, deltaTime);
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

	delete element;
	delete buffer;
	delete array;

	glfwTerminate();

	return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !tPressed)
	{
		tPressed = true;
		showWireFrame = !showWireFrame;
		showWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && tPressed)
	{
		tPressed = false;
	}

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !wPressed)
	{
		wPressed = true;
		textureAlpha = textureAlpha + 0.1f; // std::min<float>(textureAlpha + 0.1f, 1.0f);
		LOG(textureAlpha);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && wPressed)
	{
		wPressed = false;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !sPressed)
	{
		sPressed = true;
		textureAlpha = textureAlpha - 0.1f; // std::max<float>(textureAlpha - 0.1f, 0.f);
		LOG(textureAlpha);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && sPressed)
	{
		sPressed = false;
	}
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
