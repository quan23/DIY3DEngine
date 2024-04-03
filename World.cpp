#include "Chunk.h"
#include "World.h"

worldCoor nextChunk[] =
{
	worldCoor(0,1,0),
	worldCoor(0,0,1),
	worldCoor(-1,0,0),
	worldCoor(0,0,-1),
	worldCoor(1,0,0),
	worldCoor(0,-1,0),
};

World::World(GLuint ShaderProgram) : ShaderProgram(ShaderProgram)
{

}

unsigned long long World::ChunkPos(worldCoor coor)
{
	unsigned long long worldPos = (coor.x + 0xfff) & 0xfff;
	worldPos <<= 8;
	worldPos += (coor.y + 0xff) & 0xff;
	worldPos <<= 12;
	worldPos += (coor.z + 0xfff) & 0xfff;
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
					newChunk.push(worldMap[ChunkPos(worldCoor(x, y, z))]);
				}
				//std::cout << worldMap[ChunkPos(worldCoor(x, y, z))]->ChunkCoor << "\n";
			}
		}
	}
}

void World::renderWorld()
{
	while (!newChunk.empty())
	{
		allChunk.push_back(newChunk.front());
		newChunk.pop();
	}
	for (auto chunk = allChunk.begin(); chunk != allChunk.end();)
	{
		if ((*chunk)->OutOfBound)
		{
			ChunkToDelete.push(*chunk);
			chunk = allChunk.erase(chunk);
		}
		else
		{
			(*chunk)->render(ShaderProgram);
			++chunk;
		}
	}
	while (!ChunkToDelete.empty())
	{
		delete ChunkToDelete.front();
		ChunkToDelete.pop();
	}
	//std::cout << Chunk::totalIndices << "\n";
}

void World::addChunk(int x, int y, int z)
{
	worldMap[ChunkPos(worldCoor(x, y, z))] = new Chunk(x, y, z, this);
	if (!worldMap[ChunkPos(worldCoor(x, y, z))]->EmptyChunk)
	{
		updatedChunk.push(worldMap[ChunkPos(worldCoor(x, y, z))]);
		for (char i = 0; i < 6; i++)
		{
			if (worldMap[ChunkPos(worldCoor(x, y, z) + nextChunk[i])] != NULL)
			{
				if (!worldMap[ChunkPos(worldCoor(x, y, z) + nextChunk[i])]->EmptyChunk && worldMap[ChunkPos(worldCoor(x, y, z) + nextChunk[i])]->ChunkDoRender)
				{
					worldMap[ChunkPos(worldCoor(x, y, z) + nextChunk[i])]->ShouldUpdate = true;

					//std::cout << -1;
				}
			}
		}
	}
}

void World::loadChunk(int x, int y, int z)
{
}

void World::reloadWorld()
{
	for (auto chunk=worldMap.begin();chunk!=worldMap.end();)
	{
		if (chunk->second != NULL)
		{
			if (!chunk->second->Calculating)
			{
				if (tooFar(chunk->second->ChunkCoor))
				{
					chunk->second->OutOfBound = true;
					chunk->second = NULL;
					chunk = worldMap.erase(chunk);
				}
				else
				{
					if (chunk->second->ShouldUpdate)
					{
						updatedChunk.push(chunk->second);
						chunk->second->ShouldUpdate = false;
						chunk->second->Calculating = true;
					}
					chunk++;
				}
			}
			else chunk++;
		}
		else
		{
			chunk = worldMap.erase(chunk);
		}
	}
	while (!updatedChunk.empty())
	{
		updatedChunk.front()->updateFace();
		updatedChunk.pop();
	}
}

Chunk* World::getChunk(worldCoor coor)
{
	if (worldMap[ChunkPos(coor)] == NULL) return nullptr;
	return worldMap[ChunkPos(coor)];
}

Block* World::getBlock(blockCoor coor)
{
	if (worldMap[ChunkPos(inWhatChunk(coor))] == nullptr) return nullptr;
	return worldMap[ChunkPos(inWhatChunk(coor))]->getBlock(Chunk::inBoundary(coor));
}

void World::startLoading()
{
	if (loadingThread == nullptr && !doLoop)
	{
		doLoop = true;
		loadingThread = new std::thread(&World::loadingLoop, this);
		
	}
}

void World::endloading()
{
	doLoop = false;
	loadingThread->join();
	delete loadingThread;
	loadingThread = nullptr;
}

void World::pushAllChunk()
{
	while (!ChunkToPush.empty())
	{
		ChunkToPush.front()->pushToGPU();
		ChunkToPush.front()->Calculating = false;
		ChunkToPush.pop();
	}
}

void World::loadingLoop()
{
	while (doLoop)
	{
		loadWorld(worldAnchor, renderDist);
		reloadWorld();
		//std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}

bool World::tooFar(worldCoor& chunkCoor) const
{
	return (abs(chunkCoor.x - worldAnchor.x) > renderDist) || (abs(chunkCoor.y - worldAnchor.y) > renderDist) || (abs(chunkCoor.z - worldAnchor.z) > renderDist);
}
