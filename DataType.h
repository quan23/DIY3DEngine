#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <iostream>
#include <glm/glm.hpp>

struct Coor
{
	signed char x, y, z;
	Coor(signed char x, signed char y, signed char z);
	Coor operator+(const Coor& coor);
	bool operator==(const Coor& Coor);
	bool operator!=(const Coor& Coor);
};
struct worldCoor
{
	int x, y, z;
	worldCoor(int x = 0, int y = 0, int z = 0);
	worldCoor operator+(const worldCoor& coor);
};
struct face
{
	glm::vec3 vertex[4];
	face operator+(const glm::vec3& coor);
	face() = default;
	face(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
};
struct vertex
{
	int Position;
	int data;
	vertex() = default;
	vertex(int Position, int data);
};

std::ostream& operator<<(std::ostream& stream, const worldCoor& coor);

#endif
