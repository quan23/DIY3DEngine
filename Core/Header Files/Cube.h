#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

class cube
{
	private:
		union Vertex
		{
			struct
			{
				glm::vec3 position;
				//glm::vec2 textureCoor;
				//glm::vec3 vertexNormal;
			};
			struct
			{
				GLfloat data[3];
			};
		};
		struct faceIndices
		{
			GLushort Indices[4];
			glm::vec2 Texture;
			glm::vec3 Normal;
		};
		static Vertex defaultVertex[];
		static GLushort defaultIndices[];
		Vertex vertex[24];
		VAO* _VAO;
		VBO* _VBO;
		EBO* _EBO;
	public:
		glm::vec3 worldP;
		glm::mat4 worldR;
		glm::vec3 worldZ;
		Texture* mainTex,*specTex;
		cube(glm::vec3 size, glm::vec3 Position, glm::mat4 rotation, Texture& mainTex, Texture& specTex);
		void rotate(glm::mat4 nRotation);
		void setRotation(glm::mat4 nRotation);
		void move(glm::vec3 nPosition);
		void setPosition(glm::vec3 nPosition);
		void render();
};

#endif
/*
0 0 0   0 0  0 0 -1
0 1 0   0 1  0 0 -1
1 1 0   1 1  0 0 -1
1 0 0   1 0  0 0 -1
	   	    
1 0 1   0 1  0 0 1
1 1 1   0 0  0 0 1
0 1 1   1 0  0 0 1
0 0 1   1 1  0 0 1
	   	    
1 0 0   1 0  1 0 0
1 1 0   1 1  1 0 0
1 1 1   0 0  1 0 0
1 0 1   0 1  1 0 0
	   	    
0 0 1   1 1  -1 0 0
0 1 1   1 0  -1 0 0
0 1 0   0 1  -1 0 0
0 0 0   0 0  -1 0 0
	   	    
0 1 0   0 1  0 1 0
0 1 1   1 0  0 1 0
1 1 1   0 0  0 1 0
1 1 0   1 1  0 1 0
	   	    
0 0 1   1 1  0 -1 0
0 0 0   0 0  0 -1 0
1 0 0   1 0  0 -1 0
1 0 1   0 1  0 -1 0
*/
