#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
}

void VAO::LinkVBO(VBO& VBO, GLuint Layout, GLuint size, GLuint stride, GLuint start)
{
	VBO.Bind();
	glEnableVertexAttribArray(Layout);
	glVertexAttribPointer(Layout, size, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (char*)(sizeof(float)*start));
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}