#include "Shader.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "TextureLoader.h"

#include "VertexArray.h"
#include "VertexBuffer.h"

#include <iostream>
#include <cmath>

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cerr << "OpenGL Error (" << function << ", " 
			<< file << ", " << line << "): " << error << std::endl;
		return false;
	}
	return true;
}

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;

constexpr const char* vertexShaderPath = "shaders/vertexShader.glsl";
constexpr const char* fragmentShaderPath = "shaders/fragmentShader.glsl";

Shader* shader = nullptr;

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

constexpr const char* texturePath = "res/duck.jpg";
GLuint* texture = nullptr;

VertexBuffer* vertexBuffer;
VertexArray* vertexArray;
VertexBuffer* vertexBuffer2;
VertexArray* vertexArray2;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

bool showWireFrame = false;
// Temporary until I have some sort of keyboard handler
bool tPressed = false;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !tPressed) {
		tPressed = true;
		showWireFrame = !showWireFrame;
		showWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) :	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && tPressed) {
		tPressed = false;
	}
}

void update(GLFWwindow* window) {}

void render(GLFWwindow* window, const double deltaTime) {
	glClearColor(.2f, .3f, .3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	float positionOffset = .0f;
	float colourOffset = 0.f;

	shader->use();

	shader->setVec3f("colourOffset", colourOffset, 0, 0);
	shader->setVec3f("positionOffset", positionOffset, 0, 0);

	vertexArray->render();
	vertexArray2->render();

	glfwSwapBuffers(window);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, windowWidth, windowHeight);

	// Create shaders
	shader = new Shader(vertexShaderPath, fragmentShaderPath);

	// Load texture
	texture = TextureLoader::loadTexture(texturePath);
	
	// Create vertex buffer
	vertexBuffer = new VertexBuffer(verticiesTriangle1, sizeof(verticiesTriangle1));
	vertexArray = new VertexArray();
	vertexArray->setTexture(texture);
	vertexArray->setAttributes(*vertexBuffer, {3, 3, 2});

	vertexBuffer2 = new VertexBuffer(verticiesTriangle2, sizeof(verticiesTriangle2));
	vertexArray2 = new VertexArray();
	vertexArray2->setTexture(texture);
	vertexArray2->setAttributes(*vertexBuffer2, { 3, 3, 2 });

	const int fps = 60;
	const double targetTime = 1.0 / fps;
	
	double lastTime = glfwGetTime();
	double timer = lastTime;
	double currentTime = 0;
	double deltaTime = 0;
	int frames = 0;
	int updates = 0;

	while (!glfwWindowShouldClose(window)) {
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
	TextureLoader::unloadTexture(texture);
	
	delete vertexArray;
	delete vertexBuffer;

	glfwTerminate();

	return EXIT_SUCCESS;
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
