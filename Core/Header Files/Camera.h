#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "DataType.h"

class Window;

class Camera
{
	public:
		Camera(Window& window, glm::vec3 Position = glm::vec3(0.0f));
		void cMatrix(float Fov, float fNear, float fFar, GLuint uniformID);
		void cInput(double fram);

		inline void setSpeed(float speed) { this->Speed = speed; }
		inline worldCoor getWorldCoor() const { return WorldCoor; }
		inline glm::vec3 getCoor() const { return Position; }

		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	private:
		glm::vec3 Position;
		
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Move = glm::vec3(0.0f);
		float Speed = 10.0f, Xsensitivity = 0.5f, Ysensitivity = 0.5f;
		float Yaw = 0.0f, Pitch = 0.0f;
		worldCoor WorldCoor;
		Window* window;
		bool firstClick = true;
};

#endif
