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
					
				}
				//std::cout << worldMap[ChunkPos(worldCoor(x, y, z))]->ChunkCoor << "\n";
			}
		}
	}
	reloadWorld();
}

void World::renderWorld()
{
	for (auto chunk=worldMap.begin();chunk!=worldMap.end();)
	{
		if (chunk->second != NULL)
		{
			if (tooFar(chunk->second->ChunkCoor)&&!chunk->second->Calculating)
			{
				ChunkToDelete.push(chunk->second);
				chunk = worldMap.erase(chunk);
			}
			else
			{
				if (chunk->second->ChunkDoRender)
					chunk->second->render(ShaderProgram);
				if (chunk->second->ShouldUpdate)
				{
					updatedChunk.push(chunk->second);
					chunk->second->ShouldUpdate = false;
					chunk->second->Calculating = true;
				}
				chunk++;
			}
		}
		else
		{
			ChunkToDelete.push(chunk->second);
			chunk = worldMap.erase(chunk);
		}
	}
	//std::cout << Chunk::totalIndices << "\n";
}

void World::addChunk(int x, int y, int z)
{
	worldMap[ChunkPos(worldCoor(x, y, z))] = new Chunk(x, y, z, this);
	if (!worldMap[ChunkPos(worldCoor(x, y, z))]->EmptyChunk)
	{
		updatedChunk.push(worldMap[ChunkPos(worldCoor(x, y, z))]);
		renderChunk.push_back(worldMap[ChunkPos(worldCoor(x, y, z))]);
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
	if (!ChunkToDelete.empty())
	{
		delete ChunkToDelete.front();
		ChunkToDelete.pop();
	}
	if (!updatedChunk.empty())
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

worldCoor World::inWhatChunk(int x, int y, int z)
{
	return worldCoor(x / CHUNK_WIDTH + ((x < 0) ? -1 : 0), y / CHUNK_HEIGHT + ((y < 0) ? -1 : 0), z / CHUNK_LENGTH + ((z < 0) ? -1 : 0));
}

worldCoor World::inWhatChunk(Coor coor)
{
	return inWhatChunk(coor.x, coor.y, coor.z);
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
	loadingThread->detach();
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

void World::updateWorldAnchor(worldCoor newAnchor)
{
	worldAnchor = newAnchor;
}

void World::updataRenderDist(int renderDist)
{
	this->renderDist = renderDist;
}

void World::loadingLoop()
{
	while (doLoop)
	{
		loadWorld(worldAnchor, renderDist);
	}
}

bool World::tooFar(worldCoor& chunkCoor) const
{
	return (abs(chunkCoor.x - worldAnchor.x) > renderDist) || (abs(chunkCoor.y - worldAnchor.y) > renderDist) || (abs(chunkCoor.z - worldAnchor.z) > renderDist);
}
