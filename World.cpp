#include "World.h"

World::World(GLuint ShaderProgram) : ShaderProgram(ShaderProgram)
{
}

unsigned long long World::ChunkPos(Chunk::worldCoor coor)
{
	return (long long)(coor.x & 0xffffff) + ((long long)(coor.z & 0xffffff) << 24) + ((unsigned long long)(coor.z & 0xfff) << 48);
}

void World::loadWorld(Chunk::worldCoor Center, GLushort radian)
{
	for (int x = Center.x - radian; x < Center.x + radian; x++)
	{
		for (int y = Center.y - radian; y < Center.y + radian; y++)
		{
			for (int z = Center.z - radian; z < Center.z + radian; z++)
			{
				if (worldMap[ChunkPos(Chunk::worldCoor(x, y, z))] == nullptr)
				{
					addChunk(x, y, z);
					//std::cout << "-1\n";
				}
				if (!worldMap[ChunkPos(Chunk::worldCoor(x, y, z))]->EmptyChunk && worldMap[ChunkPos(Chunk::worldCoor(x, y, z))]->ChunkDoRender)
				{
					renderChunk.push_back(worldMap[ChunkPos(Chunk::worldCoor(x, y, z))]);
				}
				std::cout << worldMap[ChunkPos(Chunk::worldCoor(x, y, z))]->ChunkCoor;
			}
		}
	}
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
	worldMap[ChunkPos(Chunk::worldCoor(x, y, z))] = new Chunk(x, y, z);
}

void World::loadChunk(int x, int y, int z)
{
}

Chunk::worldCoor World::inWhatChunk(int x, int y, int z)
{
	return Chunk::worldCoor(x%CHUNK_WIDTH,y%CHUNK_HEIGHT,z%CHUNK_LENGTH);
}
