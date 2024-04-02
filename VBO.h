#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
	public:
		GLuint ID;
		VBO(GLfloat* vertices, GLsizeiptr size);
		VBO(int* vertices, GLsizeiptr size);

		inline void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, ID); }
		inline void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
		void Delete() const;
};

#endif