#ifndef WOTLD_CLASS_H
#define WORLD_CLASS_H

#include <unordered_map>
#include <vector>
#include <queue>
#include <thread>
#include "DataType.h"
#include "Chunk.h"

class World
{
	public:
		World(GLuint ShaderProgram);
		std::queue <Chunk*> ChunkToPush;

		void loadWorld(worldCoor Center, GLushort radian);
		void renderWorld();
		void addChunk(int x, int y, int z);
		void loadChunk(int x, int y, int z);
		void reloadWorld();

		static unsigned long long ChunkPos(worldCoor coor);
		Chunk* getChunk(worldCoor coor);
		Block* getBlock(blockCoor coor);

		inline static worldCoor inWhatChunk(int x, int y, int z) { return worldCoor(x / CHUNK_WIDTH + ((x < 0) ? -1 : 0), y / CHUNK_HEIGHT + ((y < 0) ? -1 : 0), z / CHUNK_LENGTH + ((z < 0) ? -1 : 0)); }
		//inline static worldCoor inWhatChunk(Coor coor) { return inWhatChunk(coor.x, coor.y, coor.z); }
		inline static worldCoor inWhatChunk(blockCoor coor) { return inWhatChunk(coor.x, coor.y, coor.z); }

		GLuint ShaderProgram;

		void startLoading();
		void endloading();
		void pushAllChunk();
		inline void updateWorldAnchor(worldCoor newAnchor) { worldAnchor = newAnchor; }
		inline void updataRenderDist(int renderDist) { this->renderDist = renderDist; }

	private:
		std::unordered_map <unsigned long long, Chunk*> worldMap;
		std::vector <Chunk*> allChunk;
		std::queue <Chunk*> updatedChunk;
		std::queue <Chunk*> newChunk;
		std::queue <Chunk*> ChunkToDelete;
		worldCoor worldAnchor = worldCoor(0, 0, 0);
		std::thread* loadingThread = nullptr;
		void loadingLoop();
		bool tooFar(worldCoor& chunkCoor) const;
		bool doLoop = false;
		int renderDist = 0;
};

#endif // !WOTLD_CLASS_H
