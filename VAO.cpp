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
	glVertexAttribPointer(Layout, size, GL_FLOAT, GL_FALSE, stride, (char*)start);
	VBO.Unbind();
}

void VAO::Delete() const
{
	Unbind();
	glDeleteVertexArrays(1, &ID);
	delete this;
}
