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
		std::unordered_map <unsigned long long, Chunk*> worldMap;
		std::vector <Chunk*> allChunk;
		std::queue <Chunk*> updatedChunk;
		std::queue <Chunk*> ChunkToPush;
		std::queue <Chunk*> ChunkToDelete;
		worldCoor worldAnchor = worldCoor(0,0,0);

		World(GLuint ShaderProgram);

		void loadWorld(worldCoor Center, GLushort radian);
		void renderWorld();
		void addChunk(int x, int y, int z);
		void loadChunk(int x, int y, int z);
		void reloadWorld();

		static unsigned long long ChunkPos(worldCoor coor);
		Chunk* getChunk(worldCoor coor);

		static worldCoor inWhatChunk(int x, int y, int z);
		static worldCoor inWhatChunk(Coor coor);

		GLuint ShaderProgram;

		void startLoading();
		void endloading();
		void pushAllChunk();
		void updateWorldAnchor(worldCoor newAnchor);
		void updataRenderDist(int renderDist);

	private:
		std::thread* loadingThread = nullptr;
		void loadingLoop();
		bool tooFar(worldCoor& chunkCoor) const;
		bool doLoop = false;
		int renderDist = 0;
};

#endif // !WOTLD_CLASS_H
