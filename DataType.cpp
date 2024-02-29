#include "DataType.h"


Coor::Coor(signed char x, signed char y, signed char z) : x(x), y(y), z(z)
{

}

Coor Coor::operator+(const Coor& coor)
{
	return Coor(this->x+coor.x,this->y+coor.y,this->z+coor.z);
}

bool Coor::operator==(const Coor& Coor)
{
	return (this->x == Coor.x) && (this->y == Coor.y) && (this->z == Coor.z);
}

bool Coor::operator!=(const Coor& Coor)
{
	return !(*this == Coor);
}

worldCoor::worldCoor(int x, int y, int z) : x(x), y(y), z(z)
{

}

worldCoor worldCoor::operator+(const worldCoor& coor)
{
	return worldCoor(this->x + coor.x, this->y + coor.y, this->z + coor.z);
}

std::ostream& operator<<(std::ostream& stream, const Coor& coor)
{
	stream << (int)coor.x << " " << (int)coor.y << " " << (int)coor.z;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const worldCoor& coor)
{
	stream << coor.x << " " << coor.y << " " << coor.z;
	return stream;
}

face face::operator+(const glm::vec3& coor)
{
	for (glm::vec3& vertex : this->vertex) vertex = vertex + coor;
	return *this;
}

face::face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	vertex[0] = v0;
	vertex[1] = v1;
	vertex[2] = v2;
	vertex[3] = v3;
}

vertex::vertex(int Position, int data) : Position(Position), data(data)
{
}
