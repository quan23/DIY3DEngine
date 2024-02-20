#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "Texture.h"
#include "DataType.h"

class Block
{
	public:
		enum faceID
		{
			TOP = 0, NORTH = 1, WEST = 2, SOUTH = 3, EAST = 4, BOTTOM = 5
		};
		static glm::vec3 vertex[];
		static GLushort indices[];
		char blockID;
		Block(char blockID = 0);
		static face getFace(faceID face);
		static glm::vec2 TextureCoor[4];
	private:

};

#endif // !BLOCK_CLASS_H
