#include "Block.h"


using namespace glm;
glm::vec3 Block::vertex[] =
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
GLushort Block::indices[] =
{
	3,4,7/*,3,7*/,2,
	6,7,4/*,6,4*/,5,
	5,4,3/*,5,3*/,0,
	0,3,2/*,0,2*/,1,
	1,2,7/*,1,7*/,6,
	1,6,5/*,1,5*/,0,
};

Block::Block(char blockID) : blockID(blockID)
{

}

Block::face Block::getFace(Block::faceID face)
{
	return Block::face({ vertex[indices[face * 4]], vertex[indices[face * 4 + 1]] ,vertex[indices[face * 4 + 2]] ,vertex[indices[face * 4 + 3]] });
}

Block::face Block::face::operator+(const glm::vec3& coor)
{
	for (glm::vec3& vertex : this->vertex) vertex = vertex + coor;
	return *this;
}
