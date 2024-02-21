#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

constexpr signed char CHUNK_WIDTH = 16;
constexpr signed char CHUNK_LENGTH = 16;
constexpr signed char CHUNK_HEIGHT = 16;
constexpr unsigned int CHUNK_SIZE = CHUNK_WIDTH*CHUNK_LENGTH*CHUNK_HEIGHT;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "DataType.h"
#include "Block.h"

class World;

class Chunk
{
	public:
		Block* blockList = new Block[CHUNK_SIZE];
		worldCoor ChunkCoor;
		bool ChunkDoRender = false, EmptyChunk = true;
		static Coor nearBlock[];
		static int totalIndices;
		Chunk(int x, int y, int z, World* world);
		unsigned int Coor2Pos(Coor coor);
		Coor Pos2Coor(unsigned int pos);
		void updateFace();
		void render(GLuint ShaderProgram);
		Block* getBlock(Coor Coor);
		Block* getBlock(GLushort Pos);
		GLushort numVertex = 0;
	private:
		GLushort numFace = 0;
		World* world;
		VAO* _VAO;
		VBO* _VBO;
		EBO* _EBO;
};


#endif
