#include "Chunk.h"
#include "Block.h"


using namespace glm;

glm::vec3 Block::Vertex[] =
{
	vec3(0.0f,0.0f,0.0f),
	vec3(1.0f,0.0f,0.0f),
	vec3(1.0f,1.0f,0.0f),
	vec3(0.0f,1.0f,0.0f),
	vec3(0.0f,1.0f,1.0f),
	vec3(0.0f,0.0f,1.0f),
	vec3(1.0f,0.0f,1.0f),
	vec3(1.0f,1.0f,1.0f),
};
GLushort Block::Indices[] =
{
	3,4,7,2,
	6,7,4,5,
	5,4,3,0,
	0,3,2,1,
	1,2,7,6,
	1,6,5,0,
};

glm::vec2 TextureCoor[4] =
{
	glm::vec2(0.0f,0.0f),
	glm::vec2(0.0f,1.0f),
	glm::vec2(1.0f,1.0f),
	glm::vec2(1.0f,0.0f),
};

Block::Block(char blockID) : blockID(blockID)
{

}
/*
face Block::getFace(Block::faceID Face)
{
	return face(Vertex[Indices[Face * 4]], Vertex[Indices[Face * 4 + 1]], Vertex[Indices[Face * 4 + 2]], Vertex[Indices[Face * 4 + 3]]);
}
*/
void Block::getFace(Chunk& chunk, blockCoor coor) const
{
	if (blockID != 0)
	{
		for (char face = faceID::TOP; face <= faceID::BOTTOM; face++)
		{
			//std::cout << coor << " " << (int)face << "\n";
			if (chunk.getBlock(coor + Chunk::nearBlock[face]) == nullptr || chunk.getBlock(coor + Chunk::nearBlock[face])->blockID == 0)
			{
				for (char i = 0; i < 4; i++)
				{
					chunk.chunkVertex.push_back(vertex(Chunk::Coor2Pos(coor), int(face << 2) + i));
					chunk.numVertex++;
				}
				chunk.chunkIndices.push_back(chunk.numFace * 4 + 0);
				chunk.chunkIndices.push_back(chunk.numFace * 4 + 1);
				chunk.chunkIndices.push_back(chunk.numFace * 4 + 2);
				chunk.chunkIndices.push_back(chunk.numFace * 4 + 0);
				chunk.chunkIndices.push_back(chunk.numFace * 4 + 2);
				chunk.chunkIndices.push_back(chunk.numFace * 4 + 3);
				chunk.numFace++;
				Chunk::totalFace++;
				//std::cout << -1;
			}
		}
	}
}

void Block::getVoxelFace(Chunk& chunk, blockCoor coor) const
{
	if (blockID != 0)
	{
		for (char face = faceID::TOP; face <= faceID::BOTTOM; face++)
		{
			//std::cout << coor << " " << (int)face << "\n";
			if (chunk.getBlock(coor + Chunk::nearBlock[face]) != nullptr && chunk.getBlock(coor + Chunk::nearBlock[face])->blockID == 0)
			{
				chunk.chunkVoxel.push_back(vertex(Chunk::Coor2Pos(coor),int(face)+6*blockID-6));
				//chunk.chunkIndices.push_back(chunk.numFace);
				chunk.numFace++;
				Chunk::totalFace++;
				//std::cout << -1;
			}
		}
	}
}

