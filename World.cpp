#include "Chunk.h"
#include "World.h"


World::World(GLuint ShaderProgram) : ShaderProgram(ShaderProgram)
{
}

unsigned long long World::ChunkPos(worldCoor coor)
{
	unsigned long long worldPos = (coor.x + 0x7fffff) & 0xffffff;
	worldPos <<= 16;
	worldPos += (coor.y + 0x7fff) & 0xffff;
	worldPos <<= 24;
	worldPos += (coor.z + 0x7fffff) & 0xffffff;
	return worldPos;
}

void World::loadWorld(worldCoor Center, GLushort radian)
{
	for (int x = Center.x - radian; x <= Center.x + radian; x++)
	{
		for (int y = Center.y - radian; y <= Center.y + radian; y++)
		//for (int y = 0; y <= 0; y++)
		{
			for (int z = Center.z - radian; z <= Center.z + radian; z++)
			{
				if (worldMap[ChunkPos(worldCoor(x, y, z))] == NULL)
				{
					addChunk(x, y, z);
					//std::cout << "-1\n";
					if (!worldMap[ChunkPos(worldCoor(x, y, z))]->EmptyChunk)
					{
						addedChunk.push_back(worldMap[ChunkPos(worldCoor(x, y, z))]);
					}
				}
				//std::cout << worldMap[ChunkPos(worldCoor(x, y, z))]->ChunkCoor << "\n";
			}
		}
	}
	//std::cout << renderChunk.size();
	//std::cout << addedChunk.size();
	for (Chunk* chunk : addedChunk)
	{
		chunk->updateFace();
		if (chunk->ChunkDoRender)
		{
			renderChunk.push_back(chunk);
			//std::cout << chunk->ChunkCoor << " " << chunk->numVertex << "\n";
			std::cout << Chunk::totalIndices << "\n";
		}
	}
	addedChunk.clear();
	//std::cout << renderChunk.size() << " " << Chunk::totalIndices << "\n";
	
}

void World::renderWorld()
{
	for (Chunk*& chunk : renderChunk)
	{
		chunk->render(ShaderProgram);
	}
}

void World::addChunk(int x, int y, int z)
{
	worldMap[ChunkPos(worldCoor(x, y, z))] = new Chunk(x, y, z, this);
}

void World::loadChunk(int x, int y, int z)
{
}


Chunk* World::getChunk(worldCoor coor)
{
	if (worldMap[ChunkPos(coor)] == NULL) return nullptr;
	return worldMap[ChunkPos(coor)];
}

worldCoor World::inWhatChunk(int x, int y, int z)
{
	return worldCoor(x / CHUNK_WIDTH + ((x < 0) ? -1 : 0), y / CHUNK_HEIGHT + ((y < 0) ? -1 : 0), z / CHUNK_LENGTH + ((z < 0) ? -1 : 0));
}

worldCoor World::inWhatChunk(Coor coor)
{
	return inWhatChunk(coor.x, coor.y, coor.z);
}
