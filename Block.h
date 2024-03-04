#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "Texture.h"
#include "DataType.h"

class Chunk;

class Block
{
	public:
		enum faceID
		{
			TOP = 0, NORTH = 1, WEST = 2, SOUTH = 3, EAST = 4, BOTTOM = 5
		};
		static glm::vec3 Vertex[];
		static GLushort Indices[];
		static glm::vec2 TextureCoor[4];
		char blockID;
		Block(char blockID = 0);
		//static face getFace(faceID face);
		void getFace(Chunk& chunk, Coor coor) const;
	private:

};

#endif // !BLOCK_CLASS_H
