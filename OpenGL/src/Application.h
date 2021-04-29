#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>


class Shader;
class VertexBuffer;
class VertexArray;
class ElementBuffer;

struct MousePosition;

class KeyboardHandler;

class Camera;

constexpr const char* vShaderPath = "shaders/vertexShader.glsl";
constexpr const char* fShaderPath = "shaders/fragmentShader.glsl";

constexpr const char* texturePath1 = "res/pattern.jpg";
constexpr const char* texturePath2 = "res/awesome.png";

class Application
{
public:
	inline static int WINDOW_WIDTH = 800;
	inline static int WINDOW_HEIGHT = 600;
	inline static const char* WINDOW_TITLE = "Sandbox";

	inline static int TARGET_FPS = 60;
	inline static const double TARGET_TIME = 1.0 / TARGET_FPS;
	
	Application();
	~Application();
	
	void start();
	
	void update(double deltaTime);
	void render(GLFWwindow* window);
	void handleEvents(GLFWwindow* window);

private:
	GLFWwindow* window;
	
	std::unique_ptr<VertexBuffer> vBuffer;
	std::unique_ptr<VertexArray> vArray;
	std::unique_ptr<ElementBuffer> eBuffer;

	std::unique_ptr<Camera> camera;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;

	void initViewport();

	std::unique_ptr<MousePosition> mouse;
	void initMouse();

	std::unique_ptr<KeyboardHandler> keyboardHandler;
	void initKeyboardHandler();

	std::unique_ptr<Shader> shader;
	void initShader();
	
	// TODO: Change to smart pointer
	unsigned int* texture;
	void initTextures();
};

