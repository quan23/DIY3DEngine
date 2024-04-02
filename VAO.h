#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
	public:
		GLuint ID;
		VAO();

		void LinkVBO(VBO& VBO, GLuint Layout, GLuint size, GLuint stride, GLuint start);
		inline void Bind() const { glBindVertexArray(ID); }
		inline void Unbind() const { glBindVertexArray(0); }
		void Delete() const;
};

#endif