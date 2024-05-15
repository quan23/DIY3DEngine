#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H
#define MAX(a,b) (a>b)?a:b

constexpr int CHUNK_WIDTH =  16;
constexpr int CHUNK_LENGTH = 16;
constexpr int CHUNK_HEIGHT = 16;
constexpr int CHUNK_SIZE = CHUNK_WIDTH*CHUNK_LENGTH*CHUNK_HEIGHT;

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
		bool ChunkDoRender = false, EmptyChunk = true, ShouldUpdate = false, Calculating = false, OutOfBound = false;
		static int totalFace;
		GLushort numFace = 0;
		GLushort numVertex = 0;

		static blockCoor nearBlock[];

		std::vector <vertex> chunkVertex;
		std::vector <GLushort> chunkIndices;
		std::vector <vertex> chunkVoxel;

		Chunk(int x, int y, int z, World* world);
		~Chunk();
		
		static blockCoor inBoundary(blockCoor coor) 
		{ 
			return blockCoor
			(
				(coor.x % CHUNK_WIDTH + CHUNK_WIDTH) % CHUNK_WIDTH,
				(coor.y % CHUNK_HEIGHT + CHUNK_HEIGHT) % CHUNK_HEIGHT, 
				(coor.z % CHUNK_LENGTH + CHUNK_LENGTH) % CHUNK_LENGTH
			); 
		}
	    static unsigned int Coor2Pos(blockCoor coor) { return (coor.x + (coor.y * CHUNK_WIDTH + coor.z) * CHUNK_LENGTH); }
		static unsigned int Coor2Pos(Coor coor) { return (coor.x + (coor.y * CHUNK_WIDTH + coor.z) * CHUNK_LENGTH); }
		static blockCoor Pos2Coor(unsigned int pos) { return blockCoor(char((pos % (CHUNK_WIDTH * CHUNK_LENGTH)) % CHUNK_WIDTH), char(pos / (CHUNK_WIDTH * CHUNK_LENGTH)), char((pos % (CHUNK_WIDTH * CHUNK_LENGTH)) / CHUNK_LENGTH)); }

		void updateFace();
		void render(GLuint ShaderProgram) const;


		Block* hitBLock(glm::vec3 start, glm::vec3 rayDire, float distant) const;
		Block* getBlock(blockCoor Coor) const;
		Block* getBlock(GLushort Pos) const;

		void pushToGPU();

	private:
		
		World* world;
		VAO* _VAO = nullptr;
		VBO* _VBO = nullptr;
		EBO* _EBO = nullptr;

};
#endif
