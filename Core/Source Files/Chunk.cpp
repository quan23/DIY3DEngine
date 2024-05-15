#include "World.h"
#include "Block.h"
#include "Chunk.h"

blockCoor Chunk::nearBlock[] =
{
	Coor(0,1,0),
	Coor(0,0,1),
	Coor(-1,0,0),
	Coor(0,0,-1),
	Coor(1,0,0),
	Coor(0,-1,0),
};

int Chunk::totalFace = 0;

template <typename T>
void cleanMem(std::vector<T>& vector)
{
	vector.clear();
	vector.shrink_to_fit();
}

Chunk::Chunk(int x, int y, int z, World* world)
{
	//std::cout << "-1\n";
	Calculating = true;
	ChunkCoor = worldCoor(x, y, z);
	this->world = world;
	//srand((ChunkCoor.x<<4)+ChunkCoor.z);
	//float multi = float(rand() % 100) / 20.0;
	for (char x = 0; x < CHUNK_WIDTH; x++)
	{
		int realX = x + CHUNK_WIDTH * ChunkCoor.x;
		for (char z = 0; z < CHUNK_LENGTH; z++)
		{
			int realZ = z + CHUNK_LENGTH * ChunkCoor.z;
			int high = (glm::sin(glm::degrees(float(realX))) + glm::cos(glm::degrees(float(realZ))));
			int shigh = (glm::sin(float(realX)) + glm::cos(float(realZ)));

			//int shigh = (glm::sin(float(realX)) + glm::cos(float(realZ)));

			for (char y = 0; y < CHUNK_HEIGHT; y++)
			{
				int realY = y + CHUNK_HEIGHT * ChunkCoor.y;
				if (realY < high)
				{
					EmptyChunk = false;
					if (realY < 0)
						getBlock(Coor(x, y, z))->blockID = 5;
					else getBlock(Coor(x, y, z))->blockID = 1;
				}
				/*else if (realY < 0)
				{
					EmptyChunk = false;
					getBlock(Coor(x, y, z))->blockID = 1;
				}*/
				//else getBlock(Coor(x, y, z))->blockID = 0;
			}
		}
	}
	if (EmptyChunk)
	{
		//ChunkDoRender = false;
		delete[] blockList;
	}
	//updateFace();
}

Chunk::~Chunk()
{
	totalFace -= numFace;
	delete[] blockList;
	if (_VAO != nullptr)
		_VAO->Delete();
}

void Chunk::updateFace()
{
	Calculating = true;
	if (EmptyChunk)
	{
		return;
	}
	GLushort thisIndice[4];
	totalFace -= numFace;
	numVertex = 0;
	numFace = 0;
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_LENGTH; z++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				blockList[Coor2Pos(blockCoor(x, y, z))].getVoxelFace(*this, blockCoor(x, y, z));
			}
		}
	}
	//std::cout << "-1";
	//std::cout << numFace << " " << chunkIndices.size();
	//std::cout << chunkIndices.size() << "\n";
	if (numFace == 0)
	{
		ChunkDoRender = false;
		return;
	}
	else
	{
		world->ChunkToPush.push(this);
	}
	//else std::cout << numFace*6 - chunkIndices .size() << " ";
}

void Chunk::render(GLuint ShaderProgram) const
{ 
	if (ChunkDoRender)
	{
		_VAO->Bind();
		glUniform1i(glGetUniformLocation(ShaderProgram, "CHUNK_WIDTH"), CHUNK_WIDTH);
		glUniform1i(glGetUniformLocation(ShaderProgram, "CHUNK_LENGTH"), CHUNK_LENGTH);
		glUniform1i(glGetUniformLocation(ShaderProgram, "CHUNK_HEIGHT"), CHUNK_HEIGHT);
		glUniform1i(glGetUniformLocation(ShaderProgram, "chunkX"), ChunkCoor.x);
		glUniform1i(glGetUniformLocation(ShaderProgram, "chunkY"), ChunkCoor.y);
		glUniform1i(glGetUniformLocation(ShaderProgram, "chunkZ"), ChunkCoor.z);
		//glDrawElements(GL_TRIANGLES, numFace * 6, GL_UNSIGNED_SHORT, 0);
		glDrawArrays(GL_POINTS, 0, numFace);
		_VAO->Unbind();
	}
}

Block* Chunk::hitBLock(glm::vec3 target, glm::vec3 rayDire, float distant) const
{
	int maxCal = 1000;
	glm::vec3 stepNeed = glm::vec3(0.0f);
	auto inBoundOf = [](glm::vec3 &coor)
	{
		return blockCoor(int(coor.x) + ((coor.x < 0.0f) ? -1 : 0), int(coor.y) + ((coor.y < 0.0f) ? -1 : 0), int(coor.z) + ((coor.z < 0.0f) ? -1 : 0));
	};
	//if (world.getChunk(worldCoor(0, 0, 0)) == nullptr) return { nullptr,nullptr };
	blockCoor targetBlock = inBoundOf(target);
	Block* block = getBlock(targetBlock);
	if (block == nullptr) return nullptr;
	blockCoor nextBlock;
	auto checkDire = [](GLfloat rayDire, long long& nextBlock, int targetBlock, GLfloat& stepNeed, GLfloat target)
	{
		if (rayDire == 0.0f)
			nextBlock = 0;
		else if (rayDire < 0.0f)
			nextBlock = -1;
		else nextBlock = 1;
		if (nextBlock != 0) stepNeed = abs(std::max(target - (targetBlock + nextBlock + 1), (targetBlock + nextBlock) - target) / rayDire);
		else stepNeed = 9999.0f;
	};
	checkDire(rayDire.x, nextBlock.x, targetBlock.x, stepNeed.x, target.x);
	checkDire(rayDire.y, nextBlock.y, targetBlock.y, stepNeed.y, target.y);
	checkDire(rayDire.z, nextBlock.z, targetBlock.z, stepNeed.z, target.z);
	while (distant > 0.0f && block->blockID == 0)
	{
		if (nextBlock.x != 0 && (stepNeed.x <= stepNeed.y && stepNeed.x <= stepNeed.z))
		{
			target += rayDire * glm::vec3(stepNeed.x);
			distant -= stepNeed.x;
			stepNeed -= glm::vec3(stepNeed.x);
			//targetBlock = inBoundOf(target);
			targetBlock.x += nextBlock.x;
			stepNeed.x = abs(std::max(target.x - (targetBlock.x + nextBlock.x + 1), (targetBlock.x + nextBlock.x) - target.x) / rayDire.x);
		}
		else if (nextBlock.y != 0 && (stepNeed.y <= stepNeed.x && stepNeed.y <= stepNeed.z))
		{
			target += rayDire * glm::vec3(stepNeed.y);
			distant -= stepNeed.y;
			stepNeed -= glm::vec3(stepNeed.y);
			//targetBlock = inBoundOf(target);
			targetBlock.y += nextBlock.y;
			stepNeed.y = abs(std::max(target.y - (targetBlock.y + nextBlock.y + 1), (targetBlock.y + nextBlock.y) - target.y) / rayDire.y);
		}
		else if (nextBlock.z != 0 && (stepNeed.z <= stepNeed.y && stepNeed.z <= stepNeed.x))
		{
			target += rayDire * glm::vec3(stepNeed.z);
			distant -= stepNeed.z;
			stepNeed -= glm::vec3(stepNeed.z);
			//targetBlock = inBoundOf(target);
			targetBlock.z += nextBlock.z;
			stepNeed.z = abs(std::max(target.z - (targetBlock.z + nextBlock.z + 1), (targetBlock.z + nextBlock.z) - target.z) / rayDire.z);
		}

		//std::cout << distant << "\n";
		if (distant < 0.0f)
		{
			//std::cout << "bruh\n";
			//std::cout << target.x << " " << target.y << " " << target.z << " " << targetBlock << "out\n";

			return nullptr;
		};
		block = getBlock(targetBlock);
		if (block == nullptr)
		{
			//std::cout << target.x << " " << target.y << " " << target.z << targetBlock << "null\n";
			return nullptr;
		}
		if (maxCal-- < 0)
		{
			//std::cout << target.x << " " << target.y << " " << target.z << targetBlock << "skull\n";
			//std::cout << "skull ";
			std::cout << rayDire.x << " " << rayDire.y << " " << rayDire.z << "skull\n";
			return nullptr;
		}

	}
	//std::cout << target.x << " " << target.y << " " << target.z << targetBlock << "hit\n";
	if (distant > 0.0f) return block;
	else return nullptr;
}

Block* Chunk::getBlock(blockCoor coor) const
{
	if (this == nullptr) return nullptr;
	if (EmptyChunk) return nullptr;
	if ((coor.x < 0) || (coor.x >= CHUNK_WIDTH) || (coor.y < 0) || (coor.y >= CHUNK_HEIGHT) || (coor.z < 0) || (coor.z >= CHUNK_LENGTH))
	{
		worldCoor wCoor = ChunkCoor;
		wCoor.x += (coor.x / CHUNK_WIDTH + ((coor.x < 0) ? -1 : 0));
		wCoor.y += (coor.y / CHUNK_HEIGHT + ((coor.y < 0) ? -1 : 0));
		wCoor.z += (coor.z / CHUNK_LENGTH + ((coor.z < 0) ? -1 : 0));
		if (world->getChunk(wCoor) != nullptr)
			return (world->getChunk(wCoor)->getBlock(inBoundary(coor)));
		else return nullptr;
	}
	return getBlock(Coor2Pos(coor));
	return nullptr;
}

Block* Chunk::getBlock(GLushort Pos) const
{
	if (this == nullptr) return nullptr;
	if (EmptyChunk) return nullptr;
	if ((Pos >= 0) && (Pos < CHUNK_SIZE)) return &(blockList[Pos]);
	return nullptr;
}

void Chunk::pushToGPU()
{
	Calculating = true;
	ChunkDoRender = false;

	/*if (_VAO != nullptr) _VAO->Delete();
	_VAO = new VAO;
	_VBO = new VBO(&chunkVertex[0].Position, numFace * 4 * sizeof(vertex));
	_EBO = new EBO(&chunkIndices[0], numFace * 6 * sizeof(GLushort));
	_VAO->LinkVBO(*_VBO, 0, 1, 2 * sizeof(int), 0);
	_VAO->LinkVBO(*_VBO, 1, 1, 2 * sizeof(int), sizeof(int));
	_VAO->Unbind();
	_VBO->Delete();
	_EBO->Delete();
	cleanMem(chunkVertex);
	cleanMem(chunkIndices);*/

	if (_VAO != nullptr) _VAO->Delete();
	_VAO = new VAO;
	//if (_VAO == nullptr) _VAO = new VAO;
	_VBO = new VBO(&chunkVoxel[0].Position, numFace*sizeof(vertex));
	_VAO->LinkVBO(*_VBO, 0, 1, 2 * sizeof(int), 0);
	_VAO->LinkVBO(*_VBO, 1, 1, 2 * sizeof(int), sizeof(int));
	_VAO->Unbind();
	_VBO->Delete();
	cleanMem(chunkVoxel);

	ChunkDoRender = true;
}

