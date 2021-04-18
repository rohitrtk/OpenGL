#include "Shader.h"
#include <GLFW/glfw3.h>

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

float verticiesTriangle1[] = {
	-.6f, -.5f, .0f, 1.f, 0.f, 0.f,
	 .4f, -.5f, .0f, 0.f, 1.f, 0.f,
	-.6f,  .5f, .0f, 0.f, 0.f, 1.f
};

float verticiesTriangle2[] = {
	 .6f, -.5f, 0.f, 1.f, 0.f, 0.f,
	 .6f,  .5f, 0.f, 0.f, 1.f, 0.f,
	-.4f,  .5f, 0.f, 0.f, 0.f, 1.f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
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
	Shader shader(vertexShaderPath, fragmentShaderPath);

	GLuint VBO1, VBO2;
	GLuint VAO1, VAO2;

	// VAO and VBO 1
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);

	glBindVertexArray(VAO1);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticiesTriangle1), verticiesTriangle1, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// VAO and VBO 2
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticiesTriangle2), verticiesTriangle2, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(.2f, .3f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		float offset = .7f;

		shader.use();
		shader.setFloat("xOffset", 0);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		shader.setFloat("xOffset", offset);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);

	glfwTerminate();

	return EXIT_SUCCESS;
}
