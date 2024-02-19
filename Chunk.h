#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

constexpr char CHUNK_WIDTH = 16;
constexpr char CHUNK_LENGTH = 16;
constexpr char CHUNK_HEIGHT = 16;
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
#include "Block.h"

class Chunk
{
	public:
		struct Coor
		{
			char x, y, z;
			Coor(char x, char y, char z);
			Coor operator+(const Coor& coor);
			bool operator==(const Coor& Coor);
			bool operator!=(const Coor& Coor);
		};
		struct worldCoor
		{
			int x, y, z;
			worldCoor(int x = 0, int y = 0, int z = 0);
			worldCoor operator+(const worldCoor& coor);
			std::ostream& operator<<(std::ostream& stream);
		};
		
		Block* blockList = new Block[CHUNK_SIZE];
		worldCoor ChunkCoor;
		bool ChunkDoRender = false, EmptyChunk = true;
		static Coor nearBlock[];
		Chunk(int x = 0, int y = 0, int z = 0);
		unsigned int Coor2Pos(Coor coor);
		Coor Pos2Coor(unsigned int pos);
		void updateFace();
		void render(GLuint ShaderProgram);
		Block* getBlock(Coor Coor);
		Block* getBlock(GLushort Pos);
	private:
		std::vector <Block::face> chunkFace;
		std::vector <glm::vec3> chunkVertex;
		std::vector <GLushort> chunkIndices;
		GLushort* indicesCheck = new GLushort[(CHUNK_WIDTH + 1) * (CHUNK_LENGTH + 1) * (CHUNK_HEIGHT + 1) + 1];
		GLushort numFace = 0;
		GLushort numVertex = 0;
		VAO* _VAO;
		VBO* _VBO;
		EBO* _EBO;
};

#endif
