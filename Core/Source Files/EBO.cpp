#include "EBO.h"

EBO::EBO(GLushort* indices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::Delete() const 
{
	Unbind();
	glDeleteBuffers(1, &ID);
	delete this;
}