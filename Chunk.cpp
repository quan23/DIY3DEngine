#include "World.h"
#include "Block.h"
#include "Chunk.h"

Coor Chunk::nearBlock[] =
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
			//int high = multi*(glm::sin(glm::degrees(float(realX))) + glm::cos(glm::degrees(float(realZ))));
			int shigh = (glm::sin(float(realX)) + glm::cos(float(realZ)));

			for (char y = 0; y < CHUNK_HEIGHT; y++)
			{
				int realY = y + CHUNK_HEIGHT * ChunkCoor.y;
				if (realY < shigh)
				{
					EmptyChunk = false;
					getBlock(Coor(x, y, z))->blockID = rand() % 5 + 1;
					/*if (realY < 0)
						getBlock(Coor(x, y, z))->blockID = 5;
					else getBlock(Coor(x, y, z))->blockID = 1;*/
				}
				/*else if (realY < 0)
				{
					EmptyChunk = false;
					getBlock(Coor(x, y, z))->blockID = 1;
				}*/
				//else getBlock(Coor(x, y, z))->blockID = 0;
				/*if (((realX ^ realY) ^ realZ) & 1  && realY<0)
				{
					EmptyChunk = false;
					getBlock(Coor(x, y, z))->blockID = 1;
				}*/
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

unsigned int Chunk::Coor2Pos(Coor coor)
{
	return (coor.x + (coor.y * CHUNK_WIDTH + coor.z) * CHUNK_LENGTH);
}

Coor Chunk::Pos2Coor(unsigned int pos)
{
	return Coor(char((pos % (CHUNK_WIDTH * CHUNK_LENGTH)) % CHUNK_WIDTH), char(pos / (CHUNK_WIDTH * CHUNK_LENGTH)), char((pos % (CHUNK_WIDTH * CHUNK_LENGTH)) / CHUNK_LENGTH));
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
	/*GLushort (*indicesID)(glm::vec3&) = [](glm::vec3& coor)
	{
		return GLushort(GLushort(coor.x) + (GLushort(coor.y) * (CHUNK_LENGTH+1) + GLushort(coor.z)) * (CHUNK_WIDTH+1));
	};*/
	for (char x = 0; x < CHUNK_WIDTH; x++)
	{
		for (char z = 0; z < CHUNK_LENGTH; z++)
		{
			for (char y = 0; y < CHUNK_HEIGHT; y++)
			{
				blockList[Coor2Pos(Coor(x, y, z))].getVoxelFace(*this, Coor(x, y, z));
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
		//glDrawElements(GL_POINTS, numFace, GL_UNSIGNED_SHORT, 0);
		glDrawArrays(GL_POINTS, 0, numFace);
		_VAO->Unbind();
	}
}

Block* Chunk::getBlock(Coor coor) const
{
	if (EmptyChunk) return nullptr;
	if ((coor.x < 0) || (coor.x >= CHUNK_WIDTH) || (coor.y < 0) || (coor.y >= CHUNK_HEIGHT) || (coor.z < 0) || (coor.z >= CHUNK_LENGTH))
	{
		worldCoor wCoor = ChunkCoor;
		wCoor.x += (coor.x / CHUNK_WIDTH + ((coor.x < 0) ? -1 : 0));
		wCoor.y += (coor.y / CHUNK_HEIGHT + ((coor.y < 0) ? -1 : 0));
		wCoor.z += (coor.z / CHUNK_LENGTH + ((coor.z < 0) ? -1 : 0));
		coor.x = (coor.x + CHUNK_WIDTH) % CHUNK_WIDTH;
		coor.y = (coor.y + CHUNK_HEIGHT) % CHUNK_HEIGHT;
		coor.z = (coor.z + CHUNK_LENGTH) % CHUNK_LENGTH;
		if (world->worldMap[World::ChunkPos(wCoor)] == NULL)
		{
			return nullptr;
		}
		return (world->worldMap[World::ChunkPos(wCoor)]->getBlock(coor));
	}
	return getBlock(Coor2Pos(coor));
	return nullptr;
}

Block* Chunk::getBlock(GLushort Pos) const
{
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
	_VBO = new VBO(&chunkVoxel[0].Position, numFace*sizeof(vertex));
	_VAO->LinkVBO(*_VBO, 0, 1, 2 * sizeof(int), 0);
	_VAO->LinkVBO(*_VBO, 1, 1, 2 * sizeof(int), sizeof(int));
	_VAO->Unbind();
	_VBO->Delete();
	cleanMem(chunkVoxel);

	ChunkDoRender = true;
}

