#include "Camera.h"

#include <algorithm>

#include "../Input/KeyboardHandler.h"

Camera::Camera()
{
	Camera(glm::vec3(0.0f, 0.0f, 0.0f));
}

Camera::Camera(glm::vec3 position)
{
	Camera(position, glm::vec3(0.0f, 0.0f, -1.0f));
}

Camera::Camera(glm::vec3 position, glm::vec3 front) :
	position(position),
	front(front),
	pitch(0),
	yaw(0),
	roll(0),
	mouseUsed(false)
{
	this->updateVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(this->position, this->position + this->front, Camera::WORLD_UP);
}

void Camera::handleMouseCallback(GLFWwindow* window, double mx, double my)
{	
	if(!this->mouseUsed)
	{
		this->prevMx = mx;
		this->prevMy = my;
		this->mouseUsed = true;
	}

	float xOffset = mx - this->prevMx;
	float yOffset = this->prevMy - my;
	this->prevMx = mx;
	this->prevMy = my;

	xOffset *= this->sensitivity;
	yOffset *= this->sensitivity;

	this->yaw += xOffset;
	this->pitch = std::clamp<float>(this->pitch + yOffset, -1 * MAX_YAW, MAX_YAW);

	this->updateVectors();
}

void Camera::update(double deltaTime)
{
	this->velocity = this->moveSpeed * deltaTime;
}

void Camera::updateVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(direction);
	this->right = glm::normalize(glm::cross(this->front, Camera::WORLD_UP));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::moveForward()
{
	this->position += this->velocity * this->front;
}

void Camera::moveBackward()
{
	this->position -= this->velocity * this->front;
}

void Camera::moveRight()
{
	this->position += this->velocity * this->right;
}

void Camera::moveLeft()
{
	this->position -= this->velocity * this->right;
}
