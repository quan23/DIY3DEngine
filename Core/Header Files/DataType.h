#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <iostream>
#include <glm/glm.hpp>

struct Coor
{
	signed char x, y, z;
	Coor(signed char x = 0, signed char y = 0, signed char z = 0) : x(x), y(y), z(z) {}
	Coor operator+(const Coor& coor) const;
	bool operator==(const Coor& Coor) const;
	bool operator!=(const Coor& Coor) const;
};
struct worldCoor
{
	int x, y, z;
	worldCoor(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}
	worldCoor operator+(const worldCoor& coor) const;
	bool operator==(const worldCoor& Coor) const;
	bool operator!=(const worldCoor& Coor) const;
};
struct blockCoor
{
	long long x, y, z;
	blockCoor(long long x = 0, long long y = 0, long long z = 0) : x(x), y(y), z(z) {}
	blockCoor(Coor coor) : x(coor.x), y(coor.y), z(coor.z) {}
	blockCoor operator+(const blockCoor& coor) const;
	bool operator==(const blockCoor& Coor) const;
	bool operator!=(const blockCoor& Coor) const;
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
struct pixel
{
	unsigned char r = 0, g = 0, b = 0, a = 0;
	pixel() = default;
	pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
};

std::ostream& operator<<(std::ostream& stream, const Coor& coor);
std::ostream& operator<<(std::ostream& stream, const worldCoor& coor);
std::ostream& operator<<(std::ostream& stream, const blockCoor& coor);

template <>
struct std::hash<worldCoor>
{
	std::size_t operator()(const worldCoor& coor) const
	{
		std::size_t worldPos = (coor.x + 0xfff) & 0xfff;
		worldPos <<= 8;
		worldPos += (coor.y + 0xff) & 0xff;
		worldPos <<= 12;
		worldPos += (coor.z + 0xfff) & 0xfff;
		return worldPos;
	}
};

#endif
