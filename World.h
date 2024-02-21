#ifndef WOTLD_CLASS_H
#define WORLD_CLASS_H

#include <unordered_map>
#include <vector>
#include "DataType.h"
#include "Chunk.h"

class World
{
	public:
		std::unordered_map <unsigned long long, Chunk*> worldMap;
		std::vector <Chunk*> renderChunk;
		std::vector <Chunk*> addedChunk;
		World(GLuint ShaderProgram);
		static unsigned long long ChunkPos(worldCoor coor);
		void loadWorld(worldCoor Center, GLushort radian);
		void renderWorld();
		void addChunk(int x, int y, int z);
		void loadChunk(int x, int y, int z);
		Chunk* getChunk(worldCoor coor);
		static worldCoor inWhatChunk(int x, int y, int z);
		static worldCoor inWhatChunk(Coor coor);
		GLuint ShaderProgram;
	private:
};

#endif // !WOTLD_CLASS_H
