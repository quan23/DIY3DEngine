#include "World.h"
#include "Camera.h"

camera::camera(int Width, int Height, glm::vec3 position) : cWidth(Width), cHeight(Height), cPosition(position)
{
}

void camera::cMatrix(float Fov, float fNear, float fFar, GLuint uniformID)
{
	glm::mat4 tran = glm::lookAt(cPosition, cPosition + cOrientation, cUp);
	glm::mat4 proj = glm::perspective(glm::radians(Fov), (float)cWidth / (float)cHeight, fNear, fFar);
	glm::mat4 view = proj * tran;
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, &view[0][0]);
}

void camera::cInput(GLFWwindow* window, double fram)
{
	cMove = glm::vec3(0.0f);
	float step = speed * fram;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cMove += glm::normalize(glm::vec3(cOrientation.x,0.0f, cOrientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cMove -= glm::normalize(glm::vec3(cOrientation.x, 0.0f, cOrientation.z)) ;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cMove -= glm::normalize(glm::cross(cOrientation, cUp));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cMove += glm::normalize(glm::cross(cOrientation, cUp));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cMove -= cUp;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cMove += cUp;
	}
	if (glm::length(cMove)!=0) cPosition += glm::normalize(cMove)*step;
	WorldCoor = World::inWhatChunk(int(cPosition.x), int(cPosition.y), int(cPosition.z));
	glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	mouseX = Xsensitivity*(mouseX - (float)cWidth / 2);
	mouseY = Ysensitivity*(mouseY - (float)cHeight / 2);
	yaw += mouseX;
	pitch += mouseY;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch <-89.0f) pitch =-89.0f;
	cOrientation.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cOrientation.y =-sin(glm::radians(pitch));
	cOrientation.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cOrientation = glm::normalize(cOrientation);
	glfwSetCursorPos(window, (double)cWidth/2, (double)cHeight/2);

}

