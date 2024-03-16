#include "World.h"
#include "Camera.h"

camera::camera(int Width, int Height, glm::vec3 Position) : Width(Width), Height(Height), Position(Position)
{
}

worldCoor camera::getWorldCoor() const
{
	return WorldCoor;
}

void camera::cMatrix(float Fov, float fNear, float fFar, GLuint uniformID)
{
	glm::mat4 tran = glm::lookAt(Position, Position + Orientation, Up);
	glm::mat4 proj = glm::perspective(glm::radians(Fov), (float)Width / (float)Height, fNear, fFar);
	glm::mat4 view = proj * tran;
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, &view[0][0]);
}

void camera::cInput(GLFWwindow* window, double fram)
{
	Move = glm::vec3(0.0f);
	float step = Speed * fram;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Move += glm::normalize(glm::vec3(Orientation.x,0.0f, Orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Move -= glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z)) ;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Move -= glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Move += glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		Move -= Up;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Move += Up;
	}
	if (glm::length(Move)!=0) Position += glm::normalize(Move)*step;
	WorldCoor = World::inWhatChunk(int(Position.x), int(Position.y), int(Position.z));
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (firstClick)
		{
			glfwSetCursorPos(window, (double)Width / 2, (double)Height / 2);
			firstClick = false;
		}
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		mouseX = Xsensitivity * (mouseX - (float)Width / 2);
		mouseY = Ysensitivity * (mouseY - (float)Height / 2);
		Yaw -= mouseX;
		Pitch -= mouseY;
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
		Orientation.x = -sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Orientation.y = sin(glm::radians(Pitch));
		Orientation.z = -cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Orientation = glm::normalize(Orientation);
		glfwSetCursorPos(window, (double)Width / 2, (double)Height / 2);
	}
	else
	{
		firstClick = true;
	}

}

void camera::setSpeed(float speed)
{
	this->Speed = speed;
}

glm::vec3 camera::getCoor() const
{
	return Position;
}
