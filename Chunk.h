#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

constexpr signed char CHUNK_WIDTH = 16;
constexpr signed char CHUNK_LENGTH = 16;
constexpr signed char CHUNK_HEIGHT = 16;
constexpr unsigned int CHUNK_SIZE = CHUNK_WIDTH*CHUNK_LENGTH*CHUNK_HEIGHT;

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
		bool ChunkDoRender = false, EmptyChunk = true, ShouldUpdate = false;
		static int totalIndices;
		GLushort numFace = 0;
		GLushort numVertex = 0;

		static Coor nearBlock[];

		std::vector <vertex> chunkVertex;
		std::vector <GLushort> chunkIndices;

		Chunk(int x, int y, int z, World* world);
		~Chunk();


		unsigned int Coor2Pos(Coor coor) const;
		Coor Pos2Coor(unsigned int pos) const;
		void updateFace();
		void render(GLuint ShaderProgram) const;
		Block* getBlock(Coor Coor) const;
		Block* getBlock(GLushort Pos) const;
		void pushToGPU();

	private:
		
		World* world;
		VAO* _VAO = nullptr;
		VBO* _VBO = nullptr;
		EBO* _EBO = nullptr;
};


#endif
