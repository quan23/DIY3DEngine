#include "World.h"
#include "Window.h"
#include "Camera.h"

Camera::Camera(Window& window, glm::vec3 Position) : window(&window), Position(Position)
{
}

void Camera::cMatrix(float Fov, float fNear, float fFar, GLuint uniformID)
{
	glm::mat4 tran = glm::lookAt(Position, Position + Orientation, Up);
	glm::mat4 proj = glm::perspective(glm::radians(Fov), float(window->getWidth())/float(window->getHeight()), fNear, fFar);
	glm::mat4 view = proj * tran;
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, &view[0][0]);
}

void Camera::cInput(double fram)
{
	Move = glm::vec3(0.0f);
	float step = Speed * fram;
	if (glfwGetKey(window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		Move += glm::normalize(glm::vec3(Orientation.x,0.0f, Orientation.z));
	}
	if (glfwGetKey(window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		Move -= glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z)) ;
	}
	if (glfwGetKey(window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		Move -= glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		Move += glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		Move -= Up;
	}
	if (glfwGetKey(window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Move += Up;
	}
	if (glm::length(Move)!=0) Position += glm::normalize(Move)*step;
	WorldCoor = World::inWhatChunk(int(Position.x), int(Position.y), int(Position.z));
	if (glfwGetMouseButton(window->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (firstClick)
		{
			glfwSetCursorPos(window->getWindow(), window->getWidth() / 2, window->getHeight() / 2);
			firstClick = false;
		}
		double mouseX, mouseY;
		glfwGetCursorPos(window->getWindow(), &mouseX, &mouseY);
		mouseX = Xsensitivity * ((int)mouseX - window->getWidth() / 2);
		mouseY = Ysensitivity * ((int)mouseY - window->getHeight() / 2);
		Yaw -= mouseX;
		Pitch -= mouseY;
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
		Orientation.x = -sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Orientation.y = sin(glm::radians(Pitch));
		Orientation.z = -cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Orientation = glm::normalize(Orientation);
		glfwSetCursorPos(window->getWindow(), window->getWidth() / 2, window->getHeight() / 2);
	}
	else
	{
		firstClick = true;
		glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

}
