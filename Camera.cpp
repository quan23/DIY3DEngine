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
	float step = speed * fram;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cPosition += cOrientation*step;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cPosition -= cOrientation*step;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cPosition -= glm::normalize(glm::cross(cOrientation, cUp))*step;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cPosition += glm::normalize(glm::cross(cOrientation, cUp))*step;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cPosition -= cUp*step;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cPosition += cUp*step;
	}
	glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	mouseX = sensitivity*(mouseX - (float)cWidth / 2)/cWidth;
	mouseY = sensitivity*(mouseY - (float)cHeight / 2)/cHeight;
	cOrientation = glm::rotate(cOrientation, (float)glm::radians(mouseX), -cUp);
	cOrientation = glm::rotate(cOrientation, (float)glm::radians(mouseY), -glm::normalize(glm::cross(cOrientation,cUp)));
	cOrientation = glm::normalize(cOrientation);
	glfwSetCursorPos(window, (double)cWidth/2, (double)cHeight/2);

}

