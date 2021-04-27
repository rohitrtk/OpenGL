#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class KeyboardHandler;

class Camera
{
public:

	inline static const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
	
	Camera();
	Camera(glm::vec3 position);
	Camera(glm::vec3 position, glm::vec3 direction);
	
	void update(double deltaTime);

	void setPositionVector(glm::vec3 position) { this->position = position; }
	glm::vec3& getPositionVector()	{ return this->position; }

	void setFrontVector(glm::vec3 front) { this->front = front; }
	glm::vec3& getFrontVector() { return this->front; }
	
	glm::mat4 getViewMatrix() const;

	void handleMouseCallback(GLFWwindow* window, double mx, double my);
	
	float moveSpeed = 0.05f;
	
	void moveForward();
	void moveBackward();
	void moveRight();
	void moveLeft();

private:
	glm::vec3 position;
	glm::vec3 front;

	glm::vec3 up;
	glm::vec3 right;
	
	const float MAX_YAW = 180.0f;
	
	float pitch;
	float yaw;
	float roll;

	// Temporarily here until I make a mouse handler
	bool mouseUsed;
	double prevMx;
	double prevMy;
	float sensitivity = 0.1f;

	void updateVectors();

	float velocity;
};

