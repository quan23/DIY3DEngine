#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "DataType.h"

class camera
{
	public:
		camera(int Width, int Height, glm::vec3 Position = glm::vec3(0.0f));
		worldCoor getWorldCoor() const;
		void cMatrix(float Fov, float fNear, float fFar, GLuint uniformID);
		void cInput(GLFWwindow* window, double fram);
		void setSpeed(float speed);
		glm::vec3 getCoor() const;
	private:
		glm::vec3 Position;
		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Move = glm::vec3(0.0f);
		int Width, Height;
		float Speed = 10.0f, Xsensitivity = 0.5f, Ysensitivity = 0.5f;
		float Yaw = 0.0f, Pitch = 0.0f;
		worldCoor WorldCoor;
};

#endif
