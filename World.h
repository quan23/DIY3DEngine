#ifndef WOTLD_CLASS_H
#define WORLD_CLASS_H

#include <unordered_map>
#include <vector>
#include "Chunk.h"



class World
{
	public:
		std::unordered_map <unsigned long long, Chunk*> worldMap;
		std::vector <Chunk*> renderChunk;
		World(GLuint ShaderProgram);
		static unsigned long long ChunkPos(Chunk::worldCoor coor);
		void loadWorld(Chunk::worldCoor Center, GLushort radian);
		void renderWorld();
		void addChunk(int x, int y, int z);
		void loadChunk(int x, int y, int z);
		static Chunk::worldCoor inWhatChunk(int x, int y, int z);
		GLuint ShaderProgram;
	private:
};

#endif // !WOTLD_CLASS_H
