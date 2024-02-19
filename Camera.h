#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class camera
{
	public:
		glm::vec3 cPosition;
		glm::vec3 cOrientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cUp = glm::vec3(0.0f, 1.0f, 0.0f);
		int cWidth, cHeight;

		float speed = 5.0f, Xsensitivity = 0.5f, Ysensitivity = 0.5f;
		float yaw = 0.0f, pitch = 0.0f;
	
		camera(int Width, int Height, glm::vec3 position = glm::vec3(0.0f));
		void cMatrix(float Fov, float fNear, float fFar, GLuint uniformID);
		void cInput(GLFWwindow* window, double fram);
};

#endif
