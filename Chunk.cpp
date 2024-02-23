#include "World.h"
#include "Block.h"
#include "Chunk.h";

Coor Chunk::nearBlock[] =
{
	Coor(0,1,0),
	Coor(0,0,1),
	Coor(-1,0,0),
	Coor(0,0,-1),
	Coor(1,0,0),
	Coor(0,-1,0),
};

int Chunk::totalIndices = 0;

Chunk::Chunk(int x, int y, int z, World* world)
{
	//std::cout << "-1\n";
	ChunkCoor = worldCoor(x, y, z);
	this->world = world;
	for (char x = 0; x < CHUNK_WIDTH; x++)
	{
		int realX = x + CHUNK_WIDTH * ChunkCoor.x;
		for (char z = 0; z < CHUNK_LENGTH; z++)
		{
			int realZ = z + CHUNK_LENGTH * ChunkCoor.z;
			char high = 8.0f + (glm::sin(float(realX)) + glm::cos(float(realZ)));
			//std::cout << glm::sinh(float(realX)) * glm::sinh(float(realZ)) << "\n";
			for (char y = 0; y < CHUNK_HEIGHT; y++)
			{
				int realY = y + CHUNK_HEIGHT * ChunkCoor.y;
				if (realY < high)
				{
					EmptyChunk = false;
					getBlock(Coor(x, y, z))->blockID = 1;
				}
				//else getBlock(Coor(x, y, z))->blockID = 0;
			}
		}
	}
	if (EmptyChunk)
	{
		ChunkDoRender = false;
		delete[] blockList;
	}
	//updateFace();
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
	std::vector <face> chunkFace;
	std::vector <vertex> chunkVertex;
	std::vector <GLushort> chunkIndices;
	face thisFace;
	GLushort thisIndice[4];
	GLushort (*indicesID)(glm::vec3&) = [](glm::vec3& coor)
	{
		return GLushort(GLushort(coor.x) + (GLushort(coor.y) * (CHUNK_LENGTH+1) + GLushort(coor.z)) * (CHUNK_WIDTH+1));
	};
	for (char x = 0; x < CHUNK_WIDTH; x++)
	{
		for (char z = 0; z < CHUNK_LENGTH; z++)
		{
			for (char y = 0; y < CHUNK_HEIGHT; y++)
			{
				if (getBlock(Coor(x, y, z))->blockID != 0)
				{
					for (char face = Block::faceID::TOP; face <= Block::faceID::BOTTOM; face++)
					{
						if (getBlock(Coor(x, y, z) + nearBlock[face]) == nullptr || getBlock(Coor(x, y, z) + nearBlock[face])->blockID == 0)
						{
							for (char i = 0; i < 4; i++)
							{
								chunkVertex.push_back(vertex(Coor2Pos(Coor(x, y, z)), (face << 2) + i));
								numVertex++;
								totalIndices++;
							}
							chunkIndices.push_back(numFace*4+0);
							chunkIndices.push_back(numFace*4+1);
							chunkIndices.push_back(numFace*4+2);
							chunkIndices.push_back(numFace*4+0);
							chunkIndices.push_back(numFace*4+2);
							chunkIndices.push_back(numFace*4+3);
							numFace++;
						}
					}
				}
			}
		}
	}
	//std::cout << numFace << " " << chunkIndices.size();
	//std::cout << chunkIndices.size() << "\n";
	if (chunkVertex.size()==0)
	{
		ChunkDoRender = false;
		return;
	}
	else ChunkDoRender = true;
	_VAO = new VAO;
	_VBO = new VBO(&chunkVertex[0].Position, chunkVertex.size() * sizeof(vertex));
	_EBO = new EBO(&chunkIndices[0], chunkIndices.size() * sizeof(GLushort));
	_VAO->LinkVBO(*_VBO, 0, 1, 2*sizeof(int), 0);
	_VAO->LinkVBO(*_VBO, 1, 1, 2*sizeof(int), sizeof(int));
	_VAO->Unbind();
	_VBO->Delete();
	_EBO->Delete();
}

void Chunk::render(GLuint ShaderProgram)
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
		glDrawElements(GL_TRIANGLES, numFace*6, GL_UNSIGNED_SHORT, 0);
		_VAO->Unbind();
	}
}

Block* Chunk::getBlock(Coor coor)
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
			std::cout << "-1\n";
		}
		return (world->worldMap[World::ChunkPos(wCoor)]->getBlock(coor));
		return nullptr;
	}
	return getBlock(Coor2Pos(coor));
	return nullptr;
}

Block* Chunk::getBlock(GLushort Pos)
{
	if (EmptyChunk) return nullptr;
	if ((Pos >= 0) && (Pos < CHUNK_SIZE)) return &(blockList[Pos]);
	return nullptr;
}

