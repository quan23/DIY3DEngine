#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include "ShaderClass.h"

class Texture
{
	public:
		GLuint ID;
		GLenum type;
		GLenum slot;

		Texture(const char* Name, GLenum Type, GLenum slot, GLenum format, GLenum bytetype);
		void linkTex(Shader& shader, const char* texID, GLint unit);
		void Bind();
		void Unbind();
		void Delete();
};

#endif
