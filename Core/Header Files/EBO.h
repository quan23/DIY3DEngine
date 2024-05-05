#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO
{
	public:
		GLuint ID;
		EBO(GLushort *indices, GLsizeiptr size);
		
		inline void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
		inline void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
		void Delete() const;
};

#endif
