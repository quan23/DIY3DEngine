#include "Chunk.h";

Chunk::Coor Chunk::nearBlock[] =
{
	Coor(0,1,0),
	Coor(0,0,1),
	Coor(-1,0,0),
	Coor(0,0,-1),
	Coor(1,0,0),
	Coor(0,-1,0),
};

Chunk::Chunk(int x, int y, int z)
{
	ChunkCoor = worldCoor(x, y, z);
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
				else getBlock(Coor(x, y, z))->blockID = 0;
			}
		}
	}
	if (EmptyChunk)
	{
		ChunkDoRender = false;
		delete[] indicesCheck;
		delete[] blockList;
	}
	updateFace();
}

unsigned int Chunk::Coor2Pos(Coor coor)
{
	return (coor.x + (coor.y * CHUNK_WIDTH + coor.z) * CHUNK_LENGTH);
}

Chunk::Coor Chunk::Pos2Coor(unsigned int pos)
{
	return Coor(char((pos % (CHUNK_WIDTH * CHUNK_LENGTH)) % CHUNK_WIDTH), char(pos / (CHUNK_WIDTH * CHUNK_LENGTH)), char((pos % (CHUNK_WIDTH * CHUNK_LENGTH)) / CHUNK_LENGTH));
}

void Chunk::updateFace()
{
	Block::face thisFace;
	GLushort thisIndice[4];
	GLushort (*indicesID)(glm::vec3&) = [](glm::vec3& coor)
	{
		return GLushort(GLushort(coor.x) + (GLushort(coor.y) * (CHUNK_LENGTH+1) + GLushort(coor.z)) * (CHUNK_WIDTH+1));
	};
	for (GLushort indices = 0; indices < (CHUNK_WIDTH + 1) * (CHUNK_LENGTH + 1) * (CHUNK_HEIGHT + 1) + 1; indices++) indicesCheck[indices] = GLushort(-1);
	
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
							thisFace = Block::getFace(Block::faceID(face)) + glm::vec3(x, y, z);
							for (char Vertex = 0; Vertex < 4; Vertex++)
							{
								if (indicesCheck[indicesID(thisFace.vertex[Vertex])] == GLushort(-1))
								{
									chunkVertex.push_back(thisFace.vertex[Vertex]);
									indicesCheck[indicesID(thisFace.vertex[Vertex])] = numVertex;
									numVertex++;
								}
								thisIndice[Vertex] = indicesCheck[indicesID(thisFace.vertex[Vertex])];
							}
							chunkIndices.push_back(thisIndice[0]);
							chunkIndices.push_back(thisIndice[1]);
							chunkIndices.push_back(thisIndice[2]);
							chunkIndices.push_back(thisIndice[0]);
							chunkIndices.push_back(thisIndice[2]);
							chunkIndices.push_back(thisIndice[3]);
							numFace++;
						}
					}
				}
			}
		}
	}
	//std::cout << numFace << " " << chunkIndices.size();
	//std::cout << chunkIndices.size() << "\n";
	if (!chunkVertex.size())
	{
		ChunkDoRender = false;
		return;
	}
	else ChunkDoRender = true;
	_VAO = new VAO;
	_VBO = new VBO(&chunkVertex[0][0], chunkVertex.size() * sizeof(glm::vec3));
	_EBO = new EBO(&chunkIndices[0], chunkIndices.size() * sizeof(GLushort));
	_VAO->LinkVBO(*_VBO, 0, 3, 3, 0);
	_VAO->Unbind();
	_VBO->Unbind();
	_EBO->Unbind();
}

void Chunk::render(GLuint ShaderProgram)
{
	if (ChunkDoRender)
	{
		_VAO->Bind();
		glUniform1ui(glGetUniformLocation(ShaderProgram, "CHUNK_WIDTH"), CHUNK_WIDTH);
		glUniform1ui(glGetUniformLocation(ShaderProgram, "CHUNK_LENGTH"), CHUNK_LENGTH);
		glUniform1ui(glGetUniformLocation(ShaderProgram, "CHUNK_HEIGHT"), CHUNK_HEIGHT);
		glUniform3i(glGetUniformLocation(ShaderProgram, "chunkPos"), ChunkCoor.x, ChunkCoor.y, ChunkCoor.z);
		glDrawElements(GL_TRIANGLES, chunkIndices.size(), GL_UNSIGNED_SHORT, 0);
		//_VAO->Unbind();
	}
}

Block* Chunk::getBlock(Coor Coor)
{
	if ((Coor.x < 0) || (Coor.x >= CHUNK_WIDTH) || (Coor.y < 0) || (Coor.y >= CHUNK_HEIGHT) || (Coor.z < 0) || (Coor.z >= CHUNK_LENGTH)) return nullptr;
	int BlockPos = Chunk::Coor2Pos(Coor);
	return getBlock(BlockPos);
	return nullptr;
}

Block* Chunk::getBlock(GLushort Pos)
{
	if ((Pos >= 0) && (Pos < CHUNK_SIZE)) return &(blockList[Pos]);
	return nullptr;
}

Chunk::Coor::Coor(char x, char y, char z) : x(x), y(y), z(z)
{

}

Chunk::Coor Chunk::Coor::operator+(const Coor& coor)
{
	this->x += coor.x;
	this->y += coor.y;
	this->z += coor.z;
	return *this;
}

bool Chunk::Coor::operator==(const Coor& Coor)
{
	return (this->x == Coor.x) && (this->y == Coor.y) && (this->z == Coor.z);
}

bool Chunk::Coor::operator!=(const Coor& Coor)
{
	return !(*this == Coor);
}

Chunk::worldCoor::worldCoor(int x, int y, int z) : x(x), y(y), z(z)
{

}

Chunk::worldCoor Chunk::worldCoor::operator+(const worldCoor& coor)
{
	this->x += coor.x;
	this->y += coor.y;
	this->z += coor.z;
	return *this;
}

std::ostream& Chunk::worldCoor::operator<<(std::ostream& stream)
{
	stream << this->x << " " << this->y << " " << this->z;
	return stream;
}
